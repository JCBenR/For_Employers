package com.company;

import java.util.ArrayList;
import java.util.Arrays;

import static org.junit.jupiter.api.Assertions.*;

class BinarySearchTreeTest {

    BinarySearchTree<String> stringTree = new BinarySearchTree<>();
    BinarySearchTree<Integer> intTree = new BinarySearchTree<>();
    @org.junit.jupiter.api.BeforeEach
    void setUp() {
        stringTree.add("purple");
        stringTree.add("monkey");
        stringTree.add("dishwasher");

        intTree.add(1);
        intTree.add(42);
        intTree.add(77);
    }

    @org.junit.jupiter.api.AfterEach
    void tearDown() {
    }

    @org.junit.jupiter.api.Test
    void add() {
        stringTree.add("homer");
        assertTrue(stringTree.contains("homer"));
        intTree.add(55);
        assertTrue(intTree.contains(55));
    }

    @org.junit.jupiter.api.Test
    void addAll() {
        ArrayList<String> testList2 = new ArrayList<>(Arrays.asList("spade", "diamond", "hi", "hearts", "clubs", "zoo", "abbie"));
        stringTree.addAll(testList2);
        assertTrue(stringTree.containsAll(testList2));

        ArrayList<Integer> intList = new ArrayList<>(Arrays.asList(22,66,901,801,701));
        intTree.addAll(intList);
        assertTrue(intList.containsAll(intList));
    }

    @org.junit.jupiter.api.Test
    void clear() {
        stringTree.clear();
        intTree.clear();
        assertEquals(0,stringTree.size());
        assertEquals(0,intTree.size());
    }

    @org.junit.jupiter.api.Test
    void contains() {
        assertTrue(stringTree.contains("purple"));
        assertTrue(intTree.contains(42));
    }

    @org.junit.jupiter.api.Test
    void containsAll() {
        ArrayList<String> testList2 = new ArrayList<>(Arrays.asList("spade", "diamond", "hi", "hearts", "clubs", "zoo", "abbie"));
        stringTree.addAll(testList2);
        assertTrue(stringTree.containsAll(testList2));

        ArrayList<Integer> intList = new ArrayList<>(Arrays.asList(22,66,901,801,701));
        intTree.addAll(intList);
        assertTrue(intList.containsAll(intList));
    }

    @org.junit.jupiter.api.Test
    void first() {
        assertEquals("dishwasher", stringTree.first());
        assertEquals(1,intTree.first());
    }

    @org.junit.jupiter.api.Test
    void isEmpty() {
        assertFalse(stringTree.isEmpty());
        stringTree.clear();
        assertTrue(stringTree.isEmpty());
    }

    @org.junit.jupiter.api.Test
    void last() {
        stringTree.add("zebra");
        assertEquals("zebra", stringTree.last());
        intTree.add(33333);
        assertEquals(33333, intTree.last());
    }

    @org.junit.jupiter.api.Test
    void remove() {
        assertTrue(stringTree.contains("dishwasher"));
        stringTree.remove("dishwasher");
        assertFalse(stringTree.contains("dishwasher"));
        assertTrue(intTree.contains(42));
        intTree.remove(42);
        assertFalse(intTree.contains(42));

        BinarySearchTree<String> newTree = new BinarySearchTree<>();
        newTree.add("happy");
        newTree.add("sad");
        newTree.add("yak");
        newTree.add("radish");
        newTree.remove("happy");
        ArrayList<String> stringTreeList = newTree.toArrayList();
        System.out.println(newTree.getRoot());
        for (String s :
                stringTreeList) {
            System.out.println(s);
        }
    }

    @org.junit.jupiter.api.Test
    void removeAll() {
        ArrayList<String> testList2 = new ArrayList<>(Arrays.asList("spade", "diamond", "hi", "hearts", "clubs", "zoo", "abbie"));
        stringTree.addAll(testList2);
        stringTree.removeAll(testList2);
        System.out.println(stringTree.size());

        ArrayList<Integer> intList = new ArrayList<>(Arrays.asList(22,66,901,801,701));
        intTree.addAll(intList);
        assertTrue(intList.removeAll(intList));
        ArrayList<String> stringTreeList = stringTree.toArrayList();
//        System.out.println(stringTree.getRoot());
        for (String s :
                stringTreeList) {
            System.out.println(s);
        }
    }

    @org.junit.jupiter.api.Test
    void size() {
        assertEquals(3,stringTree.size());
        assertEquals(3,intTree.size());
    }

    @org.junit.jupiter.api.Test
    void toArrayList() {
        ArrayList<String> testList2 = new ArrayList<>(Arrays.asList("spade", "diamond", "hi", "hearts", "clubs", "zoo", "abbie"));
        stringTree.addAll(testList2);
        ArrayList<String> myStringList = stringTree.toArrayList();
        System.out.println("=======");
        for (String s :
                myStringList) {
            System.out.println(s);
        }

        ArrayList<Integer> intList = new ArrayList<>(Arrays.asList(22,66,901,801,701));
        intTree.addAll(intList);
        ArrayList<Integer> myIntList = intTree.toArrayList();
        System.out.println("=======");
        for (Integer s :
                myIntList) {
            System.out.println(s);
        }
    }

    @org.junit.jupiter.api.Test
    void getRoot() {
        assertEquals("purple", stringTree.getRoot());
        assertEquals(1, intTree.getRoot());
    }
}