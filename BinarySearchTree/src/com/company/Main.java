package com.company;

import java.util.ArrayList;
import java.util.Arrays;

import static org.junit.jupiter.api.Assertions.assertTrue;

public class Main {

    public static void main(String[] args) {
	BinarySearchTree<String> dictionary = new BinarySearchTree<>();

	dictionary.add("hello");
	dictionary.add("world");
        System.out.println(dictionary.getRoot());
        System.out.println(dictionary.contains("world"));
        ArrayList<String> testList = new ArrayList<>(Arrays.asList("hello", "world"));
        System.out.println(dictionary.containsAll(testList));
        ArrayList<String> testList2 = new ArrayList<>(Arrays.asList("spade", "diamond", "hi", "hearts", "Clubs", "zoo", "Abbie"));
        System.out.println(dictionary.addAll(testList2));
        System.out.println(dictionary.containsAll(testList));
        System.out.println(dictionary.first());
        System.out.println(dictionary.last());
//        ArrayList<String> removeList = new ArrayList<>(Arrays.asList("spade", "hi"));
        System.out.println(dictionary.remove("hello"));
        System.out.println(dictionary.remove("hi"));
//        System.out.println(dictionary.removeAll(removeList));
        dictionary.writeDot("firstFile.dot");
        ArrayList<String> myStringList = dictionary.toArrayList();
        System.out.println("=======");
        for (String s :
                myStringList) {
            System.out.println(s);
        }
    }
}
