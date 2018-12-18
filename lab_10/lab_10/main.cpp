/*
	Lab no: 10th
	Group: @30229
	Name: Marcus Pop
	Requirements:
	- Implement and analyze depth first search
	- Implement topological sort
	- Implement Tarjan's algorithm for Strongly Connected Components
*/

#include <stdio.h>
#include "../../Profiler.h"
#include "../../Miscellaneous.h"
#include "../../Traversal.h"
#include "../../Stack_t.h"
#include "../../DirectedGraph_t.h"

#define TEST_LOG 1

#define count_op(line, size, count) \
	if (line != NULL && size != 0) { \
		profiler.countOperation((line), (size), (count));  \
}

Profiler profiler;

void dfs_helper(DirectedGraph* graph, 
	int source, 
	int* current_time, 
	Traversal* traverse_data, 
	const char* line = NULL,
	int size = 0) {
	count_op(line, size, 1);
	if (TEST_LOG == 1)
		printf("%d ", source);

	traverse_data->colors[source] = GREY;
	traverse_data->times[source].discover = *current_time;
	*current_time = *current_time + 1;

	for (auto head = graph->edges[source]->first; head != NULL; head = head->next) {
		const int vertex = *head->key;

		count_op(line, size, 1);
		traverse_data->parents[source] = vertex;
		if (traverse_data->colors[vertex] == WHITE)
			dfs_helper(graph, vertex, current_time, traverse_data, line, size);
	}
	
	traverse_data->times[source].end = *current_time;
	traverse_data->colors[source] = BLACK;
}

// Traverses all nodes of a graph in depth first manner.
void dfs(DirectedGraph* graph, const char* line = NULL, int size = 0) {
	Traversal* traverse_data = new Traversal(graph->vertices);
	int time = 0;

	for (int vertex = 0; vertex < graph->vertices; vertex++) {
		if (traverse_data->colors[vertex] == WHITE)
			dfs_helper(graph, vertex, &time, traverse_data, line, size);
	}
	delete traverse_data;
}

bool topologic_sort_helper(DirectedGraph* graph,
	int source, 
	int* current_time,
	Traversal* traverse_data,
	Stack<int>* result) {

	traverse_data->colors[source] = GREY;
	traverse_data->times[source].discover = *current_time;
	*current_time = *current_time + 1;

	for (auto head = graph->edges[source]->first; head != NULL; head = head->next) {
		const int vertex = *head->key;

		traverse_data->parents[source] = vertex;
		// Check wether there is a cycle from this vertex.
		if (traverse_data->colors[vertex] == GREY)
			return false;
		else if (traverse_data->colors[vertex] == WHITE &&
			!topologic_sort_helper(graph, vertex, current_time, traverse_data, result))
			return false;
	}

	traverse_data->times[source].end = *current_time;
	traverse_data->colors[source] = BLACK;
	result->push(source);
	return true;
}

Stack<int>* topologic_sort(DirectedGraph* graph) {
	auto traverse_data = new Traversal(graph->vertices);
	auto result = new Stack<int>();
	int time = 0;

	for (int vertex = 0; vertex < graph->vertices; vertex++) {
		if (traverse_data->colors[vertex] == WHITE) {
			// Check for cycle.
			if (topologic_sort_helper(graph, vertex, &time, traverse_data, result) == false) {
				delete result;
				return NULL;
			}
		}
	}
	delete traverse_data;
	return result;
}

void endl() {	printf("\n"); }

void run_tests() {
	DirectedGraph* graph = new DirectedGraph(6);
	Stack<int>* order;

	graph->add_edge(0, 1);
	graph->add_edge(0, 2);
	graph->add_edge(0, 5);
	graph->add_edge(1, 4);
	graph->add_edge(2, 3);
	graph->add_edge(3, 1);
	graph->add_edge(3, 4);

	dfs(graph);
	endl();
	order = topologic_sort(graph);

	if (order) {
		while (!order->is_empty()) {
			printf("%d ", order->peek());
			order->pop();
		}
		endl();
	}
	else printf("Given graph contains a cycle.\n");
	delete graph;
}

