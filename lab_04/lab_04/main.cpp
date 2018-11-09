/*
	Lab no 4
	Group: @30229
	Name: Marcus Pop
	Tasks:
		- Merge k sorted lists that have in total n elements in O(nlogk) time.
		- Create an algorithm to merge two lists.
		- Analyze the runtime complexity of the first by creating random input
		and tracing the necessary graphs.
*/

#include <stdio.h>
#include "../../Heap_t.h"

template<typename T>
struct LinkedListNode {
	T* key;
	LinkedListNode* next;

	LinkedListNode(T* key) {
		this->key = key;
		this->next = NULL;
	}

	~LinkedListNode() {
		delete key;
	}
};

template<typename T>
class LinkedList {
public:
	LinkedListNode<T> *last, *first;

	LinkedList() {
		this->first = this->last = NULL;
		this->_size = 0;
	}

	LinkedList(LinkedListNode<T>* start) {
		last = first = start;
		_size = 0;

		while (start) {
			_size++;
			last = start;
			start = start->next;
		}
		last->next = NULL;
	}

	~LinkedList() {
		LinkedListNode<T>* head = this->first;

		while (head) {
			LinkedListNode<T>* current = head;

			head = head->next;
			delete current;
		}

		first = last = NULL;
	}

	void print(void(*print_node)(T)) {
		LinkedListNode<T>* head = this->first;

		while (head) {
			print_node(*head->key);
			head = head->next;
		}
		printf("\n");
	}

	void insert_last(T key) {
		LinkedListNode<T>* new_node = new LinkedListNode<T>(new T(key));

		if (last != NULL)
			last->next = new_node;
		last = new_node;
		if (first == NULL)
			first = new_node;
		_size++;
	}

	void insert_last(LinkedListNode<T>* new_node) {
		if (last != NULL)
			last->next = new_node;
		last = new_node;
		if (first == NULL)
			first = new_node;
		_size++;
	}

	void insert_multiple_last(T* args, int size) {
		for (int i = 0; i < size; i++)
			this->insert_last(args[i]);
	}

	uint32_t size() { return this->_size;  }

private:
	uint32_t _size = 0;
};

// Compare function for a min heap of linked list nodes.
bool cmp(LinkedListNode<int>* parent, LinkedListNode<int>* child) {
	return *(parent->key) < *(child->key);
}

void print_node(int key) {
	printf("%d ", key);
}

int random_int(int lo, int hi) {
	if (lo == hi)
		return lo;
	return rand() % (hi - lo) + lo;
}

// Generates a set of k numbers that together have sum n.
// @param intergers = true => apply floor() to each number in the set
// before returning it
double* generate_k_sum_n(int n, int k, bool return_integers = false, int lo = 10, int hi = 1000) {
	double* result = new double[k];
	double  sum = 0;

	for (int i = 0; i < k; i++) {
		result[i] = random_int(lo, hi);
		sum = sum + result[i];
	}

	for (int i = 0; i < k; i++)
		result[i] = result[i] / sum * n;

	if (return_integers) {
		int final_sum = 0;
		for (int i = 0; i < k; i++) {
			result[i] = (int)result[i];
			final_sum = final_sum + result[i];
		}

		if (sum != final_sum) {
			// distribute what was lost during 
			// flooring to elements in the array
			int missing = n - final_sum;

			for (int i = 0; i < missing; i++)
				result[i] += 1;
		}
	}

	//for (int i = 0; i < k; i++)
	//	printf("%llf ", result[i]);
	//printf("\n");

	return result;
}

