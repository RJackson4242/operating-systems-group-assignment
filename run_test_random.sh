#!/usr/bin/env bash
make test-mergesort-custom || exit 1


exec > test-random-results.txt 2>&1


trials=10

length=100000000


for n in 2 "$length"; do
    for cutoff in 0 1 2 3 4 5; do
        echo "random n:$n cutoff:$cutoff seed:42" 
        ./test-mergesort-custom "$n" "$cutoff" "$trials" random 42
        echo "exit code: $?"
    done
done