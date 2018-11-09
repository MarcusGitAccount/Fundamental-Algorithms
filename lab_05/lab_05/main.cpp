/*
	Lab: no 5
	Name: Marcus Pop
	Group: @30229
	Task: Implement a hash set using an open addressing strategy and
	quadratic probing. Perform the required measurements(number of 
	buckets accesses during search) for different load factors(alpha).
*/

#include <stdio.h>
#include <cassert>
#include "../../Profiler.h"
#include "../../Vector_t.h"

int modular_add(int a, int b, int m) {
	assert(m != 0);
	return ((a % m) + (b % m)) % m;
}

int modular_add(int a, int b, int c, int m) {
	assert(m != 0);
	return modular_add(modular_add(a, b, m), c, m);
}

int modular_mul(int a, int b, int m) {
	assert(m != 0);
	return ((a % m) * (b % m)) % m;
}

int modular_mul(int a, int b, int c, int m) {
	assert(m != 0);
	return modular_mul(modular_mul(a, b, m), c, m);
}

template<typename T>
class HashSet {
	typedef enum {FREE, USED} CellStatus_t;

public:
	HashSet(uint32_t (*hash)(T, int), uint32_t size) {
		this->hash = hash;
		this->size = size;
		this->bucket = new T[size];
		this->status = new CellStatus_t[size];

		for (uint32_t i = 0; i < size; i++)
			this->status[i] = FREE;
	}

	~HashSet() {
		delete[] this->bucket;
		delete[] this->status;
	}

	bool insert(T key) {
		// tries <=> number of collisions
		uint32_t tries = 0;
		do {
			uint32_t index = this->probing(key, tries, this->size);

			if (this->status[index] == FREE) {
				this->bucket[index] = key;
				this->status[index] = USED;
				return true;
			}
			if (this->bucket[index] == key)
				return false;

			tries++;

		} while (tries < this->size);

		return false;
	}

	bool find(T key, uint32_t* counter = NULL) {
		uint32_t tries = 0;
		uint32_t index;

		do {
			if (counter != NULL)
				*counter = *counter + 1;

			index = this->probing(key, tries, this->size);

			if (this->bucket[index] == key)
				return true;
			tries++;
		} while (tries < this->size && this->status[index] != FREE);

		return false;
	}


private:
	uint32_t size;
	uint32_t c1 = 7;
	uint32_t c2 = 31;
	uint32_t (*hash)(T, int);
	T* bucket;
	CellStatus_t* status;

	// (hash + c1 * i + c2 * i * i) mod m
	uint32_t probing(T key, uint32_t collision, int m) {
		uint32_t initial = this->hash(key, m);
		uint32_t i = collision;
		uint32_t hash = (uint32_t)(
			modular_add(
				this->hash(key, m),
				modular_mul(this->c1, i, m),
				modular_mul(this->c2, i, i, m),
				m
			)
		);

		return hash;
	}
};

/*
	Modular arithmetic:

	c mod m = (a * b) mod m
	=> (a * b) mod m = [(a mod m) * (b mod m)] mod m
	c mod m = (a + b) mod m
	=> (a + b) mod c = (a mod c + b mod c) mod c 
		= add(a, b, m)

	d mod m = (a + b + c) mod m
	=> (a + b + c) mod m = ((a + b) + c) mod m
		= ((a + b) mod m + c mod m) mod m
		= ((a mod m + b mod m) mod m + c mod m)) mod m
		= add(add(a, b, m), c, m)
*/

// Computes hash(key) % m
uint32_t hash_str(const char* str, int m) {
	uint32_t hash = 0;
	uint32_t c = 1;

	// modular arithmetic to prevent overflow
	while (*str != NULL) {
		uint32_t term = modular_mul(*str, c, m);

		hash = modular_add(hash, term, m);
		c = modular_mul(c, 3, m);
		str++;
	}
	return hash;
}

uint32_t hash_int(int key, int m) {
	return key % m;
}

void mod_arith_tests() {
	printf("%u\n", hash_str("Ana are mere", 31));
	printf("%u\n", hash_str("Ana are", 31));

	printf("mod mul: %u\n", modular_mul(4, 5, 6, 5));
	printf("mod add: %u\n", modular_add(4, 5, 6, 5));

	printf("%d\n", std::string("abc") == std::string("abc"));
}

