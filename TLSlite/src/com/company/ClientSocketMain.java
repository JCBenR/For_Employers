package com.company;

import java.io.*;
import java.net.InetAddress;
import java.net.Socket;

public class ClientSocketMain {

    //colors useful for color coding console messages
    public static final String ANSI_RESET = "\u001B[0m";
    public static final String ANSI_RED = "\u001B[31m";
    public static final String ANSI_GREEN = "\u001B[32m";
    public static final String ANSI_YELLOW = "\u001B[33m";


    Socket clientSocket;
    ObjectOutputStream out;
    ObjectInputStream in;
    InetAddress ip = InetAddress.getLocalHost();
    int port;

    /**
     * constructor
     * @param port port number to run on. must match server
     * @throws IOException
     */
    ClientSocketMain(int port) throws IOException {
        this.port = port;
    }

    /**
     * starts connection with server
     * @throws IOException
     */
    public void startConnection() throws IOException {
        System.out.println(ANSI_YELLOW + "connecting socket");
        clientSocket = new Socket(ip, port);
        System.out.println(ANSI_GREEN +"connected socket" + ANSI_RESET);
    }

    /**
     * ends connection with server and closes all in/out streams
     * @throws IOException
     */
    public void stopConnection() throws IOException {
        in.close();
        out.close();
        clientSocket.close();
    }

}
