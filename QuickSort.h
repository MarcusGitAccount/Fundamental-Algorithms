#ifndef QUICKSORT_T
# define QUICKSORT_T

template <typename T>
int partition(T* array, int lo, int hi, int (*compare)(T, T)) {
	int last = lo - 1;
	int index = lo;
	int pivot = hi;

	while (index <= hi) {
		if (compare(array[index], array[pivot]) < 0) {
			last++;
			swap_(&array[last], &array[index]);
		}
		index++;
	}

	last++;
	swap_(&array[last], &array[pivot]);
	return last;
}

template <typename T>
void quicksort(T* array, int lo, int hi, int (*compare)(T, T)) {
	if (lo >= hi)
		return;

	const int pivot = partition(array, lo, hi, compare);
	quicksort(array, lo, pivot - 1, compare);
	quicksort(array, pivot + 1, hi, compare);
}

#endif // !QUICKSORT_T
