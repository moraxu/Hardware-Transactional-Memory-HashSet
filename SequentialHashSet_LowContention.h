//
// Created by mguzek on 03.06.2020.
//

#ifndef HASHSETHTM_SEQUENTIALHASHSET_LOWCONTENTION_H
#define HASHSETHTM_SEQUENTIALHASHSET_LOWCONTENTION_H

#include <algorithm>
#include <functional>
#include "HashSet.h"

class SequentialHashSet_LowContention : public HashSet {
protected:
    bool policy() override {
        return size() / table.size() > 4;
    }

    void resize() override {
        std::vector<std::vector<int>> newTable(table.size()*2);
        std::vector<int> newSizes(sizes.size()*2);
        for(auto& bucket : table) {
            for(auto& item : bucket) {
                size_t destBucket = std::hash<int>{}(item) % newTable.size();
                newTable[destBucket].push_back(item);
                ++newSizes[destBucket];
            }
        }
        table = std::move(newTable);
        sizes = std::move(newSizes);
        cachedSize = -1;
    }

    std::vector<int> sizes;
    long long int cachedSize;

public:
    explicit SequentialHashSet_LowContention(int initCapacity = 11) : HashSet{initCapacity}, sizes(initCapacity), cachedSize{0} {

    }

    bool add(int item) override {
        size_t myBucket = std::hash<int>{}(item) % table.size();
        if(std::find(table[myBucket].begin(), table[myBucket].end(), item) != table[myBucket].end()) {
            return false;
        }

        table[myBucket].push_back(item);
        ++sizes[myBucket];
        if(cachedSize >= 0) cachedSize = -1;

        if(policy()) {
            resize();
        }
        return true;
    }

    bool remove(int item) override {
        size_t myBucket = std::hash<int>{}(item) % table.size();
        auto it = std::find(table[myBucket].begin(), table[myBucket].end(), item);
        if(it == table[myBucket].end()) {
            return false;
        }

        table[myBucket].erase(it);
        --sizes[myBucket];
        if(cachedSize >= 0) cachedSize = -1;
        return true;
    }

    bool contains(int item) override {
        size_t myBucket = std::hash<int>{}(item) % table.size();
        return std::find(table[myBucket].begin(), table[myBucket].end(), item) != table[myBucket].end();
    }

    int size() override {
        if(cachedSize < 0) {
            cachedSize = 0;
            for(int bucketSize : sizes) {
                cachedSize += bucketSize;
            }
        }
        return cachedSize;
    }
};

#endif //HASHSETHTM_SEQUENTIALHASHSET_LOWCONTENTION_H
