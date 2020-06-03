//
// Created by mguzek on 03.06.2020.
//

#ifndef HASHSETHTM_HASHSETGCC_LIBITM_H
#define HASHSETHTM_HASHSETGCC_LIBITM_H

//Compile with -fgnu-tm to work

#include "SequentialHashSet.h"

class HashSetGCC_Libitm : public SequentialHashSet {
protected:
    void resize() override {
        atomic_noexcept {
            SequentialHashSet::resize();
        }
    }

public:
    explicit HashSetGCC_Libitm(int initCapacity = 11) : SequentialHashSet{initCapacity} {

    }

    bool add(int item) override {
        atomic_noexcept {
            return SequentialHashSet::add(item);
        }
    }

    bool remove(int item) override {
        atomic_noexcept {
            return SequentialHashSet::remove(item);
        }
    }

    bool contains(int item) override {
        atomic_noexcept {
            return SequentialHashSet::contains(item);
        }
    }
};

#endif //HASHSETHTM_HASHSETGCC_LIBITM_H
