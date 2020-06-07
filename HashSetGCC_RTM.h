//
// Created by mguzek on 03.06.2020.
//

#ifndef HASHSETHTM_HASHSETGCC_RTM_H
#define HASHSETHTM_HASHSETGCC_RTM_H

//Compile with -mrtm to work

#include <immintrin.h>
#include <thread>
#include <chrono>
#include <random>
#include "HashSet.h"

class HashSetGCC_RTM : public HashSet {
protected:
    void resize() override {
        size_t status;
        for(int i = 0 ; i < RTM_ATTEMPTS ; ++i)
        {
            status = _xbegin();
            if (status == _XBEGIN_STARTED) {
                if (!(rmutex.isLocked())) {
                    HashSet::resize();
                    _xend();
                    return;
                }
                _xabort(0xFF);
            }
            if((status & _XABORT_EXPLICIT) && _XABORT_CODE(status) == 0xff) {
                std::this_thread::sleep_for(std::chrono::nanoseconds((dist(rng) % RTM_WAITNSEC) + 1));
            }
            else if(!(status & _XABORT_RETRY)) {
                break;
            }
        }
        rmutex.lock();
        HashSet::resize();
        rmutex.unlock();
    }

    std::mt19937 rng;
    std::uniform_int_distribution<std::mt19937::result_type> dist; //default range is [0, MAX]
    static constexpr size_t RTM_ATTEMPTS = 3;
    static constexpr size_t RTM_WAITNSEC = 2;

public:
    explicit HashSetGCC_RTM(size_t initCapacity = 11) : HashSet{initCapacity}, rng{std::random_device{}()} {

    }

    bool add(int item) override {
        bool result;
        size_t status;
        for(int i = 0 ; i < RTM_ATTEMPTS ; ++i)
        {
            status = _xbegin();
            if (status == _XBEGIN_STARTED) {
                if (!(rmutex.isLocked())) {
                    result = HashSet::add(item);
                    _xend();
                    return result;
                }
                _xabort(0xFF);
            }
            if((status & _XABORT_EXPLICIT) && _XABORT_CODE(status) == 0xff) {
                std::this_thread::sleep_for(std::chrono::nanoseconds((dist(rng) % RTM_WAITNSEC) + 1));
            }
            else if(!(status & _XABORT_RETRY)) {
                break;
            }
        }
        rmutex.lock();
        result = HashSet::add(item);
        rmutex.unlock();
        return result;
    }

    bool remove(int item) override {
        bool result;
        size_t status;
        for(int i = 0 ; i < RTM_ATTEMPTS ; ++i)
        {
            status = _xbegin();
            if (status == _XBEGIN_STARTED) {
                if (!(rmutex.isLocked())) {
                    result = HashSet::remove(item);
                    _xend();
                    return result;
                }
                _xabort(0xFF);
            }
            if((status & _XABORT_EXPLICIT) && _XABORT_CODE(status) == 0xff) {
                std::this_thread::sleep_for(std::chrono::nanoseconds((dist(rng) % RTM_WAITNSEC) + 1));
            }
            else if(!(status & _XABORT_RETRY)) {
                break;
            }
        }
        rmutex.lock();
        result = HashSet::remove(item);
        rmutex.unlock();
        return result;
    }

    bool contains(int item) override {
        bool result;
        size_t status;
        for(int i = 0 ; i < RTM_ATTEMPTS ; ++i)
        {
            status = _xbegin();
            if (status == _XBEGIN_STARTED) {
                if (!(rmutex.isLocked())) {
                    result = HashSet::contains(item);
                    _xend();
                    return result;
                }
                _xabort(0xFF);
            }
            if((status & _XABORT_EXPLICIT) && _XABORT_CODE(status) == 0xff) {
                std::this_thread::sleep_for(std::chrono::nanoseconds((dist(rng) % RTM_WAITNSEC) + 1));
            }
            else if(!(status & _XABORT_RETRY)) {
                break;
            }
        }
        rmutex.lock();
        result = HashSet::contains(item);
        rmutex.unlock();
        return result;
    }
};

#endif //HASHSETHTM_HASHSETGCC_RTM_H
