/*
** Laboratory no: 3rd
** Group: @30229
** Name: Marcus Pop
** Requirements: 
** - implementation and average complexity for quicksort
** - quickselect/randomized-select implementation
** - compare heapsort and quicksort
** - analyze quicksort's worst/best case complexity and its overall efficiency
*/

#include <stdio.h>
#include <stdint.h>
#include <algorithm> // for std::sort used to test if quickselect works
#include <time.h>
#include "../../Heap_t.h"

// @returns and integer >= lo and < hi
int random_int(int lo, int hi) {
	if (lo == hi)
		return lo;
	return (rand() % (hi - lo)) + lo;
}

uint32_t partition(int* arr, int lo, int hi, bool(*cmp)(int, int), bool random = true, 
	const char* line = NULL, uint32_t size = 0) {
	int32_t pivot, index;

	if (random) {
		pivot = random_int(lo, hi);
		swap_(&arr[pivot], &arr[hi]);
		// profiler
		//if (line)
		//	profiler.countOperation(line, size, 3);
	}
	pivot = lo - 1;
	index = lo;
	while (index < hi) {
		// profiler
		if (line)
			profiler.countOperation(line, size, 1);

		if (cmp(arr[index], arr[hi])) {
			pivot++;
			swap_(&arr[index], &arr[pivot]);
			// profiler
			if (line)
				profiler.countOperation(line, size, 3);
		}
		index++;
	}

	pivot++;
	swap_(&arr[pivot], &arr[hi]);
	// profiler
	if (line)
		profiler.countOperation(line, size, 3);

	return (uint32_t)pivot;
}

uint32_t quickselect(int* arr, int n, int k, bool(*cmp)(int, int)) {
	uint32_t hi, lo, pivot;

	if (k < 0 || k >= n)
		throw std::out_of_range("K is out of the array's range.\n");

	lo = 0;
	hi = n - 1;
	do {
		pivot = partition(arr, lo, hi, cmp);
		if (pivot < k)
			lo = pivot + 1;
		else
			hi = pivot - 1;
	} while (pivot != k);

	return pivot;
}

void quicksort(int* arr, int lo, int hi, bool(*cmp)(int, int), bool random = true,
	const char* line = NULL, uint32_t size = 0) {
	if (lo >= hi)
		return;

	const uint32_t pivot = partition(arr, lo, hi, cmp, random, line, size);

	quicksort(arr, lo, pivot - 1, cmp, random, line, size);
	quicksort(arr, pivot + 1, hi, cmp, random, line, size);
}

bool test_quicksort(bool(*cmp)(int, int), uint32_t tests = 10, uint32_t size = 100, bool unique = true) {
	int* arr = new int[size];

	for (uint32_t i = 0; i < tests; i++) {
		FillRandomArray(arr, size, 10, 5000, unique);
		quicksort(arr, 0, size - 1, cmp);

		if (!IsSorted(arr, size))
			return false;

	}
	
	delete[] arr;
	return true;
}

bool test_quickselect(bool(*cmp)(int, int), uint32_t tests = 10, uint32_t size = 100, bool unique = true) {
	int* arr  = new int[size];
	int* copy = new int[size];


	for (uint32_t i = 0; i < tests; i++) {
		uint32_t k = (uint32_t)(rand() % size);
		uint32_t kth;

		FillRandomArray(arr, size, 10, 5000, unique);
		CopyArray(copy, arr, size);
		std::sort(copy, copy + size);
		kth = quickselect(arr, size, k, cmp);

		if (IsSorted(copy, size))
			if (arr[kth] != copy[k])
				return false;
	}

	printf("\n");
	delete[] copy;
	delete[] arr;
	return true;
}

bool cmp(int a, int b) { return a < b; }

bool heap_cmp(int parent, int child) { return parent > child; }

void average_case_analysis(Vector_t<uint32_t> sizes, uint32_t measurements = 5U) {
	printf("Average case analysis between heapsort and quicksort\n");
	for (uint32_t i = 0; i < sizes.size(); i++) {

		uint32_t n = sizes[i];
		int* a = new int[n];
		int* b = new int[n];
		
		printf("Testing average case for size: %u\r", n);
		for (uint32_t m = 0; m < measurements; m++) {
			FillRandomArray(a, n, 1, 10000000, true);
			CopyArray(b, a, n);
			Heap<int> heap(a, heap_cmp, n);

			heap.HeapSort(n, "hs-avg", true);
			quicksort(b, 0, n - 1, cmp, true, "qs-avg", n);
		}
		
		delete[] a;
		delete[] b;

		profiler.average("hs-avg", n, measurements);
		profiler.average("qs-avg", n, measurements);
	}
	printf("\n");
	profiler.createGroup("hs vs qs avg case", "hs-avg", "qs-avg");
}

void best_case_analysis(Vector_t<uint32_t> sizes, uint32_t measurements = 5U) {
	for (uint32_t i = 0; i < sizes.size(); i++) {
		int n = sizes[i];

		int* arr = new int[n];

		// n - even all the times
		// 0 ... n / 2 - 1 elements from 0 to n / 2 - 1
		// n / 2 => n / 2
		// n / 2 + 1 .... n - 1 elements from n / 2 + 1 to n - 1

		printf("Testing best case for size: %u\r", n);
		FillRandomArray(arr, n , 1, 50000, true);
		quicksort(arr, 0, n - 1, cmp, true, "qs-best", n);

		delete[] arr;
	}
}

void worst_case_analysis(Vector_t<uint32_t> sizes, uint32_t measurements = 5U) {
	// Sorted array => all the partitions return splits of new sizes n - 1 and 0
	// => quadratic complexity O(n^2)

	for (uint32_t i = 0; i < sizes.size(); i++) {
		uint32_t n = sizes[i];

		int* arr = new int[n];
		
		printf("Testing worst case for size: %u\r", n);
		FillRandomArray(arr, n, 10, 50000, false, 1);
		quicksort(arr, 0, n - 1, cmp, false, "qs-worst", n);

		delete[] arr;
	}
}

# define LIMIT 10000

int main(void) {
	Vector_t<uint32_t> sizes;
	uint32_t n = 0;

	int arr[] = { 5, 8, 7, 6, 1, 3, 2, 4, 0 };
	uint32_t size = sizeof(arr) / sizeof(int);

	printf("%Quickselect result: %u\n", quickselect(arr, size, 6, cmp));

	while (n < LIMIT) {
		if (n < 100)
			n += 10;
		else if (n < 10000)
			n += 100;
		else
			n += 10000;
		sizes.push_back(n);
	}

	profiler.disableCounters();
	printf("Does QuickSort work? %s\n", test_quicksort(cmp) ? "yes" : "no");
	printf("Does QuickSelect work? %s\n", test_quickselect(cmp) ? "yes" : "no");
	profiler.enableCounters();

	best_case_analysis(sizes, 5U);
	average_case_analysis(sizes, 5U);
	worst_case_analysis(sizes, 5U);

	profiler.changeTitle("Quicksort and Quickselect");
	profiler.showReport();
	return 0;
}