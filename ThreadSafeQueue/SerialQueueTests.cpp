//
// Created by Justin Siebenhaar on 3/30/21.

#include <cassert>
#include <iostream>
#include "SerialQueue.hpp"



int SerialQueueTests() {
    SerialQueue<int> testQueue;

    testQueue.enqueue(5);
    assert(testQueue.size() == 1);
    testQueue.enqueue(6);
    testQueue.enqueue(7);
    testQueue.enqueue(8);
    assert(testQueue.size() == 4);

    int myRet = 0;
    testQueue.dequeue(&myRet);
    assert(testQueue.size() == 3);
    assert(myRet == 6);
    testQueue.dequeue(&myRet);
    assert(myRet == 7);

    testQueue.dequeue(&myRet);
    testQueue.dequeue(&myRet);
    assert(testQueue.size() == 0);
    assert(!testQueue.dequeue(&myRet));

    //"\033[1;32mbold red text\033[0m\n";
    std::cout << "\033[1;32m --------all tests from SERIAL passed!-------- \033[0m" << std::endl;
    return 0;
}

//

