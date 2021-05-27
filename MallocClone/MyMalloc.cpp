//
// Created by Justin Siebenhaar on 3/12/21.
//
#include <sys/mman.h>
#include <iostream>
#include "MyMalloc.h"


size_t getBytesToAllocate(size_t bytesToAllocate);

MyMalloc::MyMalloc(size_t initSize){
    //initialize the table
    startOfTable = reinterpret_cast<tableEntry*>(mmap(nullptr, initSize * sizeof(tableEntry), PROT_READ | PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0));
    capacity = initSize;
}

MyMalloc::~MyMalloc() {
    for (int i = 0; i < capacity; ++i) {
        if (startOfTable[i].active)
            deallocate(startOfTable[i].ptr);
    }
    munmap(startOfTable, capacity);
}


//create hash table
//create hashing function
//create method for adding to table (if necessary)
//create method for removing from table (tombstone or lazy delete)

void *MyMalloc::allocate(size_t bytesToAllocate) {

    float ratio = (float)count/(float)capacity;
    if (ratio >= 0.6) {
//        std::cout << "ratio is: " << ratio << std::endl;
        grow();
    }
    //determine how many more bytes to allocate (needs to be multiple of 4--round up)
    bytesToAllocate = getBytesToAllocate(bytesToAllocate);

    //use mmap to allocate size
    void* newMem = mmap(nullptr, bytesToAllocate, PROT_READ | PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1 , 0 );
    tableEntry newEntry;
    newEntry.active = true;
    newEntry.ptr = newMem;
    newEntry.size = bytesToAllocate;

    //send ptr and size to hash table
    insert(newEntry);

    return newMem;
}

/**
 * inserts new entry into table
 * @param newEntry
 * @return
 */
void *MyMalloc::insert(tableEntry &newEntry) {//k=newMem, v=BTA
    std::hash<void*> index;
    int hashedIdx = index(newEntry.ptr) % capacity;

    while (startOfTable[hashedIdx].active) { //if that spot is active, look at the next one.
        if (hashedIdx == capacity-1){ //check if at end
            hashedIdx = 0;
        }
        else hashedIdx++;
    }

    startOfTable[hashedIdx] = newEntry;
    count++;

    //printTable();
}

/**
 * deallocates memory of passed pointer. hashes ptr to find correct index. loops through the table to find the matching pointer, which will contain the size that should be removed. next, calls munmap on that index and size.
 * will then call remove to remove from the table.
 * @param ptr
 * @return null
 */
void *MyMalloc::deallocate(void *ptr) {
//    std::cout << "*****************************" <<std::endl;
//    std::cout << "deallocating entry " << ptr <<std::endl;
//    std::cout << "*****************************" <<std::endl;

    //retrieve struct from table
    std::hash<void*> index;
    unsigned long hashedIdx = index(ptr) % capacity;

    int indexForItem = 0;
    for (int i = hashedIdx; i < capacity; ++i) {
        if (startOfTable[i].ptr == ptr){
            indexForItem = i;
            munmap(startOfTable[i].ptr, startOfTable[i].size);
            break;
        }
    }
    //remove ptr from hash table

    remove(indexForItem);
    //printTable();
    return nullptr;
}

/**
 * helper function for deallocate that will remove entry from the table
 * @param hashedIdx index that should be removed
 * @return null
 */
void *MyMalloc::remove(unsigned long hashedIdx) {
    tableEntry empty;
    startOfTable[hashedIdx] = empty;
    count--;
    return nullptr;
}

/**
 * function to grow table. if table ever exceeds 60% capacity it should call this function.
 * function will double amount from previous capacity, rehash each object in old table, add it to the new table and the updated index. will replace old table with new. updates capacity to new size. finally removing the old table.
 *
 * @return void pointer to start of table after it's been updated to new index.
 */
void *MyMalloc::grow() {
//    std::cout << "*****************************" <<std::endl;
//    std::cout << "growing table" <<std::endl;
//    std::cout << "count is: " << count <<std::endl;
//    std::cout << "*****************************" <<std::endl;


    //create new table to allocate to
    tableEntry *largerTable = reinterpret_cast<tableEntry*>(mmap(nullptr, (capacity * 2) * sizeof (tableEntry), PROT_READ | PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0));

    //set newCapacity to be double. Useful for the if statement, linear probing and wrapping around back to zero below.
    int newCapacity = capacity * 2;
    //for each entry currently in the hash table, rehash it and insert
    for (int i = 0; i < capacity; i++) {
        if (startOfTable[i].active) { //if it's active hash and insert
            std::hash<void*> index;
            int hashedIdx = index(startOfTable[i].ptr) % (capacity * 2); //hash
            while (largerTable[hashedIdx].active) { //if that spot is active, look at the next one.
                if (hashedIdx == newCapacity-1){ //check if at end
                    hashedIdx = 0;
                }
                else hashedIdx++;
            }
            std::swap(startOfTable[i], largerTable[hashedIdx]); //insert into new table

        }
    }
    capacity *=2; //increase capacity

    //reassign previous touble
    tableEntry *oldTable = startOfTable;

    //reassign hash table to new hash table
    startOfTable = largerTable;

    //unmap old table
    munmap(oldTable, capacity * sizeof(tableEntry));

    //free old hash table (not sure on this one... do we need to reassign the old one to another place before freeing?
    return startOfTable;
}

/**
 * determines bytes to be allocated. will be at least one 4KB page size. if over 4KB, will round up to next page size (ie, 8KB).
 * @param bytesToAllocate how many bytes to allocate
 * @return size_t of bytes to allocate after math has been done.
 */
size_t getBytesToAllocate(size_t bytesToAllocate){
    if (bytesToAllocate % 4096 == 0){
        int difference = bytesToAllocate / 4096;
        bytesToAllocate = difference * 4096;
        bytesToAllocate += 4096;
    }
    return bytesToAllocate;
}

/**
 * helper function to print out table, whether entry space is active (has active item), index and hashed ptr of index.
 */
void MyMalloc::printTable() const{
    for (int i = 0; i < capacity; ++i) {
        if (startOfTable[i].active){
            std::cout << "Active entry\t\t" << i << "\t" <<startOfTable[i].ptr << std::endl;
        } else
            std::cout << "Not Active entry\t" <<  i << "\t" <<startOfTable[i].ptr << std::endl;
    }
    std::cout << "-----------------------------" <<std::endl;

}

/**
 * helper function to get total capacity of MyMalloc object
 * @return capacity of object
 */
int MyMalloc::getCapacity() const {
    return capacity;
}

/**
 * helper function to get count (number of active objects)
 * @return int of count
 */
int MyMalloc::getCount() const {
    return count;
}

/**
 * helper function to count the actual active entries in a table
 * @return number of entries
 */
int MyMalloc::countCount() const {
    int actualCount = 0;
    for (int i = 0; i < capacity; ++i) {
        if (startOfTable[i].active) actualCount++;
    }
    return actualCount;
}

