//
// Created by Justin Siebenhaar on 3/30/21.
//

#include "ConcurrentQueue.h"
#include "SerialQueueTests.h"
#include "ConcurrentQueue.h"
#include "ConcurrentQueueTests.h"
#include <cassert>
#include <iostream>
#include <string>



int main(int argc, char *argv[]) {

    //SerialQueueTests();
    assert(argc >= 4);

    int producers = std::stoi(argv[1]);
    int consumers = std::stoi(argv[2]);
    int ints = std::stoi(argv[3]);

//    std::cout << producers << std::endl;
//    std::cout << consumers << std::endl;
//    std::cout << ints << std::endl;

    bool value = ConcurrentQueueTests::testQueue(producers, consumers, ints);
    std::cout << value << std::endl;

}
