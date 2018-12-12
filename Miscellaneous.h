#ifndef MISCELLANEOUS
# define MISCELLANEOUS

#include <cassert>
#include <tuple>

// @returns and integer in [a, b)
int random_int(int a, int b) {
	return rand() % (b - a) + a;
}

// Shuffles a given array
template <typename T>
void shuffle_array(T* array, int size) {
	assert(size > 0 && "Size is greater than 0.");

	for (int i = 0; i < size; i++) {
		int next = random_int(i, size);
		T temp = array[next];

		array[next] = array[i];
		array[i] = temp;

	}
}

typedef std::tuple<int, int> Edge_t;

// Given a number of vertices of a directed graph
// return all the possible edges.
Edge_t* create_all_possible_edges(int vertices) {
	int size = (vertices * (vertices - 1)); // (vertices, 2) * 2
	auto edges = new Edge_t[size]; 
	int index = 0;

	for (int first = 0; first < vertices - 1; first++) {
		for (int second = first + 1; second < vertices; second++) {
			edges[index++] = std::make_tuple(first, second);
			edges[index++] = std::make_tuple(second, first);
		}
	}
	return edges;
}

#endif