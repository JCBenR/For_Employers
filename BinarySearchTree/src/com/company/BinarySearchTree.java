package com.company;

import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Collection;
import java.util.NoSuchElementException;

public class BinarySearchTree<T extends Comparable<? super T>> implements SortedSet<T> {
    private Node root = null;
    private int size = 0;

    BinarySearchTree(){};

    class Node {
        T data;
        Node left, right;

        Node(T data) {
            this.data = data;
            left = null;
            right = null;
        }
    }

    /**
     * Ensures that this set contains the specified item.
     *
     * @param item - the item whose presence is ensured in this set
     * @return true if this set changed as a result of this method call (that is, if
     * the input item was actually inserted); otherwise, returns false
     * @throws NullPointerException if the item is null
     */
    @Override
    public boolean add(T item) {
        if (item == null)throw new NullPointerException();
        //cast item to string for handling uppercase items
        if (item instanceof String){
            String newItem = ((String) item).toLowerCase();
            item = (T)newItem;
        }
        Node newNode = new Node(item);
        //check to see if empty. if so, make node root.
        if (size < 1){
            root = newNode;
            size++;
            return true;
        }
        if (contains(item)) return false;

        Node x = root;
        Node y = null;
        while(x != null){
            y = x;
            if (item.compareTo(x.data) < 0){
                x = x.left;
            }
            else x = x.right;
        }

        if(newNode.data.compareTo(y.data)< 0){
            y.left = newNode;
            size++;
            return true;
        }
        else {
            y.right = newNode;
            size++;
            return true;
        }
    }

    /**
     * Ensures that this set contains all items in the specified collection.
     *
     * @param items - the collection of items whose presence is ensured in this set
     * @return true if this set changed as a result of this method call (that is, if
     * any item in the input collection was actually inserted); otherwise,
     * returns false
     * @throws NullPointerException if any of the items is null
     */
    @Override
    public boolean addAll(Collection<? extends T> items) {
        for (T item :
                items) {
            if(add(item) == false) return false;
        }
        return true;
    }

    /**
     * Removes all items from this set. The set will be empty after this method
     * call.
     */
    @Override
    public void clear() {
        size = 0;
        root = null;
    }

    /**
     * Determines if there is an item in this set that is equal to the specified
     * item.
     *
     * @param item - the item sought in this set
     * @return true if there is an item in this set that is equal to the input item;
     * otherwise, returns false
     * @throws NullPointerException if the item is null
     */
    @Override
    public boolean contains(T item) {
        if (item == null)throw new NullPointerException();
        Node x = root;
        if (size < 1) return false;
        while(x != null){
            if (item.compareTo(x.data)==0) return true;
            if (item.compareTo(x.data)<0){
                x = x.left;
            }
            else x = x.right;
        }
        return false;
    }

    /**
     * Determines if for each item in the specified collection, there is an item in
     * this set that is equal to it.
     *
     * @param items - the collection of items sought in this set
     * @return true if for each item in the specified collection, there is an item
     * in this set that is equal to it; otherwise, returns false
     * @throws NullPointerException if any of the items is null
     */
    @Override
    public boolean containsAll(Collection<? extends T> items) {
        for (T item :
                items) {
            if(!contains(item)) return false;
        }
        return true;
    }

    /**
     * Returns the first (i.e., smallest) item in this set.
     *
     * @throws NoSuchElementException if the set is empty
     */
    @Override
    public T first() throws NoSuchElementException {
        Node item = root;
        if (item == null)throw new NullPointerException();
        while(item.left != null) item = item.left;

        return item.data;
    }

    /**
     * Returns true if this set contains no items.
     */
    @Override
    public boolean isEmpty() {
        //this way of writing it returns true if size == 0. else returns false.
        return size == 0;
    }

    /**
     * Returns the last (i.e., largest) item in this set.
     *
     * @throws NoSuchElementException if the set is empty
     */
    @Override
    public T last() throws NoSuchElementException {
        Node item = root;
        while(item.right != null) item = item.right;

        return item.data;
    }

    /**
     * helper function to get item on left (not used because found a way to do remove recursively)
     * @param rootItem --where to start
     * @return --data of item
     */
    private T lastChild(Node rootItem){
        Node item = rootItem;
        while(item.right != null) item = item.right;

        return item.data;
    }

    /**
     * Ensures that this set does not contain the specified item.
     *
     * @param item - the item whose absence is ensured in this set
     * @return true if this set changed as a result of this method call (that is, if
     * the input item was actually removed); otherwise, returns false
     * @throws NullPointerException if the item is null
     */
    @Override
    public boolean remove(T item) {
        if (item == null)throw new NullPointerException();
        if (root == null) return false;
        if (remove(item, root) != null) return true;
        return false;
    }
    /**
     * helper function to find first child. initally concepted to help in finding the lowest item on the right side for remove, but not used because of recursive method implemented.
     * @param rootItem --starting root, updated on each call as travsersing down the tree
     * @return --data of item found.
     * @throws NoSuchElementException
     */
    private Node firstChild(Node rootItem) throws NoSuchElementException {
        Node item = rootItem;
        if (item == null)throw new NullPointerException();
        while(item.left != null) item = item.left;

        return item;
    }

