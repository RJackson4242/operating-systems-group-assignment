#!/usr/bin/env bash
make test-mergesort || exit 1


exec > test-results.txt 2>&1

length=100000000

for n in "$length"; do
    for cutoff in 0 1 2 3 4 5 6 7 8 9 10 15 20; do
        echo "random n:$n cutoff:$cutoff seed:42" 
        ./test-mergesort "$n" "$cutoff" 42
        echo "exit code: $?"
    done
done
