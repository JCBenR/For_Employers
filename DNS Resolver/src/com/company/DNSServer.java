package com.company;


import java.io.IOException;
import java.net.*;
import java.util.Arrays;

public class DNSServer {
    DatagramSocket socket;
    DatagramPacket packet;
    DNSCache cache = new DNSCache();
    InetAddress clientIP = null;
    int clientPort;
    byte [] badAddressFlags = {-127, -125};
    int badAddressInt = -32381;
    public void createServer() throws IOException {

        /**
         * set socket to 8053. regular DNS port of 53 is reserved
         * allocate byte array for incoming requests
         */
        socket = new DatagramSocket(8053);
        byte [] receivedData = new byte[512];
        System.out.println("server listening for data");

        byte[] arr = { 0,1 };
        int num = (arr[1] & 0xFF | arr[0] <<8);

/**
 * keep socket open until closed or crashed
 * create datagram packet to receive incoming request
 * assign request to that packet
 * capture client IP and Port
 *
 */
        while (true){
            packet = new DatagramPacket(receivedData, receivedData.length);
            socket.receive(packet);
            clientIP = packet.getAddress();
            clientPort = packet.getPort();
            System.out.println("Got some data");

            DNSMessage message = DNSMessage.decodeMessage(receivedData);

            /**
             * for each question check cache and return valid answer. if question not found ask google (else block)
             * search cache for question
             * create response out of message (including answer records)
             * turn to byte array for sending in datagram packet
             * assign to datagram packet
             * send to client
             */
                for (DNSQuestion question :
                        message.questions) {
                    if (cache.cache.containsKey(question)) {
                        System.out.println(">>>>>>>>>found in cache<<<<<<<<<<");
                        message.answerRecords.add(cache.findCacheRecord(question));
                        DNSMessage response = DNSMessage.buildResponse(message, message.answerRecords);
                        byte [] responseArray = response.toBytes();
                        DatagramPacket responseToClient = new DatagramPacket(responseArray, responseArray.length, clientIP, clientPort);
                        System.out.println("Response to client****************");
                        System.out.println(Arrays.toString(responseArray));

                        socket.send(responseToClient);
                    }
                    else{
                        //ask google
                        DNSMessage googleMessage = sendToGoogle(receivedData);
                        /**
                         * check header flags in google response. if header flag matches bad/non-extistant URL flag, enter IF, otherwise enter ELSE
                         */
                        int headerFlags = DNSRecord.getIntcout(googleMessage.header.flags);
                        /**
                         * craft response with correct RCODE flag to send back
                         * send record
                         */
                        if (headerFlags == badAddressInt){
                            DNSMessage badResponse = DNSMessage.buildNotFoundResponse(googleMessage);
                            System.out.println("URL not found");
                            byte [] responseArray = badResponse.toBytes();
                            DatagramPacket responseToClient = new DatagramPacket(responseArray, responseArray.length, clientIP, clientPort);
                            socket.send(responseToClient);
                        } else{
                            DNSMessage response = DNSMessage.buildResponse(googleMessage, googleMessage.answerRecords);
                            System.out.println("answer records: ");
                            System.out.println(googleMessage.answerRecords);
                            byte [] responseArray = response.toBytes();
                            DatagramPacket responseToClient = new DatagramPacket(responseArray, responseArray.length, clientIP, clientPort);
                            System.out.println("Response to client****************");
                            System.out.println(Arrays.toString(responseArray));
                            socket.send(responseToClient);
                        }
                    }
                };
        }
    }

    /**
     * queries google DNS server for answer
     * @param receivedData request
     * @return record from google (if found) or response (if not found)
     * @throws IOException
     */
    DNSMessage sendToGoogle(byte [] receivedData) throws IOException {
        InetAddress myIP = InetAddress.getByName("8.8.8.8");
        DatagramSocket googleSocket = new DatagramSocket();
        DatagramPacket googleSend = new DatagramPacket(receivedData, receivedData.length, myIP, 53);
        googleSocket.send(googleSend);

        System.out.println("sending data...");
        System.out.println("got data from google");

        byte [] googleReceivedPacket = new byte[512];
        DatagramPacket googlePacket = new DatagramPacket(googleReceivedPacket, googleReceivedPacket.length, clientIP, clientPort);
        googleSocket.receive(googlePacket);
        DNSMessage googleMessage = DNSMessage.decodeMessage(googleReceivedPacket);

        //check header
//        DNSHeader googleHeader = googleMessage.header;
        System.out.println("FLAGS ARE: ");
        int headerFlags = DNSRecord.getIntcout(googleMessage.header.flags);
        if (headerFlags == badAddressInt){
            System.out.println("bad address");
            googleSocket.close();
            return googleMessage;
        }
        else{
            cache.enterCacheResult(googleMessage);
            System.out.println("good address");
            googleSocket.close();
            return googleMessage;
        }
    }
}

