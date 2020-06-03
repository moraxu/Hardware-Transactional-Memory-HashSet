//
// Created by mguzek on 02.06.2020.
//

#ifndef HASHSETHTM_COARSEGRAINEDHASHSET_H
#define HASHSETHTM_COARSEGRAINEDHASHSET_H

#include "SequentialHashSet.h"

class CoarseGrainedHashSet : public SequentialHashSet {
protected:
    ReentrantMutex rmutex;

    void resize() override {
        rmutex.lock();
        SequentialHashSet::resize();    //will resize twice if someone beats us to it, but it's not a big issue
        rmutex.unlock();
    }

public:
    explicit CoarseGrainedHashSet(int initCapacity = 11) : SequentialHashSet{initCapacity} {

    }

    bool add(int item) override {
        rmutex.lock();
        bool returnVal = SequentialHashSet::add(item);
        rmutex.unlock();
        return returnVal;
    }

    bool remove(int item) override {
        rmutex.lock();
        bool returnVal = SequentialHashSet::remove(item);
        rmutex.unlock();
        return returnVal;
    }

    bool contains(int item) override {
        rmutex.lock();
        bool returnVal = SequentialHashSet::contains(item);
        rmutex.unlock();
        return returnVal;
    }
};

#endif //HASHSETHTM_COARSEGRAINEDHASHSET_H
