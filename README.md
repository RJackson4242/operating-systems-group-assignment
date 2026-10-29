README.md

## Project Number/Title 

* Authors: Arad Haghighatnejad, Lilith Jackson, Isaac Sheehan
* Group name: Group 37

## Overview

This program implements a multithreaded merge sort in C using POSIX threads (pthreads) to sort large arrays efficiently. It recursively splits the array into smaller subarrays, concurrently sorting each subarray in its own pthread until a configurable cutoff depth, below which it switches to a sequential merge sort, and then merges the sorted results.

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

## Features and usage

Summarise the main features of your program. It is also appropriate to
instruct the user how to use your program.

## Testing

This section should detail how you tested your code. Simply stating "I ran
it a few times and it seems to work" is not sufficient. Your testing needs
to be detailed here.


## Reflection and Self Assessment

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
