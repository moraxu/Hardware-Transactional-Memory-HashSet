//
// Created by mguzek on 02.06.2020.
//

#ifndef HASHSETHTM_HASHSET_H
#define HASHSETHTM_HASHSET_H

#include <vector>
#include <algorithm>
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

    ReentrantMutex rmutex;
    std::vector<std::vector<int>> table;
    size_t setSize;

    bool policy() { //only called when already holding a lock
        return setSize / table.size() > 4;
    }

    virtual void resize() {
        std::vector<std::vector<int>> newTable(table.size()*2);
        for(auto& bucket : table) {
            for(auto& item : bucket) {
                size_t destBucket = std::hash<int>{}(item) % newTable.size();
                newTable[destBucket].push_back(item);
            }
        }
        table = std::move(newTable);
    }

public:
    explicit HashSet(size_t initCapacity) : table(initCapacity), setSize{0} {

    }

    virtual bool add(int item) {
        size_t myBucket = std::hash<int>{}(item) % table.size();
        if(std::find(table[myBucket].begin(), table[myBucket].end(), item) != table[myBucket].end()) {
            return false;
        }

        table[myBucket].push_back(item);
        ++setSize;

        if(policy()) {
            resize();
        }
        return true;
    }

    virtual bool remove(int item) {
        size_t myBucket = std::hash<int>{}(item) % table.size();
        auto it = std::find(table[myBucket].begin(), table[myBucket].end(), item);
        if(it == table[myBucket].end()) {
            return false;
        }

        table[myBucket].erase(it);
        --setSize;
        return true;
    }

    virtual bool contains(int item) {
        size_t myBucket = std::hash<int>{}(item) % table.size();
        return std::find(table[myBucket].begin(), table[myBucket].end(), item) != table[myBucket].end();
    }

    size_t size() {
        return setSize;
    }
};

#endif //HASHSETHTM_HASHSET_H
