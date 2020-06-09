#!/usr/bin/python
import subprocess

# Produces a CSV formatted table with ops/millisecs throughput values in each cell
limit = 100000
threadNums = [1, 2, 4, 6, 8]
benchmark = "./a.out"
#9% add, 1% remove and 90% contains AND 50% add, 10% remove, 40% contains
for add, remove in [(11, 100), (2, 10)]:

    #CSV header
    print("")
    print("Workload: {:.2f}% contains operation".format((1 - (1/add) - (1/remove)) * 100))
    print(" ", end="");
    for threads in threadNums:
        print(", " + str(threads) + "T", end="")
    print("")

    for lock in ["coarse-lock", "gcc-rtm"]:
        print(lock, end="")
        for threads in threadNums:
            cmd = benchmark + " " + str(lock) + " " + str(threads) + " " + str(limit) + " " + str(add) + " " + str(remove)
            p = subprocess.Popen(cmd, stdout=subprocess.PIPE, shell=True)
            output = p.communicate()[0]
            p.wait()
            result = float(output.decode().strip())
            print(", {:.6f}".format((limit * threads) / (result)), end="")
        print("")
