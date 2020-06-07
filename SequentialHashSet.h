//
// Created by mguzek on 03.06.2020.
//

#ifndef HASHSETHTM_SEQUENTIALHASHSET_H
#define HASHSETHTM_SEQUENTIALHASHSET_H

#include "HashSet.h"

class SequentialHashSet : public HashSet {
protected:
    void resize() override {
        tableSize *= 2;
        PrimitiveVector* newTable = new PrimitiveVector[tableSize];

        for(size_t i = 0 ; i < tableSize ; ++i) {
            for(size_t j = 0 ; j < table[i].getSize() ; ++j) {
                size_t destBucket = hash(table[i][j]) % tableSize;
                newTable[destBucket].push_back(table[i][j]);
            }
        }

        delete [] table;
        table = newTable;
    }

public:
    explicit SequentialHashSet(size_t initCapacity = 11) : HashSet{initCapacity} {

    }

    bool add(int item) override {
        size_t myBucket = hash(item) % tableSize;
        if(table[myBucket].find(item)) {
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
        size_t myBucket = hash(item) % tableSize;
        if(!table[myBucket].remove(item)) {
            return false;
        }

        --setSize;
        return true;
    }

    bool contains(int item) override {
        size_t myBucket = hash(item) % tableSize;
        return table[myBucket].find(item);
    }
};

#endif //HASHSETHTM_SEQUENTIALHASHSET_H
