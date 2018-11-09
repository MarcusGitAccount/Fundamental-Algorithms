/**
 *
 * @author Marcus Pop
 * @group  30229
 *
 * Specificațiile problemei:
 * Implementarea a 3 metode direct de sortare:
 * - BubbleSort, InsertionSort, SelectionSort
 * ... și compararea acestora  (în cazurile favorabil, defavorabil și
 * mediu statistic), exemplificarea corectitudinii si analiză (grafice)
 * + discuție, interpretare, eficiență, comparări, stabilitate
 *

 Comparare algoritmi în cele trei cazuri și input-urile care conduc la apariția
 acestora:

 Bubble sort:
 - cel mai rapid în cazul favorabil(array sortat) deoarece va face mereu n - 1 comparații și 0
	 operații adiționale
 - cel mai lent în cazul defavorabil deoarece va face mereu (n, 2) comparații și (n, 2) swap-uri

 Insertion sort:
	- căutare poziție inserare prin metodă lineară vs căutare poziție prin căutare binară
	În cazul cel mai favorabil(când array-ul este sortat) metoda lineară este mai eficientă
	deoarece face 0 comparații oprindu-se la primul element.
	În restul cazurilor a doua metodă metodă deoarece produce doar O(logn) comparații, în timp ce prima
	va produce O(initial - found_position). Numărul de atribuiri ulterioare pentru 'shiftarea' părții
	sortate este același în ambele cazuri.

	În cazul mediu, oricare dintre cele 2 variante de algoritmi este cea mai eficientă metodă de sortare directă
	deoarece spre deosebire de selection sort/bubble sort numărul de elemente peste care se fac comparații este minim.

Selection sort:
	- cel mai rapid în cazul defavorabil. insertion-sort-b este apropiat de acesta datorită numărului redus de comparații
		rezultat datorită folosirii căutării binare

 Stabilitate:
	- BubbleSort: da
	- InsertionSort: căutare lineară poziție inserare -> da
									 căutare binara poziție inserare  -> da(dacă se folosește upper bound)
	- SelectionSort: da, dacă maximul/minimul nu se schimbă și în caz de egalitate

 Obs: Overall, insertion sort implementat folosing upper bound pentru căutarea poziției pentru inserare
			este cel mai optim algoritm de sortare directă.
**/

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "../../Profiler.h"

Profiler profiler("directSorting");

#define SIZE 1000

/*
** Helper functions used. (plus the average defined in the Profiler class as described below)
**
** bool average(const char* name, int size, int average) {
** 	if (countersDisabled == true)
** 		return false;
** 	if (opcountMap.find(name) == opcountMap.end())
** 		return false;
** 	if (opcountMap[name].find(size) == opcountMap[name].end())
** 		return false;
** 	opcountMap[name][size] /= average;
** 	return true;
** }
**
*/

/*
** Implements upper bound search in an array in O(logn) time
** @returns last position in array where target would have to be
** inserted in order to keep the array sorted.
** Usage: InsertionSortBinary
*/
int upperBound(int mode, int arr[], int end, int n, int target, const char* line) {
	int lo, hi;

	lo = 0; hi = end;
	while (lo < hi) {
		const int mid = lo + ((hi - lo) >> 1);

		if (mode == 0 || mode == 2)
			profiler.countOperation(line, n, 1);
		if (target >= arr[mid])
			lo = mid + 1;
		else
			hi = mid;
	}
	return lo;
}

template <typename T>
void swap_(T* a, T* b) {
	T temp = *a;

	*a = *b;
	*b = temp;
}

template <typename T>
void CopyArray(T* source, T* target, int n) {
	for (int i = 0; i < n; i++)
		target[i] = source[i];
}

void BubbleSort(int mode, int arr[], int n, const char* line = "bubble-sort") {
	int iteration = 0;
	bool isSorted = false;

	// init line
	profiler.countOperation(line, n, 0);
	do {
		isSorted = true;
		for (int i = 0; i < n - iteration - 1; i++) {
			if (mode == 0 || mode == 2)
				profiler.countOperation(line, n, 1);

			if (arr[i] > arr[i + 1]) {
				isSorted = false;
				swap_(&arr[i], &arr[i + 1]);
				if (mode == 1 || mode == 2)
					profiler.countOperation(line, n, 3);
			}
		}
		iteration++;
	} while (!isSorted);
}

