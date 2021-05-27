package com.company;

import java.io.FileInputStream;
import java.io.IOException;
import java.math.BigInteger;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.security.*;
import java.security.cert.CertificateException;
import java.security.cert.CertificateFactory;
import java.security.cert.X509Certificate;
import java.security.spec.InvalidKeySpecException;
import java.security.spec.PKCS8EncodedKeySpec;

public class Handshake {

    /**
     * large prime number for Diffie Hellman
     */
    String hexString = "FFFFFFFF FFFFFFFF C90FDAA2 2168C234 C4C6628B 80DC1CD1 29024E08 8A67CC74 020BBEA6 3B139B22 514A0879 8E3404DD EF9519B3 CD3A431B 302B0A6D F25F1437 4FE1356D 6D51C245 E485B576 625E7EC6 F44C42E9 A637ED6B 0BFF5CB6 F406B7ED EE386BFB 5A899FA5 AE9F2411 7C4B1FE6 49286651 ECE45B3D C2007CB8 A163BF05 98DA4836 1C55D39A 69163FA8 FD24CF5F 83655D23 DCA3AD96 1C62F356 208552BB 9ED52907 7096966D 670C354E 4ABC9804 F1746C08 CA18217C 32905E46 2E36CE3B E39E772C 180E8603 9B2783A2 EC07A28F B5C55DF0 6F4C52C9 DE2BCBF6 95581718 3995497C EA956AE5 15D22618 98FA0510 15728E5A 8AACAA68 FFFFFFFF FFFFFFFF";

    //strip off all spaces for conversion
    String hexStringNoSpace = hexString.replaceAll("\\s", "");

    /**
     * converts hex to bigInt. these are the common pieces of Diffie Hellman
     */
    BigInteger N = new BigInteger(hexStringNoSpace, 16);
    BigInteger G = new BigInteger("2");

    /**
     * retrieves the nonce which will be sent from the client to server on hello message
     * @return random 32 byte []
     */
    public byte[] getNonce1() {
        SecureRandom random = new SecureRandom();
        byte[] nonce1 = new byte[32];
        random.nextBytes(nonce1);
        return nonce1;
    }

    /**
     * calculates private key
     * @param fileName input file of privateKey. should be in .der format
     * @return PrivateKey configuration
     * @throws IOException
     * @throws NoSuchAlgorithmException
     * @throws InvalidKeySpecException
     */
    public PrivateKey getPrivKey(String fileName) throws IOException, NoSuchAlgorithmException, InvalidKeySpecException {
        Path pathToKey = Paths.get(fileName);
        PKCS8EncodedKeySpec pk = new PKCS8EncodedKeySpec(Files.readAllBytes(pathToKey));
        KeyFactory keyFactory = KeyFactory.getInstance("RSA");
        return keyFactory.generatePrivate(pk);
    }

    /**
     * same as above
     * @param fileName should be certificate from CA in .der format
     * @return
     * @throws IOException
     * @throws InvalidKeySpecException
     * @throws NoSuchAlgorithmException
     * @throws CertificateException
     */
    public PublicKey getPublicKey(String fileName) throws IOException, InvalidKeySpecException, NoSuchAlgorithmException, CertificateException {

        FileInputStream fileIn = new FileInputStream(fileName);
        CertificateFactory certificateFactory = CertificateFactory.getInstance("x.509");
        X509Certificate cert = (X509Certificate) certificateFactory.generateCertificate(fileIn);
        PublicKey publicKey = cert.getPublicKey();

        return publicKey;
    }

    /**
     * calculates DH public key
     * @param fileName private key file in .der format.
     * @return
     * @throws InvalidKeySpecException
     * @throws NoSuchAlgorithmException
     * @throws IOException
     */
    BigInteger DHpubKey(String fileName) throws InvalidKeySpecException, NoSuchAlgorithmException, IOException {
        //BigInteger pk = getPublicKey(fileName);
        PrivateKey pk = getPrivKey(fileName);
        BigInteger privKey = new BigInteger(pk.getEncoded());
        return G.modPow(privKey, N);
    }

    /**
     * signed version of DH public key. will be transferred in first message back from server, second message (after verification) from client.
     * @param rsaPrivate calculated RSA private key
     * @param DHPK DH Public Key (see above)
     * @return
     * @throws IOException
     * @throws InvalidKeySpecException
     * @throws NoSuchAlgorithmException
     * @throws InvalidKeyException
     * @throws SignatureException
     */
    BigInteger signKey(PrivateKey rsaPrivate, BigInteger DHPK) throws IOException, InvalidKeySpecException, NoSuchAlgorithmException, InvalidKeyException, SignatureException {

        KeyFactory kf = KeyFactory.getInstance("RSA");
        SecureRandom secureRandom = new SecureRandom();
        Signature signature = Signature.getInstance("SHA256WithRSA");
        signature.initSign(rsaPrivate, secureRandom);
        signature.update(DHPK.toByteArray());

        return new BigInteger(signature.sign());
    }

    /**
     * helper function that will verify the signature contained in the opening messages.
     * opening message will have certificate, DHPK and signed DHPK. from certificate, public key can be found, whcih can be used to verify signature and compare DHPK in signed DHPK
     * @param RSApublicKey
     * @param dhpk
     * @param signedKey
     * @return
     * @throws NoSuchAlgorithmException
     * @throws InvalidKeyException
     * @throws SignatureException
     */
    Boolean verifySig(PublicKey RSApublicKey, BigInteger dhpk, BigInteger signedKey) throws NoSuchAlgorithmException, InvalidKeyException, SignatureException {
        Signature signature = Signature.getInstance("SHA256WithRSA");
        signature.initVerify(RSApublicKey);
        signature.update(dhpk.toByteArray());

        return signature.verify(signedKey.toByteArray());
    }


}
