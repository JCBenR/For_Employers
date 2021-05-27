#include <iostream>
#include <chrono>
#include <sys/mman.h>
#include "MyMalloc.h"

int main() {
    MyMalloc mytable = MyMalloc(5);
    assert(mytable.getCapacity() == 5);
    void *ten = mytable.allocate(sizeof(int));
    void *eleven = mytable.allocate(11);
    void *twelve = mytable.allocate(12);
    void *thirteen = mytable.allocate(13);
    assert(mytable.getCapacity() == 10);
    void *fourteen = mytable.allocate(14);
    assert(mytable.getCount() == 5);
    mytable.deallocate(ten);
    assert(mytable.getCount() == 4);
    void *fifteen = mytable.allocate(15);
    assert(mytable.getCount() == 5);

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; ++i) {
        mytable.allocate(sizeof(std::string));
    }
    //mytable.printTable();
    auto stop = std::chrono::high_resolution_clock::now();
//    assert(mytable.getCount() == mytable.countCount());
    std::cout << mytable.getCount() << std::endl;
    std::cout << mytable.countCount() << std::endl;

    auto duration = duration_cast<std::chrono::microseconds>(stop - start);
    assert(mytable.getCount() == 1005);

    auto malstart = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 1000; ++i) {
        malloc(sizeof(std::string));
    }

    auto malstop = std::chrono::high_resolution_clock::now();

    auto malduration = duration_cast<std::chrono::microseconds>(malstop - malstart);
    std::cout << "Timing Comparison" << std::endl;
    std::cout << "-------------------" << std::endl;

    std::cout << "MyMalloc Time: \t" << duration.count() << std::endl;
    std::cout << "malloc Time: \t" << malduration.count() << std::endl;

    return 0;
}