void SelectionSort(int mode, int arr[], int n, const char* line = "selection-sort") {
	// init line
	profiler.countOperation(line, n, 0);
	for (int i = 0; i < n - 1; i++) {
		int maxIndex = i;

		for (int j = i; j < n; j++) {
			if (mode == 0 || mode == 2)
				profiler.countOperation(line, n);
			if (arr[j] < arr[maxIndex])
				maxIndex = j;
		}

		if (maxIndex != i) {
			swap_(&arr[i], &arr[maxIndex]);
			if (mode == 1 || mode == 2)
				profiler.countOperation(line, n, 3);
		}
	}
}

/*
** Implements Insertion Sort using upper bound as the method of searching
** of the position where the element would have to be inserted.
*/
void InsertionSortBinary(int mode, int arr[], int n, const char* line = "insertion-sort-b") {
	// init line
	profiler.countOperation(line, n, 0);
	for (int i = 1; i < n; i++) {
		int current = arr[i];
		int j = upperBound(mode, arr, i, n, current, line);

		for (int index = i - 1; index > j; index--) {
			if (mode == 1 || mode == 2)
				profiler.countOperation(line, n);
			arr[index + 1] = arr[index];
		}

		if (mode == 1 || mode == 2)
			profiler.countOperation(line, n);
		arr[j] = current;
	}
}

void InsertionSort(int mode, int arr[], int n, const char* line = "insertion-sort") {
	// init line
	profiler.countOperation(line, n, 0);
	for (int i = 1; i < n; i++) {
		int current = arr[i];
		int j = i - 1;

		while (j >= 0 && arr[j] > current) {
			if (mode == 0 || mode == 2)
				profiler.countOperation(line, n, 1);
			if (mode == 1 || mode == 2)
				profiler.countOperation(line, n, 1);

			arr[j + 1] = arr[j];
			j--;
		}

		if (mode == 1 || mode == 2)
			profiler.countOperation(line, n);
		arr[j + 1] = current;
	}
}

bool TestAlgorithmsIfCorrect() {
	const int n = 7;
	int arrays[4][n] = {
		{9, 6, 3, 2, 5, 6, 8},
		{7, 6, 1, 2, 3, 8, 9},
		{1, 2, 6, 5, 3, 4, 4},
		{5, 4, 1, 2, 6, 7, 6}
	};

	profiler.disableCounters();

	BubbleSort(2, arrays[0], n);
	if (IsSorted(arrays[0], n) == false) {
		printf("Bubble Sort failed\n");
		return false;
	}

	InsertionSort(2, arrays[1], n);
	if (IsSorted(arrays[1], n) == false) {
		printf("Insertion Sort failed\n");
		return false;
	}

	SelectionSort(2, arrays[2], n);
	if (IsSorted(arrays[2], n) == false) {
		printf("Selection Sort failed\n");
		return false;
	}

	InsertionSortBinary(2, arrays[3], n);
	if (IsSorted(arrays[3], n) == false) {
		printf("Insertion Sort with upperd bound check failed\n");
		return false;
	}

	profiler.enableCounters();
	return true;
}

void BestCaseMapping(int mode, std::vector<int> sizes) {
	int bubbleArr[SIZE] = {};
	int insertionArr[SIZE] = {};
	int insertionArrB[SIZE] = {};
	int selectionArr[SIZE] = {};

	FillRandomArray(bubbleArr, SIZE, 10, 50000, false, 1);
	FillRandomArray(insertionArr, SIZE, 10, 50000, false, 1);
	FillRandomArray(insertionArrB, SIZE, 10, 50000, false, 1);
	FillRandomArray(selectionArr, SIZE, 10, 50000, false, 1);

	printf("Drawing chart to compare best runtimes for each sorting algorithm.\n");
	for (auto const& n : sizes) {
		BubbleSort(mode, bubbleArr, n);		 // n - 1 comparisons, no swaps
		InsertionSort(mode, insertionArr, n); // n - 1 comparisons
		InsertionSortBinary(mode, insertionArrB, n);
		SelectionSort(mode, selectionArr, n); // (n, 2) comparisons, no swaps
	}
}

