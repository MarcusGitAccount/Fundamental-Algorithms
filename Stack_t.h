#ifndef STACK_T_H
# define STACK_T_H

#include <cassert>

template <typename T>
class Stack {
public:
	template <typename T>
	struct StackNode {
		StackNode<T>* next;
		T data;

		StackNode(T data, StackNode<T>* next = NULL) {
			this->next = next;
			this->data = data;
		}
	};

	Stack() {
		this->head = NULL;
		this->_size = 0;
	}

	~Stack() {
		StackNode<T>* current = this->head;

		while (current) {
			StackNode<T>* to_be_deleted = current;

			current = current->next;
			delete to_be_deleted;
		}
	}

	int size() { return this->_size;  }

	void push(T new_data) {
		StackNode<T>* new_node = new StackNode<T>(new_data, this->head);

		this->head = new_node;
		this->_size++;
	}

	int peek() {
		assert(!this->is_empty() && "Stack is not empty.");
		return this->head->data;
	}

	// @returns true if an elemen was popped and false otherwise
	bool pop() {
		if (this->size() == 0)
			return false;

		StackNode<T>* old_head = this->head;

		this->head = this->head->next;
		this->_size -= 1;
		
		delete old_head;
		return false;
	}

	bool is_empty() { return this->size() == 0; }

private:
	StackNode<T>* head;
	int _size;
};

#endif // !STACK_T_H
