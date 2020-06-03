//
// Created by mguzek on 02.06.2020.
//

#ifndef HASHSETHTM_COARSEGRAINEDHASHSET_H
#define HASHSETHTM_COARSEGRAINEDHASHSET_H

#include <algorithm>
#include <functional>
#include "HashSet.h"

class CoarseGrainedHashSet : public HashSet {
private:
    bool policy() override {    //only called when already holding a lock
        return setSize / table.size() > 4;
    }

    void resize() override {
        size_t oldSize = table.size();
        rmutex.lock();

        if(oldSize == table.size()) {
            std::vector<std::vector<int>> newTable(oldSize*2);
            for(auto& bucket : table) {
                for(auto& item : bucket) {
                    size_t destBucket = std::hash<int>{}(item) % newTable.size();
                    newTable[destBucket].push_back(item);
                }
            }
            table = std::move(newTable);
        }

        rmutex.unlock();
    }

    ReentrantMutex rmutex;

public:
    explicit CoarseGrainedHashSet(int initCapacity = 11) : HashSet{initCapacity} {

    }

    bool add(int item) override {
        bool returnVal = false;
        rmutex.lock();

        size_t myBucket = std::hash<int>{}(item) % table.size();
        if(std::find(table[myBucket].begin(), table[myBucket].end(), item) == table[myBucket].end()) {
            table[myBucket].push_back(item);
            returnVal = true;
            ++setSize;
        }

        if(policy()) {
            resize();
        }

        rmutex.unlock();
        return returnVal;
    }

    bool remove(int item) override {
        bool returnVal = false;
        rmutex.lock();

        size_t myBucket = std::hash<int>{}(item) % table.size();
        auto it = std::find(table[myBucket].begin(), table[myBucket].end(), item);
        if(it != table[myBucket].end()) {
            table[myBucket].erase(it);
            returnVal = true;
            --setSize;
        }

        rmutex.unlock();
        return returnVal;
    }

    bool contains(int item) override {
        rmutex.lock();

        size_t myBucket = std::hash<int>{}(item) % table.size();
        bool returnVal = std::find(table[myBucket].begin(), table[myBucket].end(), item) != table[myBucket].end();

        rmutex.unlock();
        return returnVal;
    }

};

#endif //HASHSETHTM_COARSEGRAINEDHASHSET_H
