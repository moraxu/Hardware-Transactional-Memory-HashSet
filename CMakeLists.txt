cmake_minimum_required(VERSION 3.16)
project(HashsetHTM)

set(CMAKE_CXX_STANDARD 14)

add_executable(HashsetHTM main.cpp HashSet.h CoarseGrainedHashSet.h HashSetGCC_RTM.h tests.cpp)