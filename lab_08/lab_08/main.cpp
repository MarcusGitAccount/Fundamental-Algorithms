/*
	Lb no: 08
	Group: @30229
	Name: Marcus Pop
	Requirement:
		- Implement a data structure to operate on disjoint sets with the following operations:
			make_set, union, find_set
		- Implement Kruskal's algorithm
		- Test Kruskal's algorithms on graphs with a varbiable number of vertices.
*/

#include <stdio.h>
#include <vector>
#include <tuple>
#include "../../Profiler.h"
#include "../../QuickSort.h"
#include "../../DisjointSets_t.h"

// (inbound, outbound, weight)
typedef std::tuple<int, int, int> Edge_t;

int cmp(Edge_t first, Edge_t second) {
	return std::get<2>(first) - std::get<2>(second);
}

void print_edge(Edge_t edge) {
	printf("%d %d %d\n", std::get<0>(edge), std::get<1>(edge), std::get<2>(edge));
}

// Implements Kruskal's minimum spanning tree algorithm
// @param n - number of vertices of an undirected graph labeled from 0 to n - 1
// @param edges - array of tuples(inbound vertex, outbound vertex, weight)
// @returns the total cost of the MST created
int kruskal(int n, Edge_t* edges) {
	SetNode<int>** vertices = new SetNode<int>*[n];
	int cost = 0;

	for (int i = 0; i < n; i++)
		vertices[i] = DisjointSet<int>::make_set(i);
	
	// TODO: implement counting/bucket sort when the cost function
	// is mapping to values between [0, n) => O(E) complexity for sorting
	quicksort(edges, 0, n - 1, cmp);
	for (int edge = 0; edge < n; edge++) {
		int inbound	 = std::get<0>(edges[edge]);
		int outbound = std::get<1>(edges[edge]);
		int weight	 = std::get<2>(edges[edge]);

		if (DisjointSet<int>::find_set(vertices[inbound], n) != 
			  DisjointSet<int>::find_set(vertices[outbound], n)) {

			// printf("Adding: %d - %d\n", inbound, outbound);
			cost += weight;
			DisjointSet<int>::union_sets(vertices[inbound], vertices[outbound], n);
		}
	}

	return cost;
}

void analysis() {
	for (int n = 100; n <= 10000; n += 100) {
		std::vector<Edge_t> edges;

		printf("Current n: %d\r", n);
		for (int vertex = 0; vertex < n; vertex++) {
			// make cycles of 4 edges
			if (vertex + 1 < n)
				edges.push_back(std::make_tuple(vertex, vertex + 1, rand() % n));
			if (vertex % 4 == 3)
				edges.push_back(std::make_tuple(vertex, vertex - 3, rand() % n));
		}

		Edge_t* edges_pointer = edges.data();

		kruskal(n, edges_pointer);
	}

	return;
}

int main(void) {
	//Edge_t edges[] = {
	//	std::make_tuple(0, 1, 10),
	//	std::make_tuple(0, 2, 1),
	//	std::make_tuple(1, 3, 4),
	//	std::make_tuple(1, 6, 3),
	//	std::make_tuple(2, 3, 2),
	//	std::make_tuple(2, 4, 4),
	//	std::make_tuple(3, 4, 5),
	//	std::make_tuple(3, 5, 8),
	//	std::make_tuple(3, 6, 2)
	//};
	//int n = sizeof(edges) / sizeof(Edge_t);
	//int cost = kruskal(n, edges); // should return 20

	//printf("Minimum spanning tree's final cost: %d\n", cost);

	analysis();
	profiler.showReport();
	return 0;
}