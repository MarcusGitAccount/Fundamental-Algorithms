#include <stdio.h>
#include "../../Profiler.h"
#include "../../DirectedGraph_t.h"
#include "../../Queue_t.h"

// #define TEST_ALGORITHMS

#define count_op(line, size, count) \
	if (line != NULL && size != 0) { \
		profiler.countOperation((line), (size), (count));  \
}

typedef enum {WHITE, GREY, BLACK} Colors_t;

Profiler profiler("bfs");

// [a, b]
int random_int(int a, int b) {
	return a +  1 + (int)((float)b * (rand() / (RAND_MAX + 1.0)));
}

void bfs_helper(DirectedGraph* graph, 
				int start, 
				int* parents,
				Colors_t* colors,
				const char* line = NULL, 
				int size = 0) {

	assert(colors[start] == WHITE && "Assert starting vertex has not been visited.");
	auto queue = new Queue<int>();

	count_op(line, size, 1); // queue op
	colors[start] = GREY;
	queue->enqueue(start);
	while (queue->size() != 0) {
		const int current = queue->peek();
		auto head = graph->edges[current]->first;

		colors[current] = BLACK;
		queue->dequeue();
		count_op(line, size, 2); // queue ops
		while (head != NULL) {
			const int vertex = *head->key;

			count_op(line, size, 1); // edge traversing and comparison
			if (colors[vertex] == WHITE) {
				count_op(line, size, 1); // queue op
				colors[vertex] = GREY;
				parents[vertex] = current;
				queue->enqueue(vertex);
			}

			head = head->next;
		}
	}

	delete queue;
}

void bfs(DirectedGraph* graph, const char* line = NULL, int size = 0) {
	auto parents = new int[graph->vertices];
	auto colors = new Colors_t[graph->vertices];
	
	//printf("For %s %d\n", line, size);
	for (int i = 0; i < graph->vertices; i++) {
		parents[i] = -1;
		colors[i] = WHITE;
	}

	for (int vertex = 0; vertex < graph->vertices; vertex++)
		if (colors[vertex] == WHITE)
			bfs_helper(graph, vertex, parents, colors, line, size);

	#ifdef TEST_ALGORITHMS
		printf("Printing vertices' parents: ");
		for (int i = 0; i < graph->vertices; i++) {
			printf("%d ", parents[i]);
		}
		printf("\n");
	#endif // TEST_ALGORITHMS


}

void test_queue() {
	auto queue = new Queue<int>();

	for (int i = 0; i < 10; i++)
		queue->enqueue(i);
	for (int i = 0; i < 10; i++) {
		printf("%d ", queue->peek());
		queue->dequeue();
	}

	delete queue;
	printf("\nHello world!\n");
}

DirectedGraph* build_graph(int vertices, int edges);
void test_graph() {
	// auto graph = build_graph(100, 1000);
	DirectedGraph* graph = new DirectedGraph(11);
	// int expected_parents[] = { -1, 0, 1, 2, 1, 2, 4, -1, 4, 8, 9 };

	graph->add_edge(0, 1);
	graph->add_edge(1, 2);
	graph->add_edge(1, 4);
	graph->add_edge(2, 3);
	graph->add_edge(2, 5);
	graph->add_edge(4, 2);
	graph->add_edge(4, 6);
	graph->add_edge(4, 8);
	graph->add_edge(5, 3);
	graph->add_edge(6, 3);
	graph->add_edge(6, 5);
	graph->add_edge(8, 9);
	graph->add_edge(9, 4);
	graph->add_edge(9, 10);
	graph->add_edge(10, 6);


	bfs(graph);
	delete graph;
	#undef TEST_ALGORITHMS
}

DirectedGraph* build_graph(int vertices, int edges) {
	DirectedGraph* graph = new DirectedGraph(vertices);

	for (int e = 0; e < edges; e++) {
		int a = random_int(0, vertices - 1);
		int b;

		do {
			b = a;
			while (a == b)
				b = random_int(0, vertices - 1);
		} while (graph->is_edge(a, b));

		graph->add_edge(a, b);
	}

	return graph;
}

// Fix vertices size. Vary the number of edges for each test.
void analysis_variable_edges() {
	int vertices = 100;
	const char* line = "bfs_e";

	for (int edges = 1000; edges <= 5000; edges += 100) {
		DirectedGraph* graph = build_graph(vertices, edges);

	
		bfs(graph, line, edges);
		delete graph;
	}
}

void analysis_variable_vertices() {
	int edges = 7000;
	const char* line = "bfs_v";

	for (int vertices = 100; vertices <= 200; vertices += 10) {
		printf("%d\r", vertices);
		DirectedGraph* graph = build_graph(vertices, edges);
		
		bfs(graph, line, vertices);
		delete graph;
	}
}


int main(void) {
	test_queue();
	test_graph();
	analysis_variable_edges();
	analysis_variable_vertices();
	profiler.showReport();
	return 0;
}