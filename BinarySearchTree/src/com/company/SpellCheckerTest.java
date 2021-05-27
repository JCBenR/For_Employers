package com.company;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import java.io.File;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import static org.junit.jupiter.api.Assertions.*;

class SpellCheckerTest {
    SpellChecker dict;
        File listOfWords = new File("jdsCheck.txt");
    @BeforeEach
    void setUp() {
        ArrayList<String> mountList= new ArrayList<>(Arrays.asList("mountain", "tops", "snow", "covered", "elephants", "turuila", "island", "sun", "south", "equator", "national"));
        dict = new SpellChecker(mountList);

    }

    @AfterEach
    void tearDown() {

    }

    @Test
    void addToDictionary() {
        List<String> checkList1 = dict.spellCheck(listOfWords);
        System.out.println("dictionary words mispelled or not contained:");
        System.out.println("=====================");
        for (String s :
                checkList1) {
            System.out.println(s);
        }
        dict.addToDictionary("purple");
        List<String> checkList = dict.spellCheck(listOfWords);
        System.out.println("dictionary words mispelled or not contained:");
        System.out.println("=====================");
        for (String s :
                checkList) {
            System.out.println(s);
        }
    }

    @Test
    void removeFromDictionary() {
        dict.removeFromDictionary("national");
        List<String> checkList = dict.spellCheck(listOfWords);
        System.out.println("dictionary words mispelled or not contained:");
        System.out.println("=====================");
        for (String s :
                checkList) {
            System.out.println(s);
        }
    }

    @Test
    void spellCheck() {

        List<String> checkList = dict.spellCheck(listOfWords);
        System.out.println("dictionary words mispelled or not contained:");
        System.out.println("=====================");
        for (String s :
                checkList) {
            System.out.println(s);
        }

    }
}