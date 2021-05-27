package com.company;

//                                                                            1  1  1  1  1  1
//                                              0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
//                                            +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//                                            |                      ID                       |
//                                            +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//                                            |QR|   Opcode  |AA|TC|RD|RA|   Z    |   RCODE   |
//                                            +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//                                            |                    QDCOUNT                    |
//                                            +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//                                            |                    ANCOUNT                    |
//                                            +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//                                            |                    NSCOUNT                    |
//                                            +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
//                                            |                    ARCOUNT                    |
//                                            +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+


import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.*;
import java.util.Calendar;
import java.util.Scanner;

public class Main {

//    byte [] sampleArry = {58, ef, 68, 11, 0e, c1, 14 7d da 3b 0f 76 86 dd 60 0e
//            19 0b 00 2f 11 40 26 01 06 81 4b 00 0b 99 d0 0e
//    b7 00 5d 23 89 b2 26 01 06 81 4b 00 0b 99 5a ef
//                68 ff fe 11 0e c1 de ca 00 35 00 2f c0 43 de 84
//            01 20 00 01 00 00 00 00 00 01 06 67 6f 6f 67 6c
//                65 03 63 6f 6d 00 00 01 00 01 00 00 29 10 00 00
//                        00 00 00 00 00};


    public static void main(String[] args) throws IOException {

//        byte[] byteArray = new byte[]{
//                (byte) 0xdb, 0x42, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
//                ,0x03, 0x77, 0x77, 0x77, 0x0c, 0x6e, 0x6f, 0x72, 0x74, 0x68, 0x65, 0x61, 0x73, 0x74, 0x65, 0x72, 0x6e,
//                0x03, 0x65, 0x64, 0x75, 0x00, 0x00, 0x01, 0x00, 0x01
//        };

        DNSServer server = new DNSServer();
        server.createServer();
    }


}
