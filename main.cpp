//
// Created by mguzek on 6/2/20.
//

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <string>
#include <random>
#include "CoarseGrainedHashSet.h"
#include "HashSetGCC_RTM.h"

void test(HashSet* set);

void threadBenchmark(HashSet* set, int upperLimit, int addPerc, int removePerc) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist; //default range is [0, MAX]

    for(int i = 0; i < upperLimit; i++) {
        if((i % addPerc) == 0) {
            set->add(dist(rng));
        }
        if((i % removePerc) == 0) {
            set->remove(dist(rng));
        }

        if((i % addPerc) != 0 && (i % removePerc) != 0) {
            set->contains(dist(rng));
        }
    }
}

void benchmark(HashSet* set, int upperLimit, int threadCount, int addPerc, int removePerc) {
    std::vector<std::thread> v(threadCount);

    auto t1 = std::chrono::high_resolution_clock::now();
    for(auto& t: v) {
        t = std::thread([set, upperLimit, addPerc, removePerc] {
            threadBenchmark(set, upperLimit, addPerc, removePerc);
        });
    }
    for(auto& t: v) {
        t.join();
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();

    /* Print time elapsed in miliseconds*/
    std::cout << duration << std::endl << std::flush;
}

int main(int argc, char** argv)
{
    if(argc != 6)
    {
        std::cout << "Usage: test <coarse-lock | gcc-rtm> <threads_count> <upper_limit> <add_percentage> <remove_percentage>\n";
        exit(1);
    }

    HashSet* set = nullptr;

    if(std::string(argv[1]) == "coarse-lock") {
       set = new CoarseGrainedHashSet();
    }
    else if(std::string(argv[1]) == "gcc-rtm") {
        set = new HashSetGCC_RTM();
    }
    else {
        std::cout << "The lock implementation is unknown.\n";
        exit(1);
    }

    int threadCount = std::stoi(argv[2]);
    int upperLimit = std::stoi(argv[3]);
    int addPerc = std::stoi(argv[4]);
    int removePerc = std::stoi(argv[5]);

    //test(set);

    //std::cout << "Benchmark... ";
    benchmark(set, upperLimit, threadCount, addPerc, removePerc);

    delete set;

    return 0;
}