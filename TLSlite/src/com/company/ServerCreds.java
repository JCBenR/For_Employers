package com.company;

import java.io.FileInputStream;
import java.io.IOException;
import java.math.BigInteger;
import java.security.NoSuchAlgorithmException;
import java.security.PublicKey;
import java.security.cert.CertificateException;
import java.security.cert.CertificateFactory;
import java.security.cert.X509Certificate;
import java.security.spec.InvalidKeySpecException;

public class ServerCreds {
    X509Certificate certificate;
    BigInteger serverDhpk;
    BigInteger serverSigned;
    String fileNamePublic = "src/com/company/CASignedServerCertificate.der";


    public ServerCreds(BigInteger serverDhpk, BigInteger serverSigned) throws NoSuchAlgorithmException, CertificateException, InvalidKeySpecException, IOException {
        this.certificate = getCert(fileNamePublic);
        this.serverDhpk = serverDhpk;
        this.serverSigned = serverSigned;
    }

    public X509Certificate getCert(String fileName) throws IOException, InvalidKeySpecException, NoSuchAlgorithmException, CertificateException {

        FileInputStream fileIn = new FileInputStream(fileName);
        CertificateFactory certificateFactory = CertificateFactory.getInstance("x.509");
        return (X509Certificate) certificateFactory.generateCertificate(fileIn);
    }
}
