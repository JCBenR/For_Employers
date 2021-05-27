package com.company;

import java.io.IOException;
import java.math.BigInteger;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.security.PublicKey;
import java.security.SignatureException;
import java.security.cert.CertificateException;
import java.security.spec.InvalidKeySpecException;
import java.util.Arrays;

public class Main {

    public static void main(String[] args) throws NoSuchAlgorithmException, SignatureException, InvalidKeySpecException, InvalidKeyException, IOException, CertificateException {


        Server server = new Server();
        Client client = new Client();
        PublicKey serverCert = server.RSApublicKey;
        BigInteger chPubKey = server.DHPK;
        BigInteger signed = server.signedKey;
        System.out.println("---------PUBLIC KEY---------");
        System.out.println(serverCert);
        System.out.println("---------DH PUBLIC KEY---------");
        System.out.println(chPubKey);
        System.out.println("---------DH SIGNED PUBLIC KEY---------");
        System.out.println(signed);
        System.out.println("------------VERIFY-------------");
        System.out.println(server.verify);

        BigInteger clientPubKey = client.DHPK;
        BigInteger clientSS = client.genSecret(chPubKey);
        BigInteger serverSS = server.genSecret(clientPubKey);

        System.out.println("----------------CLIENT------------------");
        System.out.println(clientSS);
        System.out.println("------------------SERVER----------------");
        System.out.println(serverSS);

        HKDF hkdf = new HKDF();
        System.out.println(Arrays.toString(hkdf.hdkfExpand(serverSS.toByteArray(), "client")));

        hkdf.makeSecretKeys(client.clientHandshake.getNonce1(), serverSS );

        System.out.println(Arrays.toString(hkdf.serverEncrypt));
        System.out.println(Arrays.toString(hkdf.clientEncrypt));
        System.out.println(Arrays.toString(hkdf.serverMAC));
        System.out.println(Arrays.toString(hkdf.clientMAC));
        System.out.println(Arrays.toString(hkdf.serverIV));
        System.out.println(Arrays.toString(hkdf.clientIV));


    }
}
