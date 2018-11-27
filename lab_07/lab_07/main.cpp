/*
	Group: 30229
	Name: Marcus Pop
	Lab no: 07

	Requirement: Implement 3 different ways to represent multiway
	trees and transformations between them.

	R1: parent representation
	R2: multi-way representation
	R3: binary tree representation

	Transformations to implement: R1 -> R2 and R2 -> R3
	Implement also pretty print for an R3 representation.

	R1: Each node is connected to its parent.
	R2: Each node has connections to its children.
	R3: The tree is represented as a binary tree such as the first
	child is connected to its parent and the children to follow
	are wired to the previous child.

*/
#include <stdio.h>
#include "../../Vector_t.h"
#include "../../LinkedList_t.h"

// Using the same node structure for each representation.
// Changes that result from any of the transformations will
// be done in place, i.e. no new nodes will be created

struct TreeNode
{
public:
	TreeNode* parent;
	int key;
	LinkedList<TreeNode*>* children_R2;
	
	TreeNode* next_R3;
	TreeNode* child_R3;

	TreeNode(int key, TreeNode* parent = NULL) {
		this->parent = parent;
		this->key = key;
		this->children_R2 = new LinkedList<TreeNode*>();
		this->child_R3 = this->next_R3 = NULL;
	}
};

// parent[i] represents the parent for the node keys[i] 
// @returns a container representing all the nodes in the tree 
TreeNode** create_R1_representation(int* parents, int* keys, int size) {
	TreeNode** nodes = new TreeNode*[size];

	for (int i = 0; i < size; i++)
		nodes[i] = new TreeNode(keys[i], NULL);
	for (int i = 0; i < size; i++) {
		if (parents[i] != -1) {
			// is not root
			nodes[i]->parent = nodes[parents[i] - 1];
		}
	}

	return nodes;
}

// @param nodes - nodes in R1 representation
// @returns the root of the tree
TreeNode* transformation_R1_to_R2(TreeNode** nodes, int size) {
	TreeNode* root = NULL;

	if (size == 0 || nodes == NULL)
		return root;

	// find the root
	root = nodes[0];
	while (root && root->parent != NULL)
		root = root->parent;
	for (int i = 0; i < size; i++) {
		// from each node go up to the tree root
		// and add each node to its parent's children list
		TreeNode* node = nodes[i];

		while (node != NULL && node->parent != NULL) {
			TreeNode* child = node;

			node->parent->children_R2->insert_last(node);
			node = node->parent;
			child->parent = NULL;
		}
	}

	return root;
}

// used to debug the R1->R2 transformation
void print_R2_bfs(TreeNode* root) {
	if (root == NULL) {
		printf("Empty tree.\n");
		return;
	}

	auto queue = new LinkedList<TreeNode*>();
	
	queue->insert_last(root);
	while (queue->size() != 0) {
		auto size = queue->size();

		// print level
		for (int i = 0; i < size; i++) {
			LinkedListNode<TreeNode*>* top = queue->first;
			LinkedListNode<TreeNode*>* node = (*(top->key))->children_R2->first;
			int tree_key = (*(top->key))->key;

			queue->remove_first();
			printf("%d ", tree_key);

			// add its children to the queue
			while (node != NULL) {
				queue->insert_last(*node->key);
				node = node->next;
			}
		}
		printf("\n");
	}
}

void preorder(TreeNode* root) {
	if (root != NULL) {
		printf("%d ", root->key);

		LinkedListNode<TreeNode*>* node = root->children_R2->first;

		while (node != NULL) {
			preorder(*node->key);
			node = node->next;
		}

	}
}

// @returns nothing, transformation is done in place
void transformation_R2_to_R3(TreeNode* root) {
	if (root == NULL)
		return;
	if (root->children_R2->size() == 0) {
		root->child_R3 = NULL;
		return;
	}

	auto current = root->children_R2->first;
	TreeNode* prev;

	prev = root->child_R3 = *current->key;

	if (current)
		current = current->next;
	while (current) {
		transformation_R2_to_R3(prev);

		prev->next_R3 = *current->key;
		prev = prev->next_R3;
		current = current->next;
	}
}

// used to debug the R3 representtion
void pretty_print(TreeNode* root, int indent = 0, int increment = 1) {
	if (root != NULL) {
		for (int i = 0; i < indent; i++)
			printf(" ");

		printf("%d\n", root->key);
		pretty_print(root->child_R3, indent + increment, increment);
		pretty_print(root->next_R3, indent, increment);
	}
}

int main(void) {
	int parents[] = {2, 7, 5, 2, 7, 7, -1, 5, 2};
	int keys[]		= {1, 2, 3, 4, 5, 6,  7, 8, 9};
	int size = sizeof(parents) / sizeof(int);
	TreeNode** nodes = create_R1_representation(parents, keys, size);
	TreeNode* root = transformation_R1_to_R2(nodes, size);

	preorder(root);
	printf("\n\n");
	print_R2_bfs(root);
	printf("\n");

	transformation_R2_to_R3(root);
	pretty_print(root, 1, 3);
	return 0;
}