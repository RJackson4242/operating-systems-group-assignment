/**
 * This file implements parallel mergesort.
 */

#include <stdio.h>
#include <string.h> /* for memcpy */
#include <stdlib.h> /* for malloc */
#include "mergesort.h"

/* this function will be called by mergesort() and also by parallel_mergesort(). */
void merge(int leftstart, int leftend, int rightstart, int rightend){
	int leftindex = leftstart;
	int rightindex = rightstart;
	int tempindex = leftstart;

	while(leftindex <= leftend && rightindex <= rightend) {
		if(A[leftindex] <= A[rightindex]) {
			B[tempindex++] = A[leftindex++];
		} else {
			B[tempindex++] = A[rightindex++];
		}
	}

	while(leftindex <= leftend) {
		B[tempindex++] = A[leftindex++];
	}

	while(rightindex <= rightend) {
		B[tempindex++] = A[rightindex++];
	}

	for(int i = leftstart; i <= rightend; i++) {
		A[i] = B[i];
	}
}

/* this function will be called by parallel_mergesort() as its base case. */
void my_mergesort(int left, int right){
	if(left >= right) return;
	int mid = left + (right - left) / 2;

	my_mergesort(left, mid);
	my_mergesort(mid + 1, right);

	merge(left, mid, mid + 1, right);
}

/* this function will be called by the testing program. */
void * parallel_mergesort(void *arg){
		return NULL;
}

/* we build the argument for the parallel_mergesort function. */
struct argument * buildArgs(int left, int right, int level){
		return NULL;
}

