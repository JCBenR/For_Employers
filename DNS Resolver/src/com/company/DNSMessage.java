package com.company;

import java.io.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;

public class DNSMessage {

    DNSHeader header = new DNSHeader();
    ArrayList<DNSQuestion> questions = new ArrayList<>();
    ArrayList<DNSRecord> answerRecords = new ArrayList<>();
    ArrayList<DNSRecord> authRecords = new ArrayList<>();
    ArrayList<DNSRecord> additionalRecords = new ArrayList<>();
    DNSRecord record = new DNSRecord();
    byte [] entireMessage;

    /**
     * decodes the message and its various parts
     * @param bytes byte array of original request
     * @return full request decoded and parsed
     * @throws IOException
     */
    static DNSMessage decodeMessage(byte[] bytes) throws IOException {
        DNSMessage fullMessage = new DNSMessage();
        ByteArrayInputStream inputStream = new ByteArrayInputStream(bytes);
        fullMessage.entireMessage = bytes;
        fullMessage.header = DNSHeader.decodeHeader(inputStream);
        for (int i = 0; i < fullMessage.header.questionCount; i++) {
           fullMessage.questions.add(DNSQuestion.decodeQuestion(inputStream, fullMessage));
        };
        for (int i = 0; i < DNSHeader.getIntcout(fullMessage.header.ANcount) ;
        i++){
            fullMessage.answerRecords.add(DNSRecord.decodeRecord(inputStream, fullMessage));
        };
        for (int i = 0; i < DNSHeader.getIntcout(fullMessage.header.NScount) ;
             i++){
            fullMessage.authRecords.add(DNSRecord.decodeRecord(inputStream, fullMessage));
        };
        for (int i = 0; i < DNSHeader.getIntcout(fullMessage.header.ARcount) ;
             i++){
            fullMessage.additionalRecords.add(DNSRecord.decodeRecord(inputStream, fullMessage));
            System.out.println("auth records are");
            System.out.println(fullMessage.additionalRecords);
        }
        //turn to byte array
        ByteArrayOutputStream bos = new ByteArrayOutputStream();
        ObjectOutputStream out = null;
        try{
            out = new ObjectOutputStream(bos);
            out.writeObject(fullMessage.header);
            out.writeObject(fullMessage.questions);
            out.flush();
            fullMessage.entireMessage = bos.toByteArray();
        } finally {
            try{
                bos.close();
            } catch (IOException ex){
                System.err.println(ex);
            }
        }
        return fullMessage;
    }


    /**
     * function creates arrayList of strings to put pieces into
     * first, check if first two BITS are 0b00000011. if so, this indicates what follows is a reference to a pointer of the previousl URL name.
     * in that case, need to remove the first bits with a mask (ob00111111) and OR it with the next byte to get the offset.
     * pass that offset to  readDomainName function and read from there.
     * if it is not an offset create new stringbuilder
     * octet will then tell you number of characters to read
     * read that many characters in for loop and create a string of them.
     * add string to array and repeate for all words in the URL string.
     * will terminate when 0 is found.
     * assign url string [] the size of the arraylist of strings
     * loop through arraylist of strings and add all strings to string [].
     * return url string [].
     * @param inputStream --stream of bytes to be read
     * @return returns the domain in a string array
     * @throws IOException
     */
    String[] readDomainName(InputStream inputStream) throws IOException {
        ArrayList<String> qNameStringList = new ArrayList<>();
        String [] url;
        int octet = inputStream.read();
        int compressionCheck = octet >> 6;
        if (compressionCheck == 3){
            byte removeBits = (byte) (0b00111111 & octet);
            int getOffset = (removeBits << 8 | inputStream.read());
            url = readDomainName(getOffset);
        }
        else
            {
            while(octet != 0x00){
                StringBuilder currentString = new StringBuilder();
                for (int i = 0; i < octet; i++) {
                    currentString.append((char)inputStream.read());
                }
                qNameStringList.add(currentString.toString());
                octet = inputStream.read();
            }
            url = new String[qNameStringList.size()];

            for (int i = 0; i < qNameStringList.size(); i++) {
                url[i] = qNameStringList.get(i);
            }
        }
        return url;
    }

    /**
     * purpose of function is to read the domain name starting at an offset (for compression). the offset will tell the inputStream how much to skip. in most cases this will be 12 bytes (length of the header) and the domain name will start from there. in other instances it may be more.
     * use the .skip() to skip over from that point.
     * using the entireMessage byte [] to be able to jump around.
     * @param firstByte offset to go to
     * @return results of readDomainName from that point
     * @throws IOException
     */
    String [] readDomainName(int firstByte) throws IOException {
        InputStream inputStream = new ByteArrayInputStream(entireMessage); //need to figure out where to start.
        inputStream.skip(firstByte);
        return readDomainName(inputStream);
    }

