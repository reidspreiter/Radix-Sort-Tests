/* Reid Spreiter
 * rhs0073
 * CSCE 3110 - HW5
 * 11/22/2023
 * Variable-base radix sort and analysis
 */


#include <algorithm>
#include <cstdlib>
#include <ctype.h>
#include <iomanip>
#include <iostream>
#include <time.h>
#include <vector>
using namespace std;


/* Function to populate an array of a given size */
void populate(int size, int arr[]) {

	for (int i = 0; i < size; i++) {
		arr[i] = rand() % 10001;
	}
}


/* Function to find the max value in an array */
int getMax(int size, int arr[]) {

	int max = arr[0];
	for (int i = 1; i < size; i++) {
		if (arr[i] > max) {
			max = arr[i];
		}
	}
	return max;
}


/* Radix sort function using bucket sort as intermediate algorithm */
void radixSort(int size, int base, int arr[]) {

	/* Get max to obtain max number of digits */
	int max = getMax(size, arr);

	/* Iterate from LSD to MSD, ensuring all digits in max are covered */
	for (int div = 1; max / div > 0; div *= base) {

		/* Make array of vector buckets */
		vector<int> buckets[base];

		/* Place elements in buckets */
		for (int i = 0; i < size; i++) {
			int bucketIndex = (arr[i] / div) % base;
			buckets[bucketIndex].push_back(arr[i]);
		}

		/* Sort each bucket */
		for (int i = 0; i < base; i++) {
			sort(buckets[i].begin(), buckets[i].end());
		}

		/* Concatenate buckets into original array */
		int arrIndex = 0;
		for (int i = 0; i < base; i++) {
			for (int val : buckets[i]) {
				arr[arrIndex++] = val;
			}
		}
	}
}			


/* Function to print array */
void print(int size, int arr[]) {

	for (int i = 0; i < size; i++) {
		cout << arr[i] << " ";
	}
	cout << endl;
}


/* Function to find passage of time, avoiding negative vals as necessary */
double passedTime(timespec start, timespec stop) {

	timespec temp;
	if ((stop.tv_nsec - start.tv_nsec) < 0) {
		temp.tv_sec = stop.tv_sec - start.tv_sec - 1;
		temp.tv_nsec = 1000000000 + stop.tv_nsec - start.tv_nsec;
	} else {
		temp.tv_sec = stop.tv_sec - start.tv_sec;
		temp.tv_nsec = stop.tv_nsec - start.tv_nsec;
	}
	return temp.tv_sec + temp.tv_nsec;
}


/* Main Driver code to test Radix Sort */
int main(int argc, char *argv[]) {

	/* Variables to manage time tracking */
	struct timespec start, stop;
	double sortTime, totalTime = 0.0;

	/* Seed random mumber generator */ 
	srand(time(0));

	/* Check for proper usage and define base */
	if (argc != 2 || !isdigit(*argv[1])) {
		perror("Usage: ./radixsort <base (integer)>");
		exit(EXIT_FAILURE);
	}
	int base = atoi(argv[1]);

	/* Populate array of size n and sort it */
	for (int n = 10; n <= 10000; n *= 10) {

		cout << "Radix Sort: base = " << base << " size = " << n << "\n";

		/* Perform radix sort 10 times for each size n */
		for (int i = 1; i <= 10; i++) {

			/* Populate array of size n */
			int arr[n];
			populate(n, arr);

			/* Only print first sort of array of size 10 */
			if (n == 10 && i == 1) {
				cout << "Unsorted: ";
				print(n, arr);
			}

			/* Get start time */
			if (clock_gettime(CLOCK_REALTIME, &start) == -1) {
				perror("clock_gettime start");
				exit(EXIT_FAILURE);
			}

			/* Sort array */
			radixSort(n, base, arr);

			/* Get end time */
			if (clock_gettime(CLOCK_REALTIME, &stop) == -1) {
				perror("clock_gettime stop");
				exit(EXIT_FAILURE);
			}

			/* Only print first sort of array of size 10 */
			if (n == 10 && i == 1) {
				cout << "Sorted  : ";
				print(n, arr);
			}

			/* Calculate sortTime and add to total */
			sortTime = passedTime(start, stop);
			totalTime += sortTime;

			/* Print time data */
			cout << "Pass " << setw(2) << right << i << ":  " 
			     << fixed << setprecision(1) << setw(11) << right 
			     << sortTime << " nanoseconds\n";
		}
		/* Print average sort time for size n */
		cout << "Average:  " << fixed << setprecision(1) << setw(11) 
		     << right << totalTime / 10.0 << " nanoseconds\n" << endl;
		totalTime = 0;
	}
	return 0;
}
