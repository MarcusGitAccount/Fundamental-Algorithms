#ifndef QUEUE_T_H
# define QUEUE_T_H

# include <cassert>

template<typename T>
class Queue {
public:
	Queue() {
		this->_size = 0;
		this->head = this->tail = NULL;
	}

	~Queue() {
		QueueNode* curr = head;

		while (head != NULL) {
			QueueNode* to_be_deleted = curr;

			head = head->next;
			delete to_be_deleted;
		}
	}

	void enqueue(T key) {
		QueueNode* new_node = new QueueNode(key, NULL);

		if (tail != NULL)
			tail->next = new_node;
		tail = new_node;
		if (head == NULL)
			head = new_node;
		this->_size++;
	}

	void dequeue() {
		QueueNode* current = head;

		if (head)
			head = head->next;
		if (head == NULL)
			tail = NULL;
		if (current != NULL) {
			this->_size--;
			delete current;
		}
	}

	T peek() {
		assert(this->size() != 0);
		return head->key;
	}

	int size() {
		return this->_size;
	}

	class QueueNode {
	public:
		QueueNode* next;
		T key;

		QueueNode(T key, QueueNode* next = NULL) {
			this->key = key;
			this->next = next;
		}
	};

private:
	int _size;
	QueueNode *head, *tail;
};

#endif