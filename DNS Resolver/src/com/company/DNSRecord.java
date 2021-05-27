package com.company;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Calendar;
import java.util.HashMap;

/**
 * @var createdDate: calendar date that the record is created
 * @var type, recordClass, ttl, rdLength, rdata, recordName: record variables
 */
public class DNSRecord {

    Calendar createdDate = Calendar.getInstance();
    byte [] type, recordClass, ttl, rdlength, rdata;
    String [] recordName;

    /**
     * this function decodes the record, pulling out the necessary flags based on their byte size.
     * @param inputStream: stream reading data
     * @param message: main message reading from
     * @return returns a DNSRecord
     * @throws IOException
     */
    static DNSRecord decodeRecord(InputStream inputStream, DNSMessage message) throws IOException {
        DNSRecord record = new DNSRecord();

        record.recordName = message.readDomainName(inputStream); //check this
        record.type = inputStream.readNBytes(2);
        record.recordClass = inputStream.readNBytes(2);
        record.ttl = inputStream.readNBytes(4);
        record.rdlength = inputStream.readNBytes(2);
        int rdLengthcount = record.getIntcout(record.rdlength);
        record.rdata = inputStream.readNBytes(rdLengthcount);
        return record;
    }

    /**
     * this function will write the bytes out to outputStream. will use writeDomainName to find if name exists. if so, it will use compression to send back pointer to where the domain already exists. if not it will write domain name. then writes all the flags.
     * @param outputStream: output stream to write to
     * @param domainLocations: hashMap of domain locations already written.
     * @throws IOException
     */
    void writeBytes(ByteArrayOutputStream outputStream, HashMap<String, Integer> domainLocations) throws IOException {
        DNSMessage.writeDomainName(outputStream,domainLocations,recordName);
        outputStream.write(type);
        outputStream.write(recordClass);
        outputStream.write(ttl);
        outputStream.write(rdlength);
        outputStream.write(rdata);
    }

    @Override
    public String toString() {
        return "DNSRecord{" +
                "type=" + Arrays.toString(type) +
                ", recordClass=" + Arrays.toString(recordClass) +
                ", ttl=" + Arrays.toString(ttl) +
                ", rdlength=" + Arrays.toString(rdlength) +
                ", recordName=" + Arrays.toString(recordName) +
                ", rdata=" + Arrays.toString(rdata) +
                '}';
    }

    /**
     * helper function that turns byte array in to int.
     * @param arr: byte array being used
     * @return: returns the int of the byte array
     */
    static int getIntcout(byte [] arr){
        return (arr[1] & 0xFF | arr[0] <<8);
    }

    /**
     * uses created date (when record was created) and compares to current date + ttl to see when the expiration should be. if expired, will return false, else will return true.
     * @return whether date is expired or not.
     */
    boolean timestampValid(){

        int recordTTL = ByteBuffer.wrap(ttl).getInt();
        Calendar currentTime = Calendar.getInstance();
        Calendar whenRecordWasCreated = createdDate;
        Calendar whenRecordWillExpire = Calendar.getInstance();

        System.out.println("currtime is: " + currentTime.getTimeInMillis());

        recordTTL = recordTTL * 1000;

        long offset = (whenRecordWasCreated.getTimeInMillis() + recordTTL);

        whenRecordWillExpire.setTimeInMillis(offset);

        System.out.println("record expires on: "+ whenRecordWillExpire.getTimeInMillis());

        return (currentTime.compareTo(whenRecordWillExpire) < 0);
    }
}
