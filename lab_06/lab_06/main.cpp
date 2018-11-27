#include <stdio.h>
#include <cassert>
#include "../../Profiler.h"

Profiler profiler("os stats");

struct TreeNode {
	int key;
	int size;
	TreeNode *left, *right;
	TreeNode *parent;

	TreeNode(int key, TreeNode* parent = NULL, int size = 0) {
		this->key = key;
		this->size = size;
		this->parent = parent;
	}
};

void inorder(TreeNode* root) {
	if (root) {
		inorder(root->left);
		printf("%d ", root->key);
		inorder(root->right);
	}
}

TreeNode* tree_from_array(int* arr, int lo, int hi) {
	if (lo > hi)
		return NULL;

	int mid = lo + ((hi - lo) >> 1);
	int size = 0;
	TreeNode* root = new TreeNode(arr[mid]);

	root->left  = tree_from_array(arr, lo, mid - 1);
	root->right = tree_from_array(arr, mid + 1, hi);

	if (root->left) {
		size += root->left->size;
		root->left->parent = root;
	}
	if (root->right) {
		size += root->right->size;
		root->right->parent = root;
	}
	root->size = size + 1;
	return root;
}

// O(logn)
TreeNode* os_select(TreeNode* root, int k, int n = 0) {
	profiler.countOperation("select", n, 2);

	if (root == NULL)
		return root;

	int left_size = 0;
	if (root->left)
		left_size = root->left->size;

	int position = left_size + 1;

	if (k == position)
		return root;
	if (k < position)
		return os_select(root->left, k, n);
	return os_select(root->right, k - position, n);
}

// replace the subtree denoted by a with b
void transplant(TreeNode** root, TreeNode* a, TreeNode* b, int n = 0) {
	profiler.countOperation("delete", n, 2);

	if (*root == a)
		*root = b;
	else if (a->parent->left == a)
		a->parent->left = b;
	else
		a->parent->right = b;

	if (b != NULL)
		b->parent = a->parent;
}

void os_delete(TreeNode** root, TreeNode* node, int n = 0) {
	if (node == NULL)
		return;

	// update sizes
	if (node->parent) {
		TreeNode* parent = node->parent;

		while (parent) {
			parent->size -= 1;
			parent = parent->parent;
		}
	}

	// if node has no successor in its right subtree =>
	// we are dealing with the case in which node has 
	// only one child
	if (node->right == NULL)
		transplant(root, node, node->left, n);
	else if (node->left == NULL)
		transplant(root, node, node->right, n);
	else {
		// find the succesor of the node
		TreeNode* successor = node->right;

		while (successor->left != NULL) {
			profiler.countOperation("delete", n, 1);
			successor->size--;
			successor = successor->left;
		}

		// re-root the successor's right subtree
		// if the successor is actually the node's right child
		// its right subtree remains in place
		// if not is transplanted to the successor's parent

		if (successor->parent != node) {
			transplant(root, successor, successor->right, n);
			profiler.countOperation("delete", n, 2);
			successor->right = node->right;
			successor->right->parent = successor;
		}

		profiler.countOperation("delete", n, 2);
		transplant(root, node, successor, n);
		successor->left = node->left;
		successor->left->parent = successor;

		int size = 0;

		if (successor->left)
			size += successor->left->size;
		if (successor->right)
			size += successor->right->size;

		successor->size = size;
	}
		
	delete node;
}

void print_node_value(TreeNode* node) {
	if (!node) {
		printf("null");
		return;
	}
	printf("%d ", node->key);
}

// Basically, a fancy inorder
void pretty_print_tree(TreeNode* root, int indent = 0, int increment = 1) {
	if (root) {
		pretty_print_tree(root->right, indent + increment, increment);
		
		for (int i = 0; i < indent; i++)
			printf(" ");
		printf("%5d\n\n", root->key);
		
		pretty_print_tree(root->left, indent + increment, increment);
	}
}

void analysis(int measurements = 5) {
	for (int n = 100; n <= 10000; n += 100) {
		int* arr = new int[n];

		printf("Current test size: %d\r", n);
		for (int m = 0; m < 5; m++) {
			FillRandomArray(arr, n, 10, 50000, true, 1);
			TreeNode* root = tree_from_array(arr, 0, n - 1);

			for (int i = 0; i < n; i++) {
				if (root == NULL)
					break;

				int position = rand() % (root->size + 1);
				TreeNode* node = os_select(root, position, n);

				os_delete(&root, node, n);
			}
		}

		profiler.average("select", n, measurements);
		profiler.average("delete", n, measurements);
		delete[] arr;
	}

	printf("\n");
	return;
}

int main(void) {
	int arr[] = { 1, 5, 7, 12, 56, 90, 101, 201, 300 };
	int size = sizeof(arr) / sizeof(int);
	TreeNode* root = tree_from_array(arr, 0, size - 1);
	int* deleted = new int[3];

	profiler.disableCounters();
	FillRandomArray(deleted, 3, 1, size - 3, true);
	printf("%d %d %d\n", deleted[0], deleted[1], deleted[2]);
	printf("Before deleting 3 elements:\n\n");
	pretty_print_tree(root, 0, 6);
	for (int i = 0; i < 3; i++) {
		TreeNode* node = os_select(root, deleted[i]);

		os_delete(&root, node);
	}

	printf("After deleting 3 elements:\n\n");
	pretty_print_tree(root, 0, 6);

	profiler.enableCounters();
	analysis(5);
	profiler.showReport();

	return 0;
}