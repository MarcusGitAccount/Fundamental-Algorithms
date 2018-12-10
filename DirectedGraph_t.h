#ifndef DIRECTED_GRAPH_T
# define DIRECTED_GRAPH_T

# include "LinkedList_t.h"
# include "Vector_t.h"
# include <cassert>

struct DirectedGraph {
	int vertices;
	Vector_t<LinkedList<int>*> edges;

	DirectedGraph(int n) {
		this->vertices = n;
		this->edges = Vector_t<LinkedList<int>*>(n);

		for (int i = 0; i < n; i++)
			this->edges[i] = new LinkedList<int>();
	}

	~DirectedGraph() {
		for (int i = 0; i < this->vertices; i++)
			delete this->edges[i];
	}

	void assert_is_vertex(int vertex) {
		assert(vertex >= 0 && vertex < this->vertices && "Assert vertex exists.");
	}

	void add_edge(int a, int b) {
		assert_is_vertex(a);
		assert_is_vertex(b);

		this->edges[a]->insert_last(b);
	}

	bool is_edge(int a, int b) {
		assert_is_vertex(a);
		assert_is_vertex(b);

		auto head = edges[a]->first;

		while (head != NULL) {
			if (*head->key == b)
				return true;
			head = head->next;
		}

		return false;
	}
};

#endif