    private Node firstChildParent(T itemToFind, Node rootNode) throws NoSuchElementException{
        Node item = rootNode;
        if (item == null)throw new NullPointerException();
        if (item.left != null && item.left.data.equals(itemToFind)) return item;
        else firstChildParent(itemToFind, item.left);
        return item;
    }
    /**
     * recursive method that takes from driver method. first determines if it should traverse left or right. when it finds the appropriate value, then checks whether that value has left, right or none branches. if so, it find the @method firstChild() to find the successor. replaces deleted item with correct successor. deletes successor from original spot. returns.
     * @param item --item to search for
     * @param nodeItem --represent root of subtree being searched.
     * @return --item being removed
     */
    public Node remove(T item, Node nodeItem){

        if (item.equals(root.data)){
            Node successor = (firstChild(nodeItem.right));
            Node parent = (firstChildParent(successor.data, nodeItem.right));
            successor.left = nodeItem.left;
            successor.right = nodeItem.right;
            root = successor;
            parent.left = null;
            size--;
            return root;
        }
        //if true item is on left side
        if (item.compareTo(nodeItem.data)<0){
            nodeItem.left = remove(item, nodeItem.left);
        }
        //if true item is on right side
        else if (item.compareTo(nodeItem.data)>0){
            nodeItem.right = remove(item, nodeItem.right);
        }
        //item is equal to item found and should be the item deleted
        else{
            //if has two branches
            if(nodeItem.left != null && nodeItem.right != null){
                //find successor
                Node successor = firstChild(nodeItem.right);
                //replace node to be deleted with successor
                nodeItem.data = successor.data;
                //call remove() on successor
                remove(successor.data, nodeItem.right);
            }
            //if has left branch
            else if (nodeItem.left != null){
                Node toDelete = nodeItem;
                nodeItem = nodeItem.left;
                toDelete = null;
            }
            //if has right branch
            else if(nodeItem.right != null){
                Node toDelete = nodeItem;
                nodeItem = nodeItem.right;
                toDelete = null;
            }
            //if has no branches and can be removed
//            if(size == 1) clear();
            else nodeItem = null;
        }
        return nodeItem;
    }

    /**
     * Ensures that this set does not contain any of the items in the specified
     * collection.
     *
     * @param items - the collection of items whose absence is ensured in this set
     * @return true if this set changed as a result of this method call (that is, if
     * any item in the input collection was actually removed); otherwise,
     * returns false
     * @throws NullPointerException if any of the items is null
     */
    @Override
    public boolean removeAll(Collection<? extends T> items) {
        for (T item :
                items) {
            if(!remove(item)) return false;
        }
        return true;
    }

    /**
     * Returns the number of items in this set.
     */
    @Override
    public int size() {
        return size;
    }

    /**
     * Returns an ArrayList containing all of the items in this set, in sorted
     * order.
     */
    @Override
    public ArrayList<T> toArrayList() {
        return createList(root);
    }

    /**
     * creates array list by traversing left side, recurssivly, then center, then right.
     * @param root --root where to begin. will be updated recursivly with the top of the tree until reaches null values.
     * @return --returns arrayList of values sorted, because it will return first left, then center, then right.
     */
    private ArrayList<T> createList(Node root){
        if (root == null) return new ArrayList<>();
        ArrayList<T> myArrayList = new ArrayList<>();
        myArrayList.addAll(createList(root.left));
        myArrayList.add(root.data);
        myArrayList.addAll(createList(root.right));
        return myArrayList;
    }

    /**
     *
     * @return --data held in root element
     */
    public T getRoot(){
        return root.data;
    }

    /**
     * // Driver method
     *     // Generates a .dot file representing this tree.
     *     // Use each node's hashCode to uniquely identify it
     * @param filename
     */
    public void writeDot(String filename) {
        try {
            PrintWriter output = new PrintWriter(new FileWriter(filename));
            output.println("graph g {");
            if (root != null)
                output.println(root.hashCode() + "[label=\"" + root.data + "\"]");
            writeDotRecursive(root, output);
            output.println("}");
            output.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    /**
     * Recursively traverse the tree, outputting each node to the .dot file
     *
     * @param n --starting node
     * @param output --file to write to
     * @throws Exception
     */
    private void writeDotRecursive(BinarySearchTree.Node n, PrintWriter output) throws Exception {
        if (n == null)
            return;
        if (n.left != null) {
            output.println(n.left.hashCode() + "[label=\"" + n.left.data + "\"]");
            output.println(n.hashCode() + " -- " + n.left.hashCode());
        }
        if (n.right != null) {
            output.println(n.right.hashCode() + "[label=\"" + n.right.data + "\"]");
            output.println(n.hashCode() + " -- " + n.right.hashCode());
        }

        writeDotRecursive(n.left, output);
        writeDotRecursive(n.right, output);
    }
}