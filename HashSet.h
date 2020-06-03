//
// Created by mguzek on 02.06.2020.
//

#ifndef HASHSETHTM_HASHSET_H
#define HASHSETHTM_HASHSET_H

#include <vector>
#include <mutex>
#include <atomic>

class HashSet {
protected:

    class ReentrantMutex {
    private:
        std::atomic<bool> locked;
        std::recursive_mutex rmutex;

    public:
        ReentrantMutex() : locked{false} {

        }

        void lock() {
            rmutex.lock();
            locked.store(true);
        }

        void unlock() {
            locked.store(false);
            rmutex.unlock();
        }

        bool isLocked() {
            return locked.load();
        }
    };

    std::vector<std::vector<int>> table;
    size_t setSize;

    explicit HashSet(int initCapacity) : table(initCapacity), setSize{0} {

    }

    virtual bool policy() = 0;
    virtual void resize() = 0;

public:
    virtual bool add(int item) = 0;
    virtual bool remove(int item) = 0;
    virtual bool contains(int item) = 0;
};

#endif //HASHSETHTM_HASHSET_H
