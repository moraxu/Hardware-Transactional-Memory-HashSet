//
// Created by mguzek on 03.06.2020.
//

#ifndef HASHSETHTM_HASHSETGCC_RTM_H
#define HASHSETHTM_HASHSETGCC_RTM_H

//Compile with -mrtm to work

#include <cstdlib>
#include <immintrin.h>
#include <thread>
#include <chrono>
#include "SequentialHashSet.h"

class HashSetGCC_RTM : public SequentialHashSet {
protected:
    void resize() override {
        size_t status;
        for(int i = 0 ; i < RTM_ATTEMPTS ; ++i)
        {
            status = _xbegin();
            if (status == _XBEGIN_STARTED) {
                if (!(rmutex.isLocked())) {
                    SequentialHashSet::resize();
                    _xend();
                    return;
                }
                _xabort(0xFF);
            }
            if((status & _XABORT_EXPLICIT) && _XABORT_CODE(status) == 0xff) {
                std::this_thread::sleep_for(std::chrono::nanoseconds((rand() % RTM_WAITNSEC) + 1));
            }
            else if(!(status & _XABORT_RETRY)) {
                break;
            }
        }
        rmutex.lock();
        SequentialHashSet::resize();
        rmutex.unlock();
    }

    ReentrantMutex rmutex;
    static constexpr size_t RTM_ATTEMPTS = 3;
    static constexpr size_t RTM_WAITNSEC = 2;

public:
    explicit HashSetGCC_RTM(int initCapacity = 11) : SequentialHashSet{initCapacity} {

    }

    bool add(int item) override {
        bool result;
        size_t status;
        for(int i = 0 ; i < RTM_ATTEMPTS ; ++i)
        {
            status = _xbegin();
            if (status == _XBEGIN_STARTED) {
                if (!(rmutex.isLocked())) {
                    result = SequentialHashSet::add(item);
                    _xend();
                    return result;
                }
                _xabort(0xFF);
            }
            if((status & _XABORT_EXPLICIT) && _XABORT_CODE(status) == 0xff) {
                std::this_thread::sleep_for(std::chrono::nanoseconds((rand() % RTM_WAITNSEC) + 1));
            }
            else if(!(status & _XABORT_RETRY)) {
                break;
            }
        }
        rmutex.lock();
        result = SequentialHashSet::add(item);
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
                    result = SequentialHashSet::remove(item);
                    _xend();
                    return result;
                }
                _xabort(0xFF);
            }
            if((status & _XABORT_EXPLICIT) && _XABORT_CODE(status) == 0xff) {
                std::this_thread::sleep_for(std::chrono::nanoseconds((rand() % RTM_WAITNSEC) + 1));
            }
            else if(!(status & _XABORT_RETRY)) {
                break;
            }
        }
        rmutex.lock();
        result = SequentialHashSet::remove(item);
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
                    result = SequentialHashSet::contains(item);
                    _xend();
                    return result;
                }
                _xabort(0xFF);
            }
            if((status & _XABORT_EXPLICIT) && _XABORT_CODE(status) == 0xff) {
                std::this_thread::sleep_for(std::chrono::nanoseconds((rand() % RTM_WAITNSEC) + 1));
            }
            else if(!(status & _XABORT_RETRY)) {
                break;
            }
        }
        rmutex.lock();
        result = SequentialHashSet::contains(item);
        rmutex.unlock();
        return result;
    }
};

#endif //HASHSETHTM_HASHSETGCC_RTM_H