    /**
     * builds response. note the order of this is backwards with header last because the header needs to have info from the other fields to be populated correctly
     * @param request original request
     * @param answers array of answers from cache
     * @return DNSMessage to be sent to client
     */
    static DNSMessage buildResponse(DNSMessage request, ArrayList<DNSRecord> answers){
        System.out.println("*****answer records from array: " + answers);
            DNSMessage response = new DNSMessage();
            response.questions = request.questions;
            response.answerRecords = answers;
            response.authRecords = request.authRecords;
            response.additionalRecords = request.additionalRecords;
            response.header = DNSHeader.buildResponseHeader(request, response);
            return response;
    }

    /**
     * if google returns an RCODE of 3, means the URL does not exist. this crafts message based on that info.
     * @param request original request
     * @return message formatted for non existent URL
     */
    static DNSMessage buildNotFoundResponse(DNSMessage request){
        DNSMessage response = new DNSMessage();
        response.questions = request.questions;
//        response.authRecords = request.authRecords;
//        response.additionalRecords = request.additionalRecords;
        response.header = DNSHeader.buildBadResponseHeader(request, response);
        return response;
    }

    /**
     * writes message to bytes for transfer in datagram packet
     * @return byte array
     * @throws IOException
     */
    byte[] toBytes() throws IOException {

        HashMap<String, Integer> domainLocations = new HashMap<>();
        ByteArrayOutputStream outputStream = new ByteArrayOutputStream();

        header.writeBytes(outputStream);

        for (DNSQuestion question :
                questions) {
            question.writeBytes(outputStream, domainLocations);
        }
        for (DNSRecord answer :
                answerRecords) {
            answer.writeBytes(outputStream, domainLocations);
        }
        for (DNSRecord auth :
                authRecords) {
            auth.writeBytes(outputStream,domainLocations);
        }
        for (DNSRecord addRec :
                additionalRecords) { //maybe do loop to see if greater than 0
                addRec.writeBytes(outputStream, domainLocations);
        }

        return outputStream.toByteArray();
    }

    /**
     * writes domain names to outgoing message using the domainPieces array list. used for compression
     * @param outputStream stream to write to
     * @param domainLocations hashmap of name pieces and their pointers
     * @param domainPieces array of pieces of domain octets
     * @throws IOException
     */
    static void writeDomainName(ByteArrayOutputStream outputStream, HashMap<String,Integer> domainLocations, String[] domainPieces) throws IOException {
        System.out.println("domain pieces");
        for (String s :
                domainPieces) {
            System.out.println(s);
        }
        String s = octetsToString(domainPieces);
            if (domainLocations.containsKey(s)){
                byte [] offset = DNSHeader.intoBytes(domainLocations.get(s));
                offset[0] = (byte)(offset[0]| 0xc0 );
                outputStream.write(offset);
            } else{
                int location = outputStream.size();//find current location
                domainLocations.put(s,location); //enter string and location into hashMap
                for(String str: domainPieces){
                    outputStream.write((byte)str.length());
                    for(char c : str.toCharArray()){
                        outputStream.write((byte) c);
                    }
                }
                outputStream.write(0x00); //
            }
    }

    /**
     * turns octets to string for output
     * @param octets pieces of domain
     * @return string of domain URL
     */
    static String octetsToString(String[] octets){
        StringBuilder urlString = new StringBuilder();
        for(int i = 0; i < octets.length; i++){
            urlString.append(octets[i]);
            if(i < octets.length - 1){
                urlString.append('.');
            }
        }
        return urlString.toString();
    }

    //METHOD DID NOT WORK
//    static String octetsToString(String[] octets){
//        StringBuilder urlString = new StringBuilder();
//        for (String o :
//                octets) {
//            urlString.append(o + '.');
//        }
//        int endOfString = urlString.length()-1;
//        urlString.deleteCharAt(endOfString);
//        return urlString.toString();
//    }

    @Override
    public String toString() {
        return "DNSMessage{" +
                "header=" + header +
                ", questions=" + questions +
                ", answerRecords=" + answerRecords +
                ", authRecords=" + authRecords +
                ", additionalRecords=" + additionalRecords +
                ", record=" + record +
                ", entireMessage=" + Arrays.toString(entireMessage) +
                '}';
    }
}
