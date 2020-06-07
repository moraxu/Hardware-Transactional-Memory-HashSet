//
// Created by mguzek on 6/7/20.
//

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include "HashSet.h"

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
    if(set->size() != 100)
        return false;
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
    if(set->size() != 5)
        return false;
    for(int i = 95; i > 0; i--)
        if(set->remove(i))
            return false;
    if(set->remove(101))
        return false;
    if(set->size() != 5)
        return false;
    if(set->remove(-1))
        return false;
    if(set->size() != 5)
        return false;
    if(!set->remove(99))
        return false;
    if(set->size() != 4)
        return false;
    return true;
}

void test(HashSet* set) {
    std::cout << "Test sequential... ";
    std::cout << (testSequential(set) ? "OK" : "FAIL") << std::endl << std::flush;

    std::cout << "Test concurrent... ";
    std::cout << (testConcurrent(set) ? "OK" : "FAIL") << std::endl << std::flush;
}