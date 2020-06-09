Program compares the performance of a concurrent HashSet data structure (hash collisions resolved by separate chaining) implementation that uses a single coarse-grained lock with the implementation that utilizes Intel's TSX to leverage Hardware Transactional Memory.

Command used to compile the program (for the program to run, CPU needs to have Intel's TSX support):
$ g++ -std=c++11 -mrtm -pthread main.cpp tests.cpp

In order to execute the benchmark, run (in the same directory as the a.out executable):
$ python benchmark.py

Reading:
https://gcc.gnu.org/onlinedocs/gcc/x86-transactional-memory-intrinsics.html?fbclid=IwAR3z8qjRbh7jt3DBEvU6HL8WX4TBhYkVz3lKOvgIByNB8qJQnLA4Cj2m99Y
