#ifndef DISJOINTS_SETS_H
# define DISJOINTS_SETS_H

# include "Profiler.h"

Profiler profiler("kruskal");

template <typename T>
struct SetNode {
	T key;
	int rank;
	SetNode<T>* parent;

	SetNode(T key, int rank = 0, SetNode<T>* parent = NULL) {
		this->key  = key;
		this->rank = rank;
		this->parent = parent;
	}
};

template <typename T>
struct DisjointSet {
	DisjointSet() {}

	static SetNode<T>* make_set(int key) {
		SetNode<T>* node = new SetNode<T>(key);

		node->parent = node;
		return node;
	}

	// adding extra parameters to be used in tracing line charts
	static SetNode<T>* find_set(SetNode<T>* node, int n) {
		profiler.countOperation("op_count", n, 1);
		if (node->parent == node)
			return node;

		// Path compression i.e. link the current node to its set(root)
		// so further searches for this node will execute in O(1) time
		SetNode<T>* root = find_set(node->parent, n);

		profiler.countOperation("op_count", n, 1);
		node->parent = root;
		return root;
	}

	static SetNode<T>* union_sets(SetNode<T>* first, SetNode<T>* second, int n) {
		return link_sets(
			find_set(first, n),
			find_set(second, n),
			n
		);
	}

private:
	// Link one set to the other one, return it and increse its rank if needed
	static SetNode<T>* link_sets(SetNode<T>* first, SetNode<T>* second, int n) {
		profiler.countOperation("op_count", n, 1);
		if (first->rank < second->rank) {
			first->parent = second;
			return second;
		}
			
		second->parent = first;
		if (first->rank == second->rank)
			first->rank++;
		
		return first;
	}
};

#endif
