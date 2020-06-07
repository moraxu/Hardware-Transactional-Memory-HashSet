//
// Created by mguzek on 06.06.2020.
//

#ifndef HASHSETHTM_PRIMITIVEVECTOR_H
#define HASHSETHTM_PRIMITIVEVECTOR_H

#include <cstddef>
#include <cstring>

class PrimitiveVector {
private:
    int* arr;
    size_t size;
    size_t capacity;

    void resize() transaction_safe {
        int* new_arr = new int[capacity*2];
        std::memcpy(new_arr, arr, capacity*sizeof(int));
        delete [] arr;
        arr = new_arr;
        capacity *= 2;
    }

public:
    explicit PrimitiveVector() : arr{new int[2]}, size{0}, capacity{2} {

    }

    void push_back(int item) transaction_safe {
        if(size == capacity) {
            resize();
        }
        arr[size++] = item;
    }

    bool find(int item) const transaction_safe {
        for(int i = 0 ; i < size ; ++i) {
            if(arr[i] == item) {
                return true;
            }
        }
        return false;
    }

    bool remove(int item) transaction_safe {
        for(int i = 0 ; i < size ; ++i) {
            if(arr[i] == item) {
               memmove(arr+i, arr+i+1, size-i-1);
               --size;
               return true;
            }
        }
        return false;
    }

    int& operator[](size_t index) transaction_safe {
        return arr[index];
    }

    size_t getSize() const transaction_safe {
        return size;
    }

    ~PrimitiveVector() {
        delete [] arr;
    }
};

#endif //HASHSETHTM_PRIMITIVEVECTOR_H
