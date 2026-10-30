#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/times.h> /* for times system call */
#include <sys/time.h>  /* for gettimeofday system call */
#include <unistd.h>
#include "mergesort.h"

/* the number of levels of threads, specified by the user */
int cutoff = 0;

/* the array we want to sort */
int *A;

/* when merge, we need a temp array (named B) to temporarily store elements of the original array */
int *B;

// function prototypes
int check_if_sorted(int A[], int n);
void generate_random_array(int A[], int n, int seed);
double getMilliSeconds(void);

/*
---------------------------------------------------------------------------
clock_t times(struct tms *buffer);
times() fills the structure pointed to by buffer with
time-accounting information.  The structure defined in
<sys/times.h> is as follows:
struct tms {
    clock_t tms_utime;       user time
    clock_t tms_stime;       system time
    clock_t tms_cutime;      user time, children
    clock_t tms_cstime;      system time, children
The time is given in units of 1/CLK_TCK seconds where the
value of CLK_TCK can be determined using the sysconf() function
with the agrgument _SC_CLK_TCK.
---------------------------------------------------------------------------
*/


/* we do not really use this function anymore, but somehow we still keep it here. */
float report_cpu_time(void)
{
	struct tms buffer;
	float cputime;

	times(&buffer);
	cputime = (buffer.tms_utime)/ (float) sysconf(_SC_CLK_TCK);
	return (cputime);
}

/* we do not really use this function anymore, but somehow we still keep it here. */
float report_sys_time(void)
{
	struct tms buffer;
	float systime;

	times(&buffer);
	systime = (buffer.tms_stime)/ (float) sysconf(_SC_CLK_TCK);
	return (systime);
}

double getMilliSeconds(void)
{
    struct timeval now;
    gettimeofday(&now, (struct timezone *)0);
    return (double) now.tv_sec*1000.0 + now.tv_usec/1000.0;
}


/*
 * generate_random_array(int A[], int n, int seed):
 *
 * description: Generate random integers in the range [0,RANGE]
 *              and store in A[0..n-1]
 */

#define RANGE 1000000

void generate_random_array(int A[], int n, int seed)
{
	int i;
	srandom(seed);
	for (i=0; i<n; i++)
        	A[i] = random()%RANGE;
}

void generate_random_negative_array(int A[], int n, int seed)
{
	int i;
	srandom(seed);
	for (i=0; i<n; i++)
        	A[i] = random()%RANGE-500000;
}

void generate_sorted_array(int A[], int n)
{
	int i;
	for (i=0; i<n; i++)
        	A[i] = i;
}

void generate_reverse_sorted_array(int A[], int n)
{
	int i;
	for (i=0; i<n; i++)
        	A[i] = n-i-1;
}

void generate_equal_array(int A[], int n, int num)
{
	int i;
	for (i=0; i<n; i++)
        	A[i] = num;
}

void generate_alternating_array(int A[], int n, int num1, int num2)
{
	int i;
	for (i=0; i<n; i++)
		if (i % 2 == 0)
		{
			A[i] = num1;
		} else
		{
			A[i] = num2;
		}
}

void read_file_array(int A[], int *n, int capacity, FILE* file)
{
	int file_length;
	int i;

	if (file == NULL) {
        fprintf(stderr, "file could not be opened\n");
        exit(1);
    }
	if (fscanf(file, "%d", &file_length) != 1 || file_length < 2 || file_length > capacity) {
        fprintf(stderr, "invalid file length (must be between 2 and %d)\n", capacity);
        fclose(file);
        exit(1);
    }
    for (i = 0; i < file_length; i++) {
        if (fscanf(file, "%d", &A[i]) != 1) {
			fprintf(stderr, "file ended early\n");
			fclose(file);
            exit(1); 
        }
    }
	*n = file_length;
}


/*
 * check_if_sorted(int A[], int n):
 *
 * description: returns TRUE if A[1..n] are sorted in nondecreasing order
 *              otherwise returns FALSE
 */

int check_if_sorted(int A[], int n)
{
	int i;
	for (i=0; i<n-1; i++) {
		if (A[i] > A[i+1]) {
			return FALSE;
		}
	}
	return TRUE;
}

/* this function prints A, but we only print the first 100 elements */
void printA(int n){
	int i;
	printf("Array A:");
	if (n >= 100)
	{
		n = 100;
	}
	for(i=0;i<n;i++){
		printf(" %d",A[i]);
	}
	printf("\n");
}

