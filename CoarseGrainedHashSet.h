//
// Created by mguzek on 6/2/20.
//

#ifndef HASHSETHTM_COARSEGRAINEDHASHSET_H
#define HASHSETHTM_COARSEGRAINEDHASHSET_H

#include "HashSet.h"

class CoarseGrainedHashSet : public HashSet {
protected:
    void resize() override {
        rmutex.lock();
        HashSet::resize();    //will resize twice if someone beats us to it, but it's not a big issue
        rmutex.unlock();
    }

public:
    explicit CoarseGrainedHashSet(size_t initCapacity = 11) : HashSet{initCapacity} {

    }

    bool add(int item) override {
        rmutex.lock();
        bool returnVal = HashSet::add(item);
        rmutex.unlock();
        return returnVal;
    }

    bool remove(int item) override {
        rmutex.lock();
        bool returnVal = HashSet::remove(item);
        rmutex.unlock();
        return returnVal;
    }

    bool contains(int item) override {
        rmutex.lock();
        bool returnVal = HashSet::contains(item);
        rmutex.unlock();
        return returnVal;
    }
};

#endif //HASHSETHTM_COARSEGRAINEDHASHSET_H
