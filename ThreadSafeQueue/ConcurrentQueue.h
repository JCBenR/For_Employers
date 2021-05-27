#pragma once
#include <mutex>
#include <iostream>
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */

template <typename T>
class ConcurrentQueue{
private:
    struct Node{
        T data;
        Node* next;
    };

    Node* head;
    Node* tail;
    std::mutex head_m, tail_m;
    std::atomic<int> size_{};
    std::atomic<int> DQhits{};

public:
    ConcurrentQueue()
//            :head(new Node{T{}, nullptr}), size_(0)
    {
        Node *dummyNode = new Node();
        head = dummyNode;
        tail = dummyNode;
        //tail = head;
        size_ = 0;
        DQhits = 0;
    }


    void enqueue(const T& x){
//        std::cout << GREEN<< x << RESET<<std::endl;
        Node *newNode = new Node();
        newNode->data = x;
        newNode->next = nullptr;
        std::unique_lock<std::mutex> my_lock(tail_m);

        if (size() == 0) {
            std::unique_lock<std::mutex> my_head_lock(head_m);
            head = newNode;
        }

        tail->next = newNode;
        tail = newNode;
        size_++;
    }

    void enqueueNTimes(int howManyTimes){
        for (int i = 0; i < howManyTimes; ++i) {
            enqueue(i);
        }
    }

    bool dequeue(T* ret){
        //std::cout << RED << head->data << RESET << std::endl;

        std::unique_lock<std::mutex> my_lock(head_m);
        //cases to check:
        //queue is empty = return false
        //queue has one item left = dequeue item, ret = -1, head & tail = dummy node, size = 0;
        //queue is normal = dequeue head item, make next new head, return next->data.
        //queue is empty = return false
        if (size() <= 0) {
            DQhits++;
            return false;
        }

        //create new nodes for placeholding
        Node *temp = head; //old head
        Node *newHead = temp->next; //set new head

        if (newHead == NULL){ //check if new head has something after it, if not, it's the last one in the queue
            std::unique_lock<std::mutex> my_tail_lock(tail_m);
            *ret = -1;
            Node *dummy = new Node();
            head = dummy;
            tail = dummy;
            delete(temp);
            size_--;
            return true;
        }

        head = newHead; //move new head to head.
        *ret = head->data;

        delete(temp);
        size_--;
        return true;
    }

    void dequeueNTimes(int howManyTimes){
        int *myRet = nullptr;
        for (int i = 0; i < howManyTimes; ++i) {
            dequeue(myRet);
        }
    }

    ~ConcurrentQueue(){

        while(head){
            Node* temp = head->next;
            delete head;
            head = temp;
        }
    }

    int size() const{
        return size_;
    }

    int getDQhits() {
        return DQhits;
    }
};
