package com.company;

import java.util.HashMap;

public class DNSCache {

    HashMap<DNSQuestion, DNSRecord> cache = new HashMap<>();

    /**
     * method to search cache for question from original request.
     * @param question question from request
     * @return valid answer if found, otherwise NULL
     */
    DNSRecord findCacheRecord(DNSQuestion question){
        if (cache.containsKey(question) && cache.get(question).timestampValid()){
            return cache.get(question);
        }
        else if (cache.containsKey(question) &! cache.get(question).timestampValid()){
                cache.remove(question);
                return null;
            }
        else return null;
    }

    /**
     * method to enter a result into the cache after it's been received by google
     * @param message received message from google
     */
    void enterCacheResult(DNSMessage message){
        cache.put(message.questions.get(0), message.answerRecords.get(0));
    }
}
