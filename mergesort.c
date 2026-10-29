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

	/* if last element of the left half is <= the first element of the right half skip the merge */
	if (leftstart <= leftend && rightstart <= rightend
	    && A[leftend] <= A[rightstart]) {
		return;
	}

	while (leftindex <= leftend && rightindex <= rightend) {
		if (A[leftindex] <= A[rightindex]) {
			B[tempindex++] = A[leftindex++];
		} else {
			B[tempindex++] = A[rightindex++];
		}
	}

	if (leftindex <= leftend) {
    	memcpy(&B[tempindex], &A[leftindex], (leftend - leftindex + 1) * sizeof(int));
	} else if (rightindex <= rightend) {
		memcpy(&B[tempindex], &A[rightindex], (rightend - rightindex + 1) * sizeof(int));
	}

	memcpy(&A[leftstart], &B[leftstart], (rightend - leftstart + 1) * sizeof(int));
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
	int left, right, level, mid;
    void *leftargs;
    void *rightargs;
    pthread_t rightthread;
	int thread_created; // whether pthread_create succeeded

	struct argument *args = (struct argument*)arg;

	left = args->left;
	right = args->right;
	level = args->level;

	// free the memory allocated by buildArgs
	free(args);

	if (left >= right) return NULL;

	// Stop parallelism before cutoff is reached or subarray is too small
	if (level >= cutoff|| right - left + 1 <= 1000000) {
		my_mergesort(left, right);
		return NULL;
	}

	mid = left + (right - left) / 2;

	leftargs = buildArgs(left, mid, level + 1);
	rightargs = buildArgs(mid + 1, right, level + 1);

	thread_created = (pthread_create(&rightthread, NULL, parallel_mergesort, rightargs) == 0);

	parallel_mergesort(leftargs); // current main thread sorting the left side

	if (thread_created) {                 //safety incase the thread cration fails. 
		pthread_join(rightthread, NULL);
	} else {
		parallel_mergesort(rightargs);  // direct function call if the thread creation failed
	}

	merge(left, mid, mid + 1, right);

	return NULL;
}

/* we build the argument for the parallel_mergesort function. */
struct argument * buildArgs(int left, int right, int level){
	struct argument *args = (struct argument*)malloc(sizeof(struct argument));

	args->left = left;
	args->right = right;
	args->level = level;

	return args;
}

