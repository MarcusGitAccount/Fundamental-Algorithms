#ifndef DISJOINTS_SETS_H
# define DISJOINTS_SETS_H

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

	static SetNode<T>* find_set(SetNode<T>* node) {
		if (node->parent == node)
			return node;

		// Path compression i.e. link the current node to its set(root)
		// so further searches for this node will execute in O(1) time
		SetNode<T>* root = find_set(node->parent);

		node->parent = root;
		return root;
	}

	static SetNode<T>* union_sets(SetNode<T>* first, SetNode<T>* second) {
		return link_sets(
			find_set(first),
			find_set(second)
		);
	}

private:
	// Link one set to the other one, return it and increse its rank if needed
	static SetNode<T>* link_sets(SetNode<T>* first, SetNode<T>* second) {
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