/* this function prints B, but we only print the first 100 elements */
void printB(int n){
	int i;
	printf("Array B:");
	if (n >= 100)
	{
		n = 100;
	}
	for(i=0;i<n;i++){
		printf(" %d",B[i]);
	}
	printf("\n");
}

int main(int argc, char **argv) {

	if (argc < 5) { // there must be at least one command-line argument
			fprintf(stderr, "Usage: %s <input size> <cutoff level> <trial> <number of trials> <type of array> <parameter> \n", argv[0]);
			exit(1);
	}

	int n = atoi(argv[1]);
	if(n<=1){
		printf("the input size must be at least 2!");
		exit(1);
	}
	cutoff = atoi(argv[2]);
	int trial = atoi(argv[3]);
	if(trial<1){
		printf("the number of trials must be at least 1");
		exit(1);
	} 
	int capacity = n;
	A = (int *) malloc(sizeof(int) * n);
	B = (int *) malloc(sizeof(int) * n);
	if (A == NULL || B == NULL) {
		fprintf(stderr, "could not allocate arrays\n");
		free(A);
		free(B);
		exit(1);
	}
	int count = 0;
	
	double sum = 0;

	while (count < trial)
	{
		if (strcmp(argv[4], "random") == 0)
		{
			// generate random input
			
			if (argc < 5) { // there must be at least one command-line argument
				fprintf(stderr, "Usage: %s <input size> <cutoff level> <number of trials> <random> <seed> \n", argv[0]);
				exit(1);
			}
			int seed = atoi(argv[5]);
			generate_random_array(A, n, seed);
		} else if (strcmp(argv[4], "negative") == 0)
		{
			// generate random input
			if (argc < 6) { // there must be at least one command-line argument
				fprintf(stderr, "Usage: %s <input size> <cutoff level> <number of trials> <negative> <seed> \n", argv[0]);
				exit(1);
			}
			int seed = atoi(argv[5]);
			generate_random_negative_array(A, n, seed);
		} else if (strcmp(argv[4], "sorted") == 0)
		{
			// generate sorted input
			generate_sorted_array(A, n);
		} else if (strcmp(argv[4], "rsorted") == 0)
		{
			// generate reverse sorted input
			generate_reverse_sorted_array(A, n);
		} else if (strcmp(argv[4], "equal") == 0)
		{
			// generate reverse sorted input
			if (argc < 6) { // there must be at least one command-line argument
				fprintf(stderr, "Usage: %s <input size> <cutoff level> <number of trials> <equal> <num> \n", argv[0]);
				exit(1);
			}
			int num = atoi(argv[5]);
			generate_equal_array(A, n, num);
		}  else if (strcmp(argv[4], "alternating") == 0)
		{
			// generate alternating input
			if (argc < 7) { // there must be at least one command-line argument
				fprintf(stderr, "Usage: %s <input size> <cutoff level> <number of trials> <alternating> <num1> <num2> \n", argv[0]);
				exit(1);
			}
			int num1 = atoi(argv[5]);
			int num2 = atoi(argv[6]);
			generate_alternating_array(A, n, num1, num2);
		}  else if (strcmp(argv[4], "file") == 0)
		{
			// read file input
			if (argc < 6) { // there must be at least one command-line argument
				fprintf(stderr, "Usage: %s <padding> <cutoff level> <number of trials> <file> <filepath> \n", argv[0]);
				exit(1);
			}
			FILE *file = fopen(argv[5], "r");

			read_file_array(A, &n, capacity, file);

			fclose(file);
		} else
		{
			fprintf(stderr, "Unknown array type provided.\n Accepted types are sorted, rsorted, equal, alternating, random, negative, file");
			exit(1);
		}


		double start_time;
		double sorting_time;

		// sort the input (and time it)
		start_time = getMilliSeconds();
		/* first index is 0, last index is n-1, we consider the main thread as level 0 */
		struct argument *arg=buildArgs(0, n-1, 0);
		parallel_mergesort(arg);
		sorting_time = getMilliSeconds() - start_time;

		if (check_if_sorted(A,n)) {
			//printf("Sorting %d elements took %4.2lf milliseconds.\n", n,  sorting_time);
			sum += sorting_time;
		} else {
			printf("%s: sorting failed!!!!\n", argv[0]);
			free(B);
			free(A);
			exit(EXIT_FAILURE);
		}
		/* Now we can free the memory for the two arrays*/
		count++;
	}
	free(B);
	free(A);

	double average = sum/(double)trial;
	
	printf("Sorting %d elements over %d trials took an average of %4.2lf milliseconds.\n", n, trial, average);

	exit(EXIT_SUCCESS);
}
