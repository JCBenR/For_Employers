package com.company;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.Buffer;
import java.security.cert.X509Certificate;
import java.util.Arrays;
import java.util.Scanner;

public class ServerSocketMain {

    ServerSocket serverSocket;
    Socket connection;
    ObjectInputStream ois;
    ObjectOutputStream oos;
    ServerSocketMain(int port) throws IOException, ClassNotFoundException {

        serverSocket = new ServerSocket(port);

        System.out.println("server listening on port: " + port);

        while(true){
                connection = serverSocket.accept();
                System.out.println("connection received");
                oos = new ObjectOutputStream(connection.getOutputStream());
                ois = new ObjectInputStream(connection.getInputStream());
                byte[] message = (byte[]) ois.readObject();
                System.out.println("---Received Nonce---");
                byte [] nonce = getNonce(message);


//                try{
//
////                oos.writeObject("Server received message: " + message);
////                if ("exit".equals(message)){
////                    oos.writeObject("Server received exit message. now closing");
////                    oos.close();
////                    ois.close();
////                    connection.close();
////                } else {
////                    System.out.println(message);
////                    oos.writeObject("Server received message: " + message);
////                }
//            } catch (UnknownError | ClassNotFoundException e){
//                System.err.println(e);
////                oos.close();
////                ois.close();
////                connection.close();
//            }
            System.out.println("ending");
        }

    }
        public byte[] getNonce(byte [] clientNonce){
            System.out.println(Arrays.toString(clientNonce));
            return clientNonce;
        }

}
