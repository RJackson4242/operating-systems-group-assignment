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
	int i;

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
    pthread_t thread_left;

	struct argument *args = (struct argument*)arg;

	left = args->left;
	right = args->right;
	level = args->level;

	// free the memory allocated by buildArgs
	free(args);

	if (left >= right) return NULL;

	if (level >= cutoff) {
		my_mergesort(left, right);
		return NULL;
	}

	mid = left + (right - left) / 2;

	leftargs = buildArgs(left, mid, level + 1);
	rightargs = buildArgs(mid + 1, right, level + 1);

	pthread_t leftthread;
	pthread_t rightthread;

	pthread_create(&leftthread, NULL, parallel_mergesort, leftargs);
	pthread_create(&rightthread, NULL, parallel_mergesort, rightargs);
	pthread_join(leftthread, NULL);
	pthread_join(rightthread, NULL);

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

