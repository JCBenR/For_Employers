package com.company;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.Serializable;
import java.util.Arrays;
import java.util.HashMap;

public class DNSQuestion implements Serializable {

    byte[]  qType, qClass, qNameBA;
    String[] qName;

    /**
     * decodes the question from original request, so we know what to ask for (searching our cache and asking google)
     * @param inputStream: input stream to read from
     * @param message: message to use readDomainName method on
     * @return DNSQuestion with fields properly extracted
     * @throws IOException
     */
    static DNSQuestion decodeQuestion(ByteArrayInputStream inputStream, DNSMessage message) throws IOException {

        DNSQuestion question = new DNSQuestion();
        question.qName = message.readDomainName(inputStream);
        question.qType = inputStream.readNBytes(2);
        question.qClass = inputStream.readNBytes(2);
        System.out.println(question.toString());
        return question;
    }

    /**
     * writes to the output stream the bytes of the question
     * @param outputStream: output stream to write to
     * @param domainNameLocations: hashmap of locations already written for reference in compression
     * @throws IOException
     */
    void writeBytes(ByteArrayOutputStream outputStream, HashMap<String,Integer> domainNameLocations) throws IOException {
        DNSMessage.writeDomainName(outputStream,domainNameLocations,qName);
        outputStream.write(qType);
        outputStream.write(qClass);
    }

    @Override
    public String toString() {
        return "DNSQuestion{" +
                "qName=" + Arrays.toString(qName) +
                ", qType=" + Arrays.toString(qType) +
                ", qClass=" + Arrays.toString(qClass) +
                '}';
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        DNSQuestion that = (DNSQuestion) o;
        return Arrays.equals(qName, that.qName) &&
                Arrays.equals(qType, that.qType) &&
                Arrays.equals(qClass, that.qClass);
    }

    @Override
    public int hashCode() {
        int result = Arrays.hashCode(qName);
        result = 31 * result + Arrays.hashCode(qType);
        result = 31 * result + Arrays.hashCode(qClass);
        return result;
    }
}
