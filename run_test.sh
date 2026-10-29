#!/usr/bin/env bash
make test-mergesort-custom || exit 1


exec > test-results.txt 2>&1


trials=3

length=100000000

for n in 2 "$length"; do
    for cutoff in 0 1 2 3 4 5; do
        echo "sorted n:$n cutoff:$cutoff" 
        ./test-mergesort-custom "$n" "$cutoff" "$trials" sorted
        echo "exit code: $?"
    done
done

for n in 2 "$length"; do
    for cutoff in 0 1 2 3 4 5; do
        echo "rsorted n:$n cutoff:$cutoff" 
        ./test-mergesort-custom "$n" "$cutoff" "$trials" rsorted
        echo "exit code: $?"
    done
done


for n in 2 "$length"; do
    for cutoff in 0 1 2 3 4 5; do
        echo "random n:$n cutoff:$cutoff seed:42" 
        ./test-mergesort-custom "$n" "$cutoff" "$trials" random 42
        echo "exit code: $?"
    done
done

for n in 2 "$length"; do
    for cutoff in 0 1 2 3 4 5; do
        echo "negative n:$n cutoff:$cutoff seed:42" 
         ./test-mergesort-custom "$n" "$cutoff" "$trials" negative 42
        echo "exit code: $?"
    done
done


for n in 2 "$length"; do
    for cutoff in 0 1 2 3 4 5; do
        echo "equal n:$n cutoff:$cutoff number:7" 
        ./test-mergesort-custom "$n" "$cutoff" "$trials" equal 7
        echo "exit code: $?"
    done
done

for n in 2 "$length"; do
    for cutoff in 0 1 2 3 4 5; do
        echo "alternating n:$n cutoff:$cutoff num1:-2147483648 num2:2147483647" 
        ./test-mergesort-custom "$n" "$cutoff" "$trials" alternating -2147483648 -2147483647
        echo "exit code: $?"
    done
done
        
        
        