// Create k lists with n elements in total.
// Condition: n >= k
// It is sure that each list will have at least one element.
LinkedList<int>** randomized_lists(int n, int k) {
	int* sizes = new int[k];
	int index = 0;
	LinkedList<int>** result = new LinkedList<int>*[k];

	// Make sure each list has at least size 1.
	for (int i = 0; i < k; i++)
		sizes[i] = 1;
	double* additions = generate_k_sum_n(n - k, k, true, 10, 10000);
	for (int i = 0; i < k; i++)
		sizes[i] += (int)additions[i];
	delete[] additions;
	/*
	Old way of generating a set of k elements with sum n. Not so good tho

		while (n > 0 && index < k - 1) {
			int scalar = random_int(0, n + 1);

			if (scalar > n / 3) {
				// if bigger than a third of remaning n divide it for a better dispersion
				scalar = n / 3;
			}

			n -= scalar;
			sizes[index] += scalar; // increment size at position index and go the next size
			index++;
			}
			// add the last number of elements remaining to be given to the last list
			sizes[k - 1] += n;
	*/


	// create the lists
	for (int i = 0; i < k; i++) {
		int size = sizes[i];
		int* container = new int[size];

		result[i] = new LinkedList<int>();
		FillRandomArray(container, size, 10, 1000, false, 1);
		for (int index = 0; index < size; index++)
			result[i]->insert_last(container[index]);

		delete[] container;
	}

	//printf("Printing the lists:\n");
	//for (index = 0; index < k; index++)
	//	result[index].print(print_node);

	delete[] sizes;
	return result;
}

LinkedListNode<int>* merge_two_lists(LinkedListNode<int>* a, LinkedListNode<int>* b) {
	if (!a)
		return b;
	if (!b)
		return a;

	LinkedListNode<int>* next = NULL;
	if (*a->key < *b->key) {
		next = a->next;
		a->next = merge_two_lists(next, b);
		return a;
	}

	next = b->next;
	b->next = merge_two_lists(a, next);
	return b;
}

void p(LinkedListNode<int>* node) {
	printf("%d ", *node->key);
}

LinkedList<int>* merge_k_lists(LinkedList<int>** lists, int k, const char* line = NULL, uint32_t size = 0) {
	auto result = new LinkedList<int>();
	auto priority_queue = new Heap<LinkedListNode<int>*>(cmp);

	// insert the first element from each list into the priority queue
	for (int index = 0; index < k; index++)
		priority_queue->push(lists[index]->first, line, size);

	while (priority_queue->isEmpty() == false) {	
		LinkedListNode<int>* top = *(priority_queue->pop());

		result->insert_last(top); // O(1) - no sense in counting it
		if (top->next != NULL)
			priority_queue->push(top->next, line, size);
	}

	return result;
}

// @max_n maximum number of elements in the k lists
void first_analysis(int max_n = 10000, int increment = 100) {
	int lists_numbers[] = { 5, 10, 100 };
	int lists_count = sizeof(lists_numbers) / sizeof(int);

	for (int i = 0; i < lists_count; i++) {
		int k = lists_numbers[i];
		char* line = new char[5];
		
		printf("\nFirst analysis step: %d\n", i + 1);
		sprintf(line, "k%d", (i + 1));
		for (int n = 100; n <= max_n; n += increment) {
			LinkedList<int>** lists = randomized_lists(n, k);

			printf("Current n: %d\r", n);
			LinkedList<int>* result = merge_k_lists(lists, k, line, n);

			delete result;
			delete lists;
		}
	}

	printf("\n");
	profiler.createGroup("k-constant", "k1", "k2", "k3");
}

void second_analysis(int n = 10000) {
	for (int k = 10; k <= 500; k += 10) {
		LinkedList<int>** lists = randomized_lists(n, k);

		printf("Second analysis current k: %d\r", k);
		LinkedList<int>* result = merge_k_lists(lists, k, "rand-k", k);

		delete result;
		delete lists;
	}
}

int main(void) {
	int k = 10;
	int n = 100;
	
	LinkedList<int>* a = new LinkedList<int>();
	LinkedList<int>* b = new LinkedList<int>();
	int arr_a[] = { 6, 7, 10, 45, 67 };
	int arr_b[] = { 14, 17, 19, 97 };

	a->insert_multiple_last(arr_a, 5);
	b->insert_multiple_last(arr_b, 4);

	LinkedList<int>* merged = new LinkedList<int>(merge_two_lists(a->first, b->first));
	merged->print(print_node);

	delete merged;

	LinkedList<int>** lists = randomized_lists(n, k);
	printf("Printing lists to be merged:\n");
	for (int i = 0; i < k; i++)
		lists[i]->print(print_node);

	LinkedList<int>* merged_list = merge_k_lists(lists, k);
	printf("Merge result:\n");
	merged_list->print(print_node);

	delete merged_list;
	delete[] lists;

	first_analysis();
	second_analysis();

	profiler.changeTitle("Merge K sorted lists.");
	profiler.showReport();
	return 0;
}