//
// Created by Justin Siebenhaar on 3/12/21.
//

#ifndef MALLOCCLONE_MYMALLOC_H
#define MALLOCCLONE_MYMALLOC_H


#include <cstdlib>

struct tableEntry {
    bool active;
    void *ptr;
    size_t size;
    tableEntry(){
        this->active = false;
        this->ptr = nullptr;
        this->size = 0;
    };
};

class MyMalloc {
private:
    int count = 0;
    int capacity = 0;
public:
    explicit MyMalloc(size_t initSize);
    ~MyMalloc();
    tableEntry *startOfTable;
    void * allocate(size_t bytesToAllocate);
    void * deallocate(void* ptr);
    void * grow();
    void * insert(tableEntry &newEntry);
    void * remove(unsigned long hashedIdx);
    void printTable() const;
    int getCount() const;
    int getCapacity() const;
    int countCount() const;
};


#endif //MALLOCCLONE_MYMALLOC_H
