package com.company;

import javax.crypto.*;
import javax.crypto.spec.SecretKeySpec;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.math.BigInteger;
import java.nio.charset.StandardCharsets;
import java.security.*;
import java.security.cert.CertificateException;
import java.security.cert.X509Certificate;
import java.security.spec.InvalidKeySpecException;
import java.util.Arrays;

public class Client {

    String fileNamePrivate = "src/com/company/clientPrivateKey.der";
    String fileNamePublic = "src/com/company/CASignedClientCertificate.der";

    //colors useful for color coding console messages
    public static final String ANSI_RESET = "\u001B[0m";
    public static final String ANSI_GREEN = "\u001B[32m";
    public static final String ANSI_CYAN = "\u001B[36m";

    Handshake clientHandshake;
    PublicKey RSApublicKey;
    BigInteger DHPK;
    PrivateKey RSAprivateKey;
    BigInteger signedKey;
    Boolean verify;
    BigInteger sharedSecret;

    BigInteger genSecret(BigInteger dhpk){
        BigInteger privKey = new BigInteger(RSAprivateKey.getEncoded());
        return dhpk.modPow(privKey, clientHandshake.N);
    }


    public Client() throws IOException, InvalidKeySpecException, NoSuchAlgorithmException, SignatureException, InvalidKeyException, CertificateException {

        clientHandshake = new Handshake();
        RSApublicKey = clientHandshake.getPublicKey(fileNamePublic);
        DHPK = clientHandshake.DHpubKey(fileNamePrivate);
        RSAprivateKey = clientHandshake.getPrivKey(fileNamePrivate);
        signedKey = clientHandshake.signKey(RSAprivateKey, DHPK);
        verify = clientHandshake.verifySig(RSApublicKey, DHPK, signedKey);
        sharedSecret = null;
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


    public static void main(String [] args) throws IOException, ClassNotFoundException, NoSuchAlgorithmException, InvalidKeyException, SignatureException, CertificateException, InvalidKeySpecException, NoSuchPaddingException, BadPaddingException, IllegalBlockSizeException, InvalidAlgorithmParameterException {
        //set up client, server, handshake, socket and in/out streams
        Client myClient = new Client();
        ClientSocketMain cs = new ClientSocketMain(5000);
        cs.startConnection();
        cs.out = new ObjectOutputStream(cs.clientSocket.getOutputStream());
        cs.in = new ObjectInputStream(cs.clientSocket.getInputStream());
        Handshake clientHandshake = new Handshake();

        //generate nonce
        byte [] nonce = clientHandshake.getNonce1();

        //send nonce
        cs.out.writeObject(nonce);

        //receive server
        X509Certificate serverCert = (X509Certificate) cs.in.readObject();
        BigInteger serverDHPK = (BigInteger) cs.in.readObject();
        BigInteger serverSigned = (BigInteger) cs.in.readObject();
        System.out.println(ANSI_GREEN + "got SERVER data" + ANSI_RESET);

        //get server public key
        PublicKey serverPK = serverCert.getPublicKey();

        //verify server
        Boolean verifyServer = clientHandshake.verifySig(serverPK, serverDHPK, serverSigned);
        System.out.println(ANSI_CYAN + "---------SERVER verification status---------");
        System.out.println(ANSI_GREEN + verifyServer);

        //generate creds
        ClientCreds clientCreds = new ClientCreds(myClient.DHPK, myClient.signedKey);
        if (verifyServer) {
            //send creds to server
            try {
                cs.out.writeObject(clientCreds.certificate);
                cs.out.writeObject(clientCreds.clientDhpk);
                cs.out.writeObject(clientCreds.clientSigned);
            } catch (Error b) {
                System.err.println(b);
            }
        } else {
            System.err.println("server certificate was not verified");
            cs.stopConnection();
        }

        //compute shared secret
        myClient.sharedSecret = myClient.genSecret(serverDHPK);

        //compute 6 keys
        HKDF clientSix = new HKDF();
        clientSix.makeSecretKeys(nonce, myClient.sharedSecret);

        //wait for server MAC
        byte[] serverMacBA = (byte[]) cs.in.readObject();

        //send MAC to server
        byte [] serverCertBA = serverCert.getEncoded();
        byte [] serverDHPKBA = serverDHPK.toByteArray();
        byte [] serverSignedBA = serverSigned.toByteArray();
        byte [] clientCertBA = clientCreds.certificate.getEncoded();
        byte [] clientDHPKBA = clientCreds.clientDhpk.toByteArray();
        byte [] clientSignedBA = clientCreds.clientSigned.toByteArray();
        ByteArrayOutputStream clientBAOS = new ByteArrayOutputStream();
        clientBAOS.write(nonce);
        clientBAOS.write(serverCertBA);
        clientBAOS.write(serverDHPKBA);
        clientBAOS.write(serverSignedBA);
        clientBAOS.write(clientCertBA);
        clientBAOS.write(clientDHPKBA);
        clientBAOS.write(clientSignedBA);
        byte [] clientMasterBA = clientBAOS.toByteArray();

        //compare with server version
        Mac clientServerCompare = Mac.getInstance("HmacSHA256");
        clientServerCompare.init(clientSix.serverMACgen);
        byte [] serverToClientMacBA = clientServerCompare.doFinal(clientMasterBA);

        if (Arrays.equals(serverMacBA, serverToClientMacBA)){
            System.out.println(ANSI_GREEN + "successful MAC compare server to client" + ANSI_RESET);
        }
        else {
            System.err.println("Non matching macs server to client");
            cs.stopConnection();
        }

        //create byte [] for full set
        ByteArrayOutputStream finalBAOS = new ByteArrayOutputStream();
        finalBAOS.write(serverMacBA);
        finalBAOS.write(clientMasterBA);

        byte [] clientPlusServerBA = finalBAOS.toByteArray();

        //create MAC instance, init with key from serverSix, pass it the server MAC'd byte array.
        Mac clientMAC = Mac.getInstance("HmacSHA256");
        clientMAC.init(clientSix.clientMACgen);
        byte [] clientMacBA = clientMAC.doFinal(clientPlusServerBA);
        cs.out.writeObject(clientMacBA);

        //receive message
        byte [] serverEncryptMessage = (byte[]) cs.in.readObject();
        //byte [] serverEncryptHmac = (byte[]) cs.in.readObject();

        //decrypt
        Cipher clientDecryptCipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
        clientDecryptCipher.init(Cipher.DECRYPT_MODE, clientSix.serverEncryptKey, clientSix.serverIVspec);
        byte[] clientDecryptMessage = clientDecryptCipher.doFinal(serverEncryptMessage);

        //loop through to get pieces
        byte[] serverMessage = new byte[clientDecryptMessage.length-32], serverHmac = new byte[32];

        if (clientDecryptMessage.length - 32 >= 0)
            System.arraycopy(clientDecryptMessage, 0, serverMessage, 0, clientDecryptMessage.length - 32);
        if (clientDecryptMessage.length - (clientDecryptMessage.length - 32) >= 0)
            System.arraycopy(clientDecryptMessage, clientDecryptMessage.length - 32, serverHmac, 0, clientDecryptMessage.length - (clientDecryptMessage.length - 32));

        //get plain text
        String plainText = new String(serverMessage, StandardCharsets.UTF_8);

        //HMAC plain text to compare
        byte [] serverFileMac = getHmac(clientSix.serverMAC, plainText.getBytes(StandardCharsets.UTF_8));

        if (Arrays.equals(serverFileMac, serverHmac)){
            System.out.println(ANSI_GREEN + "HMAC match" + ANSI_RESET);
            //success message
            String success = "File received successfully";

            //get HMAC of success message
            byte [] clientSuccessHmac = getHmac(clientSix.clientMAC, success.getBytes(StandardCharsets.UTF_8));
            Cipher clientEncryptCipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
            clientEncryptCipher.init(Cipher.ENCRYPT_MODE, clientSix.clientEncryptKey, clientSix.clientIVspec);
            ByteArrayOutputStream messageBAOS = new ByteArrayOutputStream();
            messageBAOS.write(success.getBytes(StandardCharsets.UTF_8));
            messageBAOS.write(clientSuccessHmac);
            byte [] fullMessageBA = messageBAOS.toByteArray();
            byte [] encryptedClientBA = clientEncryptCipher.doFinal(fullMessageBA);
            cs.out.writeObject(encryptedClientBA);

        }
        else
            System.err.println("HMAC do not match");


        cs.stopConnection();
    }


}

