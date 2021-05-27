package com.company;

import javax.crypto.*;
import javax.crypto.spec.SecretKeySpec;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.math.BigInteger;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.security.*;
import java.security.cert.CertificateException;
import java.security.cert.X509Certificate;
import java.security.spec.InvalidKeySpecException;
import java.util.Arrays;

public class Server {
    Handshake serverHandshake;
    public PublicKey RSApublicKey;
    public BigInteger DHPK;
    public PrivateKey RSAprivateKey;
    public BigInteger signedKey;
    Boolean verify;
    BigInteger sharedSecret;

    //colors useful for color coding console messages
    public static final String ANSI_RESET = "\u001B[0m";
    public static final String ANSI_RED = "\u001B[31m";
    public static final String ANSI_GREEN = "\u001B[32m";
    public static final String ANSI_YELLOW = "\u001B[33m";
    public static final String ANSI_BLUE = "\u001B[34m";

    private final String fileNamePrivate = "src/com/company/serverPrivateKey.der";
    private final String fileNamePublic = "src/com/company/CASignedServerCertificate.der";
    private final String MonteCristo = "src/com/company/CountMonteCristo.txt";



    public Server() throws InvalidKeySpecException, NoSuchAlgorithmException, IOException, SignatureException, InvalidKeyException, CertificateException {
        serverHandshake = new Handshake();

        RSApublicKey = serverHandshake.getPublicKey(fileNamePublic);
        DHPK = serverHandshake.DHpubKey(fileNamePrivate);
        RSAprivateKey = serverHandshake.getPrivKey(fileNamePrivate);
        signedKey = serverHandshake.signKey(RSAprivateKey, DHPK);
        verify = serverHandshake.verifySig(RSApublicKey, DHPK, signedKey);
    };

    /**
     * helper function to generate secret key from DHPK
     * @param dhpk diffie Hellman public key
     * @return bigInt secret key
     */
    BigInteger genSecret(BigInteger dhpk){
        BigInteger privKey = new BigInteger(RSAprivateKey.getEncoded());

        return dhpk.modPow(privKey, serverHandshake.N);
    }

    /**
     * helper function go generate HMAC of message. the HMAC of the plain text string can be regenerated on the other side when the message is decrypted. the client/server have derived the same keys, IV, MAC Keys, etc. so they can reproduce this all for everything. this allows the verification that what was received is what was intended to be sent and was not altered by man-in-the-middle.
     * @param secretKey should be the same HMAC key for clientSix/serverSix as message being received or sent.
     * @param message byte [] of plain text message
     * @return HMAC in byte array
     */
    static public byte[] getHmac(byte[] secretKey, byte[] message) {
        byte[] hmacSha256 = null;
        try {
            Mac mac = Mac.getInstance("HmacSHA256");
            SecretKeySpec secretKeySpec = new SecretKeySpec(secretKey, "HmacSHA256");
            mac.init(secretKeySpec);
            hmacSha256 = mac.doFinal(message);
        } catch (Exception e) {
            throw new RuntimeException("Failed to calculate hmac-sha256", e);
        }
        return hmacSha256;
    }

    public static void main(String [] args) throws IOException, CertificateException, NoSuchAlgorithmException, InvalidKeySpecException, SignatureException, InvalidKeyException, ClassNotFoundException, NoSuchPaddingException, BadPaddingException, IllegalBlockSizeException, InvalidAlgorithmParameterException {
        //create server, handshake and serverCreds
        Server myServer = new Server();
        Handshake serverHandshake = new Handshake();
        ServerCreds serverCreds = new ServerCreds(myServer.DHPK, myServer.signedKey);

        //assign port
        int port = 5000;
        ServerSocket serverSocket = new ServerSocket(port);//make port a variable
            System.out.println(ANSI_GREEN + "server listening on port: " + port + ANSI_RESET);
        Socket connection = serverSocket.accept();
            System.out.println(ANSI_GREEN+ "connection received");

        //create in/out streams
        ObjectOutputStream oos = new ObjectOutputStream(connection.getOutputStream());
        ObjectInputStream ois = new ObjectInputStream(connection.getInputStream());

        //server receives nonce, which begins process
        byte[] nonce = (byte[]) ois.readObject();

        //server writes out parts of opening response (certificate, DHPK, signed DHPK)
        System.out.println(ANSI_YELLOW + "writing cert");
        try{
            oos.writeObject(serverCreds.certificate);
            oos.writeObject(serverCreds.serverDhpk);
            oos.writeObject(serverCreds.serverSigned);
        } catch (Exception e){
            System.err.println(e);
        }

        //server receives the same as above from client
        X509Certificate clientCert = (X509Certificate) ois.readObject();
        BigInteger clientDHPK = (BigInteger) ois.readObject();
        BigInteger clientSigned = (BigInteger) ois.readObject();
        System.out.println(ANSI_YELLOW+ "got CLIENT data" + ANSI_RESET);

        //get Client public key
        PublicKey clientPK = clientCert.getPublicKey();

        //verify Client
        Boolean verifyClient = serverHandshake.verifySig(clientPK, clientDHPK, clientSigned);
        System.out.println(ANSI_BLUE+ "---------CLIENT verification status---------");
        System.out.println(ANSI_GREEN + verifyClient + ANSI_RESET);

        if (!verifyClient){
            System.err.println(ANSI_RED+ "could not verify client");
            connection.close();
        }

        //get shared secret
        myServer.sharedSecret = myServer.genSecret(clientDHPK);

        //compute 6 keys
        HKDF serverSix = new HKDF();
        serverSix.makeSecretKeys(nonce, myServer.sharedSecret);

        //send MAC to client
        //get byte[] for everything so far
        //this will allow the individual byte arrays to be passed to a byteArrayOutputStream
        byte [] serverCert = serverCreds.certificate.getEncoded();
        byte [] serverDHPK = serverCreds.serverDhpk.toByteArray();
        byte [] serverSigned = serverCreds.serverSigned.toByteArray();
        byte [] clientCertBA = clientCert.getEncoded();
        byte [] clientDHPKBA = clientDHPK.toByteArray();
        byte [] clientSignedBA = clientSigned.toByteArray();
        ByteArrayOutputStream serverBAOS = new ByteArrayOutputStream();
        serverBAOS.write(nonce);
        serverBAOS.write(serverCert);
        serverBAOS.write(serverDHPK);
        serverBAOS.write(serverSigned);
        serverBAOS.write(clientCertBA);
        serverBAOS.write(clientDHPKBA);
        serverBAOS.write(clientSignedBA);
        byte [] serverMasterBA = serverBAOS.toByteArray();

        //create MAC instance, init with key from serverSix, pass it the server MAC'd byte array.
        Mac serverMac = Mac.getInstance("HmacSHA256");
        serverMac.init(serverSix.serverMACgen);
        byte [] serverMacBA = serverMac.doFinal(serverMasterBA);
        oos.writeObject(serverMacBA);

        //wait for client MAC
        byte [] clientMacBA = (byte[]) ois.readObject();

        //generate comparison array
        ByteArrayOutputStream finalBAOS = new ByteArrayOutputStream();
        finalBAOS.write(serverMacBA);
        finalBAOS.write(serverMasterBA);
        byte [] clientPlusServerBA = finalBAOS.toByteArray();

        //compare MAC messages
        Mac serverClientCompare = Mac.getInstance("HmacSHA256");
        serverClientCompare.init(serverSix.clientMACgen);
        byte [] serverToClientMacBA = serverClientCompare.doFinal(clientPlusServerBA);

        if (Arrays.equals(clientMacBA, serverToClientMacBA)){
            System.out.println(ANSI_GREEN + "successful MAC compare server to client");
        }
        else {
            System.err.println("Non matching macs server to client");
            connection.close();
        }

        String testMessage = "A wonderful serenity has taken possession of my entire soul, like these sweet mornings of spring which I enjoy with my whole heart. I am alone, and feel the charm of existence in this spot, which was created for the bliss of souls like mine. I am so happy, my dear friend, so absorbed in the exquisite sense of mere tranquil existence, that I neglect my talents. I should be incapable of drawing a single stroke at the present moment; and yet I feel that I never was a greater artist than now. When, while the lovely valley teems with vapour around me, and the meridian sun strikes the upper surface of the impenetrable foliage of my trees, and but a few stray gleams steal into the inner sanctuary, I throw myself down among the tall grass by the trickling stream; and, as I lie close to";

        Path filePath = Paths.get("src/com/company/CountMonteCristo.txt");
        byte [] CountCristo = Files.readAllBytes(filePath);

        //HMAC string
        //byte [] serverFileMac = getHmac(serverSix.serverMAC, testMessage.getBytes(StandardCharsets.UTF_8));
        byte [] serverFileMac = getHmac(serverSix.serverMAC, CountCristo);
        //create cipher
        Cipher serverEncryptCipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
        serverEncryptCipher.init(Cipher.ENCRYPT_MODE, serverSix.serverEncryptKey, serverSix.serverIVspec);

        //create byte [] of message + HMAC
        ByteArrayOutputStream messageBAOS = new ByteArrayOutputStream();
        messageBAOS.write(CountCristo);
        messageBAOS.write(serverFileMac);
        byte [] fullMessageBA = messageBAOS.toByteArray();
        byte [] encryptedServerBA = serverEncryptCipher.doFinal(fullMessageBA);
        oos.writeObject(encryptedServerBA);

        //receive client message
        byte[] clientResponse = (byte[]) ois.readObject();

        //decrypt
        Cipher serverDecryptCipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
        serverDecryptCipher.init(Cipher.DECRYPT_MODE, serverSix.clientEncryptKey, serverSix.clientIVspec);
        byte[] serverDecryptMessage = serverDecryptCipher.doFinal(clientResponse);

        //loop through to get pieces
        byte[] clientMessage = new byte[serverDecryptMessage.length-32], clientHmac = new byte[32];

        if (serverDecryptMessage.length - 32 >= 0)
            System.arraycopy(serverDecryptMessage, 0, clientMessage, 0, serverDecryptMessage.length - 32);
        if (serverDecryptMessage.length - (serverDecryptMessage.length - 32) >= 0)
            System.arraycopy(serverDecryptMessage, serverDecryptMessage.length - 32, clientHmac, 0, serverDecryptMessage.length - (serverDecryptMessage.length - 32));

        //get plain text
        String plainText = new String(clientMessage, StandardCharsets.UTF_8);

        //HMAC plain text to compare
        byte [] clientFileMac = getHmac(serverSix.clientMAC, plainText.getBytes(StandardCharsets.UTF_8));

        if (Arrays.equals(clientFileMac, clientHmac)) {
            System.out.println(ANSI_GREEN + plainText + ANSI_RESET);
        } else {
            System.err.println("file not received successfully");
        }
        oos.close();
        ois.close();
        connection.close();
    }

}
