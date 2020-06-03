//
// Created by mguzek on 03.06.2020.
//

#ifndef HASHSETHTM_SEQUENTIALHASHSET_H
#define HASHSETHTM_SEQUENTIALHASHSET_H

#include <algorithm>
#include <functional>
#include "HashSet.h"

class SequentialHashSet : public HashSet {
protected:
    void resize() override {
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
    explicit SequentialHashSet(int initCapacity = 11) : HashSet{initCapacity} {

    }

    bool add(int item) override {
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

    bool remove(int item) override {
        size_t myBucket = std::hash<int>{}(item) % table.size();
        auto it = std::find(table[myBucket].begin(), table[myBucket].end(), item);
        if(it == table[myBucket].end()) {
            return false;
        }

        table[myBucket].erase(it);
        --setSize;
        return true;
    }

    bool contains(int item) override {
        size_t myBucket = std::hash<int>{}(item) % table.size();
        return std::find(table[myBucket].begin(), table[myBucket].end(), item) != table[myBucket].end();
    }
};

#endif //HASHSETHTM_SEQUENTIALHASHSET_H
