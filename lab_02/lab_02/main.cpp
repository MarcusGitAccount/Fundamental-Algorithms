/*
	author: Marcus Pop
	group: 30229
	lab no: 2nd
	
	In order to compare the 2 construction methods, top-down and bottom-up only the 
	number of nodes in the heap tree on which operations where performed where counted,
	i.e. the number of heapify recursive calls for the bottom-up method and number of nodes
	traversed upwards for each insertion in the top-down construction method.
	
	=> Top-Down: Insert n elements into the heap one by one and each insertion takes O(logn) time
	=> Height of the tree goes from less to logn
	=> ~ O(nlogn)
	
	=> Bottom-Up: Start from the bottom with the first node that is not a leaf,
	=> heights grow slower
	=> ~O(n) complexity
	
	Worst case for build construction in both cases is when the initial container has 
	the elements sorted in reversed order.
*/

#include <stdio.h>
#include <vector>
#include "../../Heap_t.h"

bool cmp(int parent, int child) {
	return parent > child;
}

void print(int a) {
	printf("%d ", a);
}

bool check_heapsort() {
	int items[] = { 45, 13, 21, 3, 2, 7, 8, 0, 1 };
	int n = sizeof(items) / sizeof(int);
	Heap<int> heap(items, cmp, n);

	printf("Checking heapsort. Array before and after heapsort\n");
	for (uint32_t i = 0; i < n; i++)
		printf("%d ", items[i]);
	printf("\n");

	heap.HeapSort();
	for (uint32_t i = 0; i < n; i++)
		printf("%d ", items[i]);
	printf("\n");

	return IsSorted(items, n);
}

const char* condition_str(bool condition, const char* a, const char* b) {
	return condition == true ? a : b;
}

void heap_as_priority_queue_operations() {
	Heap<int> heap(cmp);
	Heap<int> heap2(cmp);
	int elements[] = { 10, 15, 7, 18, 21, 5 };

	heap.push(10);
	heap.push(15);
	heap.push(7);
	heap.push(18);
	heap.push(21);
	heap.push(5);

	heap2.push_container(elements, 6);
	heap2.make_heap();

	heap.print_heap(print);
	printf("\n");
	heap2.print_heap(print);
	printf("\n");
	printf("Top: %d\n", *(heap.top()));

	int* container = heap.get_container(NULL);

	for (uint32_t i = 0; i < 6; i++)
		printf("%d ", container[i]);
	printf("\n");
}

void HeapConstructionAverage(Vector_t<uint32_t> sizes, uint32_t measurements = 5) {
	printf("Average case analysis\n");


	for (uint32_t i = 0; i < sizes.size(); i++) {
		uint32_t n = sizes[i];
		int* a = new int[n];
		int *b = new int[n];

		for (uint32_t m = 0; m < measurements; m++) {
			FillRandomArray(a, n);
			CopyArray(b, a, n);

			Heap<int> top(a, cmp, n);
			Heap<int> bottom(b, cmp, n);

			top.build_heap(n, "top-down");
			bottom.make_heap(n, "bottom-up");
		}

		profiler.average("top-down",  n, measurements);
		profiler.average("bottom-up", n, measurements);

		delete[] a;
		delete[] b;
	}

	profiler.createGroup("average", "top-down", "bottom-up");
}

void HeapConstructionWorst(Vector_t<uint32_t> sizes) {
	printf("Worst case analysis\n");

	for (uint32_t i = 0; i < sizes.size(); i++) {
		uint32_t n = sizes[i];

		int *a = new int[n];
		int *b = new int[n];

		FillRandomArray(a, n, 10, 50000, true, 1);
		FillRandomArray(b, n, 10, 50000, true, 1);

		Heap<int> top(a, cmp, n);
		Heap<int> bottom(b, cmp, n);

		top.build_heap(n, "top-down-w");
		bottom.make_heap(n, "bottom-up-w");

		delete[] a;
		delete[] b;
	}

	profiler.createGroup("worst-case", "top-down-w", "bottom-up-w");
}

int main(void) {
	printf("Heapsort works? %s\n", condition_str(check_heapsort(), "yes", "no"));

	Vector_t<uint32_t> sizes;
	for (uint32_t i = 10; i <= 30000; i += (i < 100 ? 10 : 100))
		sizes.push_back(i);

	HeapConstructionAverage(sizes, 5U);
	HeapConstructionWorst(sizes);
	profiler.showReport();

	return 0;
}