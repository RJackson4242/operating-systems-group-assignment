README.md

## Project Number/Title 

* Authors: Arad Haghighatnejad, Lilith Jackson, Isaac Sheehan
* Group name: Group 37

## Overview

This program implements a multithreaded merge sort in C using POSIX threads (pthreads) to sort large arrays efficiently. It recursively splits the array into smaller subarrays, concurrently sorting each subarray in its own pthread until a configurable cutoff depth, below which it switches to a sequential merge sort, and then merges the sorted results.

### Our Approach in More Detail

The code sorts the array A with a top-down parallel merge sort. `parallel_mergesort` splits A in half and recurses until level reaches `cutoff` (configurable value). At each level, the current thread spawns a single `pthread` for the right half and sorts the left half itself so no thread sits idle and thread creation drops by a factor of 2. Once the cutoff is reached, each thread switches to a sequential `my_mergesort` to avoid creating threads for small ranges and reduce the overhead. If `pthread_create` fails, the right half will be sorted by a direct function call and hence the sort will complete correctly without errors.

Sorted halves are merged by `merge` which writes into the scratch array B and copies the result back into A (the main array). One minor optimisation done at this level was to skip the merge if the last element of the left half is not larger than the first element of the right half, meaning the range is already in order. Moreover, bulk tail copy was implemented, meaning once one half is exhausted, the remainder of the other half is moved with a single `memcpy` rather than a loop.

The two threads working on sibling halves touch disjoint index ranges of A and B and hence no locks are needed. The parent merges only after joining its child. `buildArgs` allocates each thread’s argument struct, which `parallel_mergesort` frees on entry, and it exits with an error message if `malloc` fails.

## Manifest

- `Makefile`: builds the `test-mergesort` program from the source files.
- `mergesort.c`: implements the parallel merge sort (`parallel_mergesort`, `my_mergesort`, `merge`, `buildArgs`).
- `mergesort.h`: declares the shared data (`A`, `B`, `cutoff`), `struct argument` and the function prototypes.
- `test-mergesort.c`: test program that fills an array, runs the sort, and checks and times the result.
- `README.md`: this file, an overview of the project.

## Building the project

Requires `gcc` (or another C compiler) and `make`.

    make

Compiles `mergesort.c` and `test-mergesort.c` and produces the `test-mergesort` executable.

To clean the repo by removing the build files:

    make clean

There is 3 custom bash scripts:
- run_test_custom.sh
- run_test_random.sh
- run_test.sh

These can be run with

    bash run_test.sh

    bash run_test_custom.sh

    bash run_test_random_.sh

This will run the script generating a variety of arrays for different testing.

## Features and usage

Summarise the main features of your program. It is also appropriate to
instruct the user how to use your program.

## Testing

Created a custom test script that read input from a text file to test specific custom inputs and has various functions to generate different types of arrays rather than just randomly generated inputs. Also created a bash script to automate the creation and testing of these different input arrays rather than manually running many different commands. The output of these commands is also piped into a text file so that they cna easily be read and compared through this script. The types of arrays tested and compared are sorted arrays, reverse sorted arrays, equal arrays where all arrays are the same element, alternating arrays where the array alternates between two values, random arrays which are made of random positive integers and random negative arrays that have random positive and negative numbers.




### The format of the test-mergesort-custom usage is as follows:
./test-mergesort-custom `input size` `cutoff level` `number of trials` `sorted`

./test-mergesort-custom `input size` `cutoff level` `number of trials`  `rsorted`

./test-mergesort-custom `input size` `cutoff level` `number of trials` `random` `seed`

./test-mergesort-custom `input size` `cutoff level` `number of trials` `negative` `seed`

./test-mergesort-custom `input size` `cutoff level` `number of trials` `equal` `number`

./test-mergesort-custom `input size` `cutoff level` `number of trials` `alternating` `num1` `num2`


To determine the speedup provided by the parallesiation the provided `test-mergesort.c` was called in a bash script call `run_test.sh` to test a number of cutoff values ranging from 0 (serial) to 10 (creation of many threads).

The following performance tests were conducted on a MacBook Pro (Mac17,2) with an Apple M5 chip (4 performance and 6 efficiency cores), 32 GB of memory, and macOS 26.4. Programs were compiled by Apple Clang 21.0.0.

| cutoff value | sort time (seconds) | Speed Up |
| ------------ | ------------------- |-|
| 0            |    22.40      | 1x |
| 1            |    11.38      | 1.97x |
| 2            |    6.01      | 3.73x|
| 3            |    3.39      | 6.61x |
| 4            |    3.26      | 6.87x |
| 5            |    3.19      | 7.02x |
| 6            |    3.22      | 6.96x |
| 7            |    3.23      | 6.91x |
| 8            |    3.18      | 7.04x |
| 9            |    3.18      | 7.04x |
| 10           |    3.44     | 6.51x |









## Known Bugs

List known bugs that you weren't able to fix (or ran out of time to fix).

## Reflection and Self Assessment

One problem we ran into during testing was the speed of running tests as the initial testing scheme involved writing and reading enormous arrays to files wasting signifigant time and space resources on the computer slowing down testing. Thus the custom testing scheme was adapted so the large arrays could be quickly generated by C code in memory rather than writing them to the disk which was much slower. This led to a great speedup in testing.

Another problem that we encounted was occasional errors when running the programm to do with memory management. By investigating what the error code was we were able to determine it was a double free error. To determine where this was occuring we analysed all the places `free()` and `malloc()` was used and found that the arg structure was inputted into the initial `parallel_mergesort()` call was freed by parallel_mergesort and then also freed by the testing file. Thus one of these had to be removed and the most obvious pick was the `free()` in the testing file since the `free()` in the `parallel_mergesort()` function also free all subsequent argument strucutres made by recursive calls of the function. Thus the incorrectly placed `free()` was removed and the error was overcome.

One thing that was only considered later into the development of the code was to adding a failsafe for the pthread creation. Without error handling for `pthread_create` if creation failed the code would join an uninitialised handle. Therefore it was chosen to fall back to sequential sorting on failure to avoid potential errors.

A challenging concept to study was the effect of adding a minimum subarray size check before creating more threads, in addition to the original cutoff check. With this check, even if the configurable cutoff value were set too high no additional threads would be created for small ranges, which would not benefit from parallelism and would only add unnecessary overhead. Studying this effect was valuable for understanding, but it was decided not to include the check in the code in case it did not aligned with the purpose of this assignment. However, without this check, the average time taken with a cutoff of 15 is roughly double that with a cutoff of 5, and with a cutoff of 20 the time taken is unnecessarily high and not feasible at all. A potential performance problem therefore exists if the configurable cutoff value is set too high, and adding the check back in would prevent it.

Discuss the issues you encountered during development and testing. What
problems did you have? What did you have to research and learn on your own?
What kinds of errors did you get? How did you fix them?

What parts of the project did you find challenging? Is there anything that
finally "clicked" for you in the process of working on this project? How well
did the development and testing process go for you?

## Sources Used

If you used any sources outside of the textbook, you should list them here. 
If you looked something up on stackoverflow.com or you use help from AI, and 
fail to cite it in this section, it will be considered plagiarism and dealt 
with accordingly. So be safe CITE!
