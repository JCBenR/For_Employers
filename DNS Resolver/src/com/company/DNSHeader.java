package com.company;

import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.io.Serializable;
import java.nio.ByteBuffer;
import java.util.Arrays;

//implemented Serializable to convert it byte array
public class DNSHeader implements Serializable {

    byte [] headerID, flags, QDcount, ANcount, NScount, ARcount;
    byte QR, Opcode, AA, TC, RD, RA, Z, AD, CD, RCODE;
    int questionCount;

    /**
     * decodes the header, pulling out flags and counts. assigns to appropriate fields (byte arrays)
     * @param inputStream: input stream being read from
     * @return DNSHeader with all fields assigned
     * @throws IOException
     */
    public static DNSHeader decodeHeader(ByteArrayInputStream inputStream) throws IOException {
        DNSHeader header = new DNSHeader();
        header.headerID = inputStream.readNBytes(2);
//        System.out.println(getIntcout(header.headerID));
        header.flags = inputStream.readNBytes(2);
        //byte [] thirdByte = header.flags;
        header.QDcount = inputStream.readNBytes(2); //should be 1
        header.questionCount = header.getIntcout(header.QDcount);
        header.ANcount = inputStream.readNBytes(2); //should be 0 on request, 1 on response
        header.NScount = inputStream.readNBytes(2); //should be 0
        header.ARcount = inputStream.readNBytes(2); //should be 0

//        header.QR = (byte)(thirdByte[0] & 0b00000000); //should be 0 for request, 1 for response
//        header.Opcode = (byte)(thirdByte[0] & 0b01111000); //should always be 0
//        header.AA = (byte)(thirdByte[0] & 0b00000100); //should always be 0
//        header.TC = (byte)(thirdByte[0] & 0b00000010); //should always be 0
//        header.RD = (byte)(thirdByte[0] & 0b00000001); //should always be 1
//        header.RA = (byte)(thirdByte[1] & 0b10000000); //should always be 1
//        header.Z = (byte)(thirdByte[1]  & 0b01000000);
//        header.AD = (byte)(thirdByte[1] & 0b00100000);
//        header.CD = (byte)(thirdByte[1] & 0b00010000);
//        header.RCODE = (byte)(thirdByte[1] & 0b0001111);

        System.out.println(header.toString());
        return header;

    }

    /**
     * builds response header using fields from original request and with updated fields based on answers found in cache or from google. NOTE: this is passed last to the build response because it depends on other fields for getting correct counts
     * @param request original request
     * @param response response being constructed.
     * @return DNSHeader with RCODE set to 0
     */
    static DNSHeader buildResponseHeader(DNSMessage request, DNSMessage response){
        DNSHeader headerResponse = request.header;
        headerResponse.headerID = request.header.headerID;
//        System.out.println(getIntcout(headerResponse.headerID));
        headerResponse.flags = setHeaderBits(request.header.flags);
        headerResponse.QDcount = request.header.QDcount;
        headerResponse.ANcount = intoBytes(response.answerRecords.size());
        headerResponse.NScount = intoBytes(response.authRecords.size());
        headerResponse.ARcount = intoBytes(response.additionalRecords.size());
        return headerResponse;
    }

    /**
     * builds the BAD response header when a record is not received from google.
     * @param request original request
     * @param response response being built
     * @return DNSHeader with flags in RCODE set to 3 (code for non existent URL)
     */
    static DNSHeader buildBadResponseHeader(DNSMessage request, DNSMessage response){
        DNSHeader headerResponse = request.header;
        headerResponse.headerID = request.header.headerID;
//        System.out.println(getIntcout(headerResponse.headerID));
        headerResponse.flags = setBadHeaderBits(request.header.flags);
        headerResponse.QDcount = request.header.QDcount;
        headerResponse.ANcount = intoBytes(response.answerRecords.size());
        headerResponse.NScount = intoBytes(response.authRecords.size());
        headerResponse.ARcount = intoBytes(response.additionalRecords.size());
        return headerResponse;
    }

    /**
     * sets bits in header flags to appropriate signals for response.
     * @param flags byte array of flags
     * @return byte array of flags for response
     */
    static byte [] setHeaderBits(byte [] flags){
        byte [] swapped = flags;
        byte flag0 = flags[0];
        byte flag1 = flags[1];
        swapped[0] = (byte) (0b10000000 | flag0);
        swapped[1] = (byte) (0b10000000 | flag1);

        return swapped;
    }

    /**
     * sets bits in header for response of non existent URL
     * @param flags byte array of flags
     * @return byte array of flags for response with RCODE set to 3 for non existent URL
     */
    static byte [] setBadHeaderBits(byte [] flags){
        byte [] swapped = flags;
        byte flag0 = flags[0];
        byte flag1 = flags[1];
        swapped[0] = (byte) (0b10000000 | flag0);
        swapped[1] = (byte) (0b10000011 | flag1);

        return swapped;
    }

    /**
     * helper funciton that takes in an interger and returns a byte [] of size 2.
     * @param i interger to change into byte array
     * @return byte array of size 2 of the interger
     */
    static byte [] intoBytes(int i){
        ByteBuffer buffer = ByteBuffer.allocate(2);
        buffer.putShort((short)i);
        return buffer.array();
    }

    /**
     * writes bytes to output stream
     * @param outputStream outpust stream to write to
     * @throws IOException
     */
    void writeBytes(OutputStream outputStream) throws IOException {
        outputStream.write(headerID);
        outputStream.write(flags);
        outputStream.write(QDcount);
        outputStream.write(ANcount);
        outputStream.write(NScount);
        outputStream.write(ARcount);
    }

    /**
     * helper function to get int out of byte array
     * @param arr array to translate
     * @return int for byte array value
     */
    static int getIntcout(byte [] arr){
        return (arr[1] & 0xFF | arr[0] <<8);
    }


    @Override
    public String toString() {
        return "DNSHeader{" +
                "headerID=" + Arrays.toString(headerID) +
                ", flags=" + Arrays.toString(flags) +
                ", QDcount=" + Arrays.toString(QDcount) +
                ", ANcount=" + Arrays.toString(ANcount) +
                ", NScount=" + Arrays.toString(NScount) +
                ", ARcount=" + Arrays.toString(ARcount) +
                ", QR=" + QR +
                ", Opcode=" + Opcode +
                ", AA=" + AA +
                ", TC=" + TC +
                ", RD=" + RD +
                ", RA=" + RA +
                ", Z=" + Z +
                ", AD=" + AD +
                ", CD=" + CD +
                ", RCODE=" + RCODE +
                ", questionCount=" + questionCount +
                '}';
    }
}
