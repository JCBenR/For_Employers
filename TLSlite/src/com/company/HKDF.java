package com.company;
import javax.crypto.*;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;
import java.math.BigInteger;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.util.Arrays;

public class HKDF {

    byte[] serverEncrypt, clientEncrypt, serverMAC, clientMAC, serverIV, clientIV;
    SecretKeySpec serverEncryptKey;
    SecretKeySpec clientEncryptKey;
    SecretKeySpec serverMACgen;
    SecretKeySpec clientMACgen;
    IvParameterSpec serverIVspec;
    IvParameterSpec clientIVspec;

    public HKDF() throws NoSuchAlgorithmException {
    }

    public byte[] hdkfExpand(byte[] input, String inputString) throws NoSuchAlgorithmException, InvalidKeyException {
        String inStr1 = inputString + 0x1;
        SecretKeySpec inputKey = new SecretKeySpec(input, "HmacSHA256");

        Mac hmac = Mac.getInstance("HmacSHA256");
        hmac.init(inputKey);
        byte[] okm = hmac.doFinal(inStr1.getBytes());
        return Arrays.copyOfRange(okm, 0, 16);
    }

    void makeSecretKeys(byte[] clientNonce, BigInteger sharedSecret) throws NoSuchAlgorithmException, InvalidKeyException {
        SecretKeySpec inputKey = new SecretKeySpec(clientNonce, "HmacSHA256");
        Mac hmac = Mac.getInstance("HmacSHA256");
        hmac.init(inputKey);
        //get byte arrays
        byte[] prk = hmac.doFinal(sharedSecret.toByteArray());
        serverEncrypt = hdkfExpand(prk, "server encrpyt");
        clientEncrypt = hdkfExpand(serverEncrypt, "client encrypt");
        serverMAC = hdkfExpand(clientEncrypt, "server MAC");
        clientMAC = hdkfExpand(serverMAC, "client MAC");
        serverIV = hdkfExpand(clientMAC, "server IV");
        clientIV = hdkfExpand(serverIV, "client IV");
        //set keys
        serverEncryptKey = new SecretKeySpec(serverEncrypt, "AES");
        clientEncryptKey = new SecretKeySpec(clientEncrypt, "AES");
        serverMACgen = new SecretKeySpec(serverMAC, "AES");
        clientMACgen = new SecretKeySpec(clientMAC, "AES");
        serverIVspec = new IvParameterSpec(serverIV);
        clientIVspec = new IvParameterSpec(clientIV);
    }
}