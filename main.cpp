//
// Created by mguzek on 02.06.2020.
//

#include <iostream>
#include <vector>
#include <thread>
#include <string>
#include "CoarseGrainedHashSet.h"

void threadJob(HashSet* set, int id)
{
    for(int i = 0; i < 10; i++) {
        set->add(id + 1);
        std::this_thread::sleep_for (std::chrono::milliseconds(100));
        if(set->contains(id + 1))
            set->remove(id + 1);
    }
}

bool testConcurrent(HashSet* set) {
    std::vector<std::thread> v(8);
    int i = 0;
    for(auto& t: v) {
        t = std::thread([set, i]{
            threadJob(set, i);
        });
        i++;
    }
    for(auto& t: v) {
        t.join();
    }
    for(int i = 8; i > 0; i--)
        if(set->contains(i))
            return false;
    return true;
}

bool testSequential(HashSet* set) {
    for(int i = 100; i > 0; i--)
    {
        if(!set->add(i))
            return false;
    }
    for(int i = 100; i > 0; i--)
    {
        if(set->add(i))
            return false;
    }
    for(int i = 100; i > 0; i--)
        if(!set->contains(i))
            return false;
    for(int i = 95; i > 0; i--)
        if(!set->remove(i))
            return false;
    for(int i = 95; i > 0; i--)
        if(set->remove(i))
            return false;
    if(set->remove(101))
        return false;
    if(set->remove(-1))
        return false;
    if(!set->remove(99))
        return false;
    return true;
}

void threadBenchmark(HashSet* set, int id,  int upperLimit, int addPerc, int removePerc) {
    for(int i = 0; i < upperLimit; i++) {
        if((i % addPerc) == 0) {
            set->add(i);
        }
        else if((i % removePerc) == 0) {
            set->remove(i);
        }
        else {
            set->contains(i);
        }
    }
}

void benchmark(HashSet* set, int upperLimit, int threadCount, int addPerc, int removePerc) {
    std::vector<std::thread> v(threadCount);
    int i = 0;

    auto t1 = std::chrono::high_resolution_clock::now();
    for(auto& t: v) {
        t = std::thread([set, i, upperLimit, addPerc, removePerc] {
            threadBenchmark(set, i, upperLimit, addPerc, removePerc);
        });
        i++;
    }
    for(auto& t: v) {
        t.join();
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();

    /* Print time elapsed in miliseconds*/
    std::cout << duration << std::endl << std::flush;
}

void test(HashSet* set) {
    std::cout << "Test sequential... ";
    std::cout << (testSequential(set) ? "OK" : "FAIL") << std::endl << std::flush;

    std::cout << "Test concurrent... ";
    std::cout << (testConcurrent(set) ? "OK" : "FAIL") << std::endl << std::flush;
}

int main(int argc, char** argv)
{
    /*
    if(argc != 6)
    {
        std::cout << "Usage: test <coarse-lock | > <threads_count> <upper_limit> <add_percentage> <remove_percentage>\n";
        exit(1);
    }
     */

    HashSet* set;

    //if(std::string(argv[1]) == "coarse-lock") {
        set = new CoarseGrainedHashSet();
    //}
    /*
    else if(std::string(argv[1]) == "lock-free") {
        set = new LockFreeSet();
    }
     */
    //else {
    //    std::cout << "The lock implementation is unknown.\n";
    //    exit(1);
    //}
/*
    int threadCount = std::stoi(argv[2]);
    int upperLimit = std::stoi(argv[3]);
    int addPerc = std::stoi(argv[4]);
    int removePerc = std::stoi(argv[5]);
    */

    int threadCount = 2;
    int upperLimit = 10000;
    int addPerc = 20;
    int removePerc = 25;

    test(set);

    /*std::cout << "Benchmark... ";*/
    benchmark(set, upperLimit, threadCount, addPerc, removePerc);

    return 0;
}