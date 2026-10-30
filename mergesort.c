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

	/* merge skipping:
	if last element of the left half is <= the first element of the right half skip the merge */
	if (leftstart <= leftend && rightstart <= rightend
	    && A[leftend] <= A[rightstart]) {
		return;
	}

	/* Repeatedly take the smaller front element of the two halves.
	   including <= instead of < keeps the sort stable by taking from the left half on ties */
	while (leftindex <= leftend && rightindex <= rightend) {
		if (A[leftindex] <= A[rightindex]) {
			B[tempindex++] = A[leftindex++];
		} else {
			B[tempindex++] = A[rightindex++];
		}
	}

	/* At most one of the halves still has elements so copy that remainder to a single block. */
	if (leftindex <= leftend) {
    	memcpy(&B[tempindex], &A[leftindex], (leftend - leftindex + 1) * sizeof(int));
	} else if (rightindex <= rightend) {
		memcpy(&B[tempindex], &A[rightindex], (rightend - rightindex + 1) * sizeof(int));
	}

	/* Copy the merged result from the B back into A. */
	memcpy(&A[leftstart], &B[leftstart], (rightend - leftstart + 1) * sizeof(int));
}

/* this function will be called by parallel_mergesort() as its base case once cutoff depth is reached. */
void my_mergesort(int left, int right){

	if(left >= right) return; // base case: already sorted or array is empty
	
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

	// base case
	if (left >= right) return NULL;

	// Stop parallelism once level reaches cutoff
	if (level >= cutoff) {
		my_mergesort(left, right);
		return NULL;
	}

	mid = left + (right - left) / 2;

	leftargs = buildArgs(left, mid, level + 1);
	rightargs = buildArgs(mid + 1, right, level + 1);

	// creating new thread which sorts the right half
	thread_created = (pthread_create(&rightthread, NULL, parallel_mergesort, rightargs) == 0);

	parallel_mergesort(leftargs); // current main thread sorting the left side

	if (thread_created) {                 // safety check incase the thread creation fails. 
		pthread_join(rightthread, NULL); // If thread was created join safely
	} else {
		parallel_mergesort(rightargs);  // if the thread creation has failed failed do direct function call to merge
	}

	// merge the sorted halves
	merge(left, mid, mid + 1, right);

	return NULL;
}

/* building the argument for the parallel_mergesort function. */
struct argument * buildArgs(int left, int right, int level){
	struct argument *args = (struct argument*)malloc(sizeof(struct argument));

	if (args == NULL) {
		fprintf(stderr, "memory allocation failed in buildArgs\n");
		exit(EXIT_FAILURE);
	}
	args->left = left;
	args->right = right;
	args->level = level;

	return args;
}