/*
** @param m - number of measurements for each sorting algorithm
*/
void AverageCaseMapping(int mode, std::vector<int> sizes, int m = 5) {
	int** testArrays = (int**)malloc(sizeof(int*) * m);
	int dummyArray[SIZE];

	printf("Creating test arrays to test the average runtimes.\n");
	for (int i = 0; i < m; i++) {
		testArrays[i] = (int*)malloc(sizeof(int) * SIZE);
		FillRandomArray(testArrays[i], SIZE);
	}

	for (int i = 0; i < m; i++) {
		printf("\tCalculating sum for average on set no %d\n", i + 1);
		for (auto const& n : sizes) {
			CopyArray(testArrays[i], dummyArray, n);
			BubbleSort(mode, dummyArray, n, "bubble-sort-avg");

			CopyArray(testArrays[i], dummyArray, n);
			InsertionSort(mode, dummyArray, n, "insertion-sort-avg");

			CopyArray(testArrays[i], dummyArray, n);
			InsertionSortBinary(mode, dummyArray, n, "insertion-sort-b-avg");

			CopyArray(testArrays[i], dummyArray, n);
			SelectionSort(mode, testArrays[i], n, "selection-sort-avg");
		}
	}

	printf("Calculating the actual average for bubble-sort for each n.\n");
	for (auto const& n : sizes) {
		profiler.average("bubble-sort-avg", n, m);
		profiler.average("insertion-sort-avg", n, m);
		profiler.average("insertion-sort-b-avg", n, m);
		profiler.average("selection-sort-avg", n, m);
	}
}

void WorstCaseMapping(int mode, std::vector<int> sizes) {
	int testArray[SIZE] = {};

	printf("Drawing chart to compare worst runtimes for each sorting algorithm.\n");
	for (auto const& n : sizes) {
		FillRandomArray(testArray, n, 10, 50000, false, 2);
		BubbleSort(mode, testArray, n, "bubble-sort-worst");

		FillRandomArray(testArray, n, 10, 50000, false, 1);
		/*
		** Worst case for selection sort. (n, 2) comparisons, 3(n - 1) swaps
		** This occurs when we have to get the first element to the end of the array.
		** ie: a rotated sorted array by one position
		*/
		testArray[n - 1] = 9; // 9 is guaranteed to be the smallest value in the array
		SelectionSort(mode, testArray, n, "selection-sort-worst");

		FillRandomArray(testArray, n, 10, 50000, false, 2);
		InsertionSort(mode, testArray, n, "insertion-sort-worst");

		FillRandomArray(testArray, n, 10, 50000, false, 2);
		InsertionSortBinary(mode, testArray, n, "insertion-sort-b-worst");
	}

}

int main(void) {
	/*
	** TODO: test whether the sorting algorithms implementation are correct
	** Disable counters first
	*/

	/*
	** TODO: discuție stabilitate și adaptabilitate(în antet).
	*/

	const char* modes[] = {
		"comparisons", "assignments", "total"
	};
	int mode = 0;

	printf("Enter mode:\n");
	scanf(" %d", &mode);
	printf("Counting: %s\n", modes[mode]);
	printf("Algorithms are correct? %s\n", TestAlgorithmsIfCorrect() == true ? "y" : "n");

	std::vector<int> sizes;
	// Create sizes to be used on tests.
	for (int n = 10; n < 1000; n += (n < 100 ? 10 : 100))
		sizes.push_back(n);

	BestCaseMapping(mode, sizes);
	profiler.createGroup("best-case-1", "insertion-sort",
		"selection-sort", "bubble-sort", "insertion-sort-b");

	AverageCaseMapping(mode, sizes);
	profiler.createGroup("average-case-with-5-measurements",
		"bubble-sort-avg", "insertion-sort-avg", "insertion-sort-b-avg", "selection-sort-avg");

	WorstCaseMapping(mode, sizes);
	profiler.createGroup("worst-case", "bubble-sort-worst",
		"insertion-sort-worst", "selection-sort-worst", "insertion-sort-b-worst");

	profiler.changeTitle(modes[mode]);
	profiler.showReport();
	return 0;
}