//
// Created by mguzek on 03.06.2020.
//

#ifndef HASHSETHTM_HASHSETGCC_RTM_OPTIMIZED_H
#define HASHSETHTM_HASHSETGCC_RTM_OPTIMIZED_H

//Compile with -mrtm to work

#include <cstdlib>
#include <immintrin.h>
#include <thread>
#include <chrono>
#include "SequentialHashSet_LowContention.h"

class HashSetGCC_RTM_Optimized : public SequentialHashSet_LowContention {
protected:
    void resize() override {
        size_t status;
        for(int i = 0 ; i < RTM_ATTEMPTS ; ++i)
        {
            status = _xbegin();
            if (status == _XBEGIN_STARTED) {
                if (!(rmutex.isLocked())) {
                    SequentialHashSet_LowContention::resize();
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
        SequentialHashSet_LowContention::resize();
        rmutex.unlock();
    }

    ReentrantMutex rmutex;
    static constexpr size_t RTM_ATTEMPTS = 3;
    static constexpr size_t RTM_WAITNSEC = 2;

public:
    explicit HashSetGCC_RTM_Optimized(int initCapacity = 11) : SequentialHashSet_LowContention{initCapacity} {

    }

    bool add(int item) override {
        bool result;
        size_t status;
        for(int i = 0 ; i < RTM_ATTEMPTS ; ++i)
        {
            status = _xbegin();
            if (status == _XBEGIN_STARTED) {
                if (!(rmutex.isLocked())) {
                    result = SequentialHashSet_LowContention::add(item);
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
        result = SequentialHashSet_LowContention::add(item);
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
                    result = SequentialHashSet_LowContention::remove(item);
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
        result = SequentialHashSet_LowContention::remove(item);
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
                    result = SequentialHashSet_LowContention::contains(item);
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
        result = SequentialHashSet_LowContention::contains(item);
        rmutex.unlock();
        return result;
    }

    int size() override {
        int result;
        size_t status;
        for(int i = 0 ; i < RTM_ATTEMPTS ; ++i)
        {
            status = _xbegin();
            if (status == _XBEGIN_STARTED) {
                if (!(rmutex.isLocked())) {
                    result = SequentialHashSet_LowContention::size();
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
        result = SequentialHashSet_LowContention::size();
        rmutex.unlock();
        return result;
    }
};

#endif //HASHSETHTM_HASHSETGCC_RTM_OPTIMIZED_H
