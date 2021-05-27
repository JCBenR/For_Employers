#pragma once

template <typename T>
class SerialQueue{
private:
  struct Node{
	T data;
	Node* next;
  };

  Node* head;
  Node* tail;
  int size_;

public:
  SerialQueue()
	:head(new Node{T{}, nullptr}), size_(0)
  {
	tail = head;
  }


  void enqueue(const T& x){
      Node *newNode = new Node();
      newNode->data = x;
      tail->next = newNode;
      tail = newNode;

      if (size_ == 0)
          head = newNode;

      size_++;
  }

  bool dequeue(T* ret){
      if (size_ == 0)
        return false;
      else{
          if (size_ == 1){
              head = head->next;
              *ret = -1;
              size_--;
              return true;
          }
        head = head->next;
        *ret = head->data;
        size_--;
        return true;
      }
  }

  ~SerialQueue(){

	while(head){
	  Node* temp = head->next;
	  delete head;
	  head = temp;
	}
  }

  int size() const{
      return size_;
  }
};