DirectedGraph* build_graph(int vertices_count, int edges_count) {
	assert(edges_count < vertices_count * (vertices_count - 1) &&
		"Assert wether edges_count is valid.");

	DirectedGraph* graph = new DirectedGraph(vertices_count);
	Edge_t* edges = create_all_possible_edges(vertices_count);

	shuffle_array(edges, edges_count);
	for (int i = 0; i < edges_count; i++) {
		graph->add_edge(std::get<0>(edges[i]), std::get<1>(edges[i]));
	}

	delete edges;
	return graph;
}

// Fix vertices size. Vary the number of edges for each test.
void analysis_variable_edges() {
	int vertices = 100;
	const char* line = "dfs_e";

	for (int edges = 1000; edges <= 5000; edges += 100) {
		DirectedGraph* graph = build_graph(vertices, edges);

		dfs(graph, line, edges);
		delete graph;
	}
}

void analysis_variable_vertices() {
	int edges = 9000;
	const char* line = "dfs_v";

	for (int vertices = 100; vertices <= 200; vertices += 10) {
		printf("%d\r", vertices);
		DirectedGraph* graph = build_graph(vertices, edges);

		dfs(graph, line, vertices);
		delete graph;
	}
}

void tarjan_helper(DirectedGraph* graph, 
	int source,
	int* time,
	int* index,
	int* link,
	bool* on_stack,
	Stack<int>* stack) {

	// Link of source is the furthest back node we can reach from source.
	// Initially is set to the current vertex.
	index[source] = link[source] = *time;
	*time = *time + 1;
	stack->push(source);
	on_stack[source] = true;

	for (auto head = graph->edges[source]->first; head != NULL; head = head->next) {
		const int vertex = *head->key;

		// If vertex has not been yet visited added to the component
		if (index[vertex] == -1) {
			tarjan_helper(graph, vertex, time, index, link, on_stack, stack);
			// Update the link of the source node
			link[source] = min(link[source], link[vertex]);
		}
		else if (on_stack[vertex] == true) {
			// Vertex has been previously visited and is part of the current SCC from
			// which source belongs too. Update the link.

			// (source, vertex) forms a backwards edge
			link[source] = min(link[source], index[vertex]);
		}
	}

	// If source is the root of its SCC, i.e. its link equals its index
	// pop all the vertices from its component and use them(print in this case)
	if (link[source] == index[source]) {
		on_stack[source] = false;
		while (!stack->is_empty() && stack->peek() != source) {
			printf("%d ", stack->peek());
			on_stack[stack->peek()] = false;
			stack->pop();
		}
		printf("%d\n", source);
		stack->pop();
	}
}

void tarjan(DirectedGraph* graph) {
	int n = graph->vertices;
	int time = 0;
	auto index = new int[n];
	auto link = new int[n];
	auto on_stack  = new bool[n];
	auto stack = new Stack<int>();

	for (int i = 0; i < n; i++) {
		link[i] = index[i] = -1;
		on_stack[i] = false;
	}

	for (int vertex = 0; vertex < n; vertex++)
		if (index[vertex] == -1)
			tarjan_helper(graph, vertex, &time, index, link, on_stack, stack);

	delete index;
	delete link;
	delete on_stack;
	delete stack;
}

void test_tarjan() {
	DirectedGraph* graph = new DirectedGraph(8);

	graph->add_edge(0, 1);
	graph->add_edge(1, 2);
	graph->add_edge(2, 0);
	graph->add_edge(3, 1);
	graph->add_edge(3, 2);
	graph->add_edge(3, 4);
	graph->add_edge(4, 3);
	graph->add_edge(4, 5);
	graph->add_edge(5, 2);
	graph->add_edge(5, 6);
	graph->add_edge(6, 5);
	graph->add_edge(7, 6);
	graph->add_edge(7, 4);
										
	tarjan(graph);
										
	delete graph;
}

int main(void) {
	run_tests();
	test_tarjan();
	//analysis_variable_vertices();
	//analysis_variable_edges();
	//profiler.showReport();
	return 0;
}