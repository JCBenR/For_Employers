#include <__bit_reference>
//
// Created by Justin Siebenhaar on 3/30/21.

#include <cassert>
#include <iostream>
#include <thread>
#include <vector>
#include "ConcurrentQueue.h"
#include "ConcurrentQueueTests.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */

bool ConcurrentQueueTests::testQueue(int producers, int consumers, int ints){
        std::vector<std::thread> queueVector;
        int totalSize = producers + consumers;
        queueVector.reserve(totalSize);

        static auto *myQueue = new ConcurrentQueue<int>;
        for (int i = 0; i < producers; ++i) {
            std::cout << "making enqueue" << std::endl;
            std::thread th([&](){
                for (int j = 0; j < ints; ++j) {
                    myQueue->enqueue(j);
                }
            });
            queueVector.push_back(std::move(th));
        }

        for (int i = 0; i < consumers; ++i) {
            std::cout << "making dequeue" << std::endl;
            std::thread th([&](){
                int ret = -1;
                for (int j = 0; j < ints; ++j) {
                    myQueue->dequeue(&ret);
                }
            });
            queueVector.push_back(std::move(th));
        }

//    std::cout << queueVector.size() << std::endl;
    std::cout << "About to join..." << std::endl;

        for (auto &t: queueVector){
            try {

            t.join();
            } catch (int e){
                std::cerr << e << std::endl;
            }
        }

//        assert(myQueue->size() == 30);

        std::cout << GREEN << "finished all joins" << RESET << std::endl;

    std::cout << "producers: " << producers << std::endl;
    std::cout << "consumers: " << consumers << std::endl;
    std::cout << "ints: " << ints << std::endl;
    std::cout << "DQ Hits: " << myQueue->getDQhits() << std::endl;
    std::cout << "vector size: " << queueVector.size() << std::endl;


    if (((producers-consumers)*ints)  != queueVector.size()- myQueue->getDQhits())
            return false;
        return true;
}
