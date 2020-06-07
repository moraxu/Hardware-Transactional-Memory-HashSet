//
// Created by mguzek on 02.06.2020.
//

#ifndef HASHSETHTM_HASHSET_H
#define HASHSETHTM_HASHSET_H

#include <mutex>
#include <atomic>
#include "PrimitiveVector.h"

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

    PrimitiveVector* table;
    size_t tableSize;

    explicit HashSet(size_t initCapacity) : table{new PrimitiveVector[initCapacity]}, tableSize{initCapacity} {

    }

    static int hash(int item) {
        return item;
    }

    virtual bool policy() = 0; //only called when already holding a lock
    virtual void resize() = 0;

public:
    virtual bool add(int item) = 0;
    virtual bool remove(int item) = 0;
    virtual bool contains(int item) = 0;
    virtual int size() = 0;

    virtual ~HashSet() {
        delete [] table;
    }
};

#endif //HASHSETHTM_HASHSET_H