void set_tests() {
	auto set = new HashSet<int>(hash_int, 31);

	printf("Was inserted? %d\n", set->insert(9));
	printf("Was inserted? %d\n", set->insert(6));
	printf("Was inserted? %d\n", set->insert(14));
	printf("Was inserted? %d\n", set->insert(67));

	printf("Does exist? %d\n", set->find(9));
	printf("Does exist? %d\n", set->find(6));
	printf("Does exist? %d\n", set->find(14));
	printf("Does exist? %d\n", set->find(67));

	printf("Does exist? %d\n", set->find(12));
	printf("Does exist? %d\n", set->find(15));
	printf("Does exist? %d\n", set->find(1));

	delete set;
}

// Return a random integer in the interval [a, b)
int random_int(int a, int b) {
	if (a == b)
		return a;
	return rand() % (b - a) + a;
}

// Randomize a given array so that all size! permutations
// can occur, O(n) time complexity and O(1) extra space
template <typename T>
void randomize_array(T* array, uint32_t size) {
	for (uint32_t i = 0; i < size; i++) {
		uint32_t next = random_int(i, size);

		swap_(&array[i], &array[next]);
	}
}

void analysis() {
	const char* file_name = "results.csv";
	FILE* fd = fopen(file_name, "w");

	if (fd == NULL)
		throw "File no opened.";

	const char* header =
		"Load factor(alpha)," \
		"Avg found,"\
		"Max found," \
		"Avg nfound," \
		"Max nfound\n";

	fprintf(fd, header);

	const int upper_limit = 100000;
	const float alphas[] = {.8f, .85f, .9f, .95f, .99f};
	const int alphas_count = sizeof(alphas) / sizeof(float);
	const int size = 9973;
	const int search_size = 3000;

	// search_size:search search_size / 2 elements that exist in the HashSet
	// and search_size / 2 elements that do not exist

	int* elements = new int[size];
	int* not_found_elements = new int[search_size / 2];

	FillRandomArray(elements, size, 1, upper_limit, true);
	FillRandomArray(not_found_elements, search_size / 2, upper_limit + 1, upper_limit * 2, true);

	for (int i = 0; i < alphas_count; i++) {
		const float alpha = alphas[i];
		const int count = (int)(alpha * size);
		
		int avg_found = 0;
		int max_found = 0;
		int avg_not_found = 0;
		int max_not_found = 0;

		randomize_array(elements, size);

		printf("Analysis for alpha = %.2f\n", alpha);
		printf("Current count of elements inserted: %d\n", count);

		auto set = new HashSet<int>(hash_int, size);

		for (int j = 0; j < count; j++) {
			printf("Inserting key no: %d\r", j);
			if (set->insert(elements[j]) == false)
				printf("Insert failed at %d.%d\n", i, j);
		}

		// randomize elements to be searched in order to give 
		// each of them an equal chance to be selected

		randomize_array(elements,						search_size / 2);
		randomize_array(not_found_elements, search_size / 2);

		int found_size = 0;
		int not_found_size = 0;

		printf("Searching existing elements.\n");
		for (int i = 0; i < search_size / 2; i++) {
			uint32_t operations = 0;

			if (set->find(elements[i], &operations) == true) {
				avg_found += operations;
				max_found = max(max_found, operations);
				found_size += 1;
			}
		}

		printf("Found elements: %d\n", found_size);
		printf("Searching non-existing elements.\n");
		for (int i = 0; i < search_size / 2; i++) {
			uint32_t operations = 0;

			if (set->find(not_found_elements[i], &operations) == false) {
				avg_not_found += operations;
				max_not_found = max(max_not_found, operations);
				not_found_size += 1;
			}
		}
		printf("Not found elements: %d\n", not_found_size);

		fprintf(fd, "%f,%.2f,%d,%.2f,%d,\n", alpha, avg_found / (float)found_size, max_found, avg_not_found / (float)not_found_size, max_not_found);
		printf("\n\n");
		delete set;
	}

	ShellExecuteA(NULL, "open", file_name, NULL, NULL, SW_SHOW);
	fclose(fd);
	delete[] elements;
	delete[] not_found_elements;
}

int main(void) {
	analysis();
	return 0;
}