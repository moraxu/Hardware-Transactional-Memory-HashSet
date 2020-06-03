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

    bool policy() transaction_safe {    //only called when already holding a lock
        return setSize / table.size() > 4;
    }

    virtual void resize() transaction_safe_dynamic = 0;

public:
    virtual bool add(int item) transaction_safe_dynamic = 0;
    virtual bool remove(int item) transaction_safe_dynamic = 0;
    virtual bool contains(int item) transaction_safe_dynamic = 0;
};

#endif //HASHSETHTM_HASHSET_H
