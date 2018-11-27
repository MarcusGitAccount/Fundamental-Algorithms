#ifndef LINKED_LIST_h
# define LINKED_LIST_h

template<typename T>
struct LinkedListNode {
	T* key;
	LinkedListNode* next;

	LinkedListNode(T* key) {
		this->key = key;
		this->next = NULL;
	}

	~LinkedListNode() {
		delete key;
	}
};

template<typename T>
class LinkedList {
public:
	LinkedListNode<T> *last, *first;

	LinkedList() {
		this->first = this->last = NULL;
		this->_size = 0;
	}

	LinkedList(LinkedListNode<T>* start) {
		last = first = start;
		_size = 0;

		while (start) {
			_size++;
			last = start;
			start = start->next;
		}
		last->next = NULL;
	}

	~LinkedList() {
		LinkedListNode<T>* head = this->first;

		while (head) {
			LinkedListNode<T>* current = head;

			head = head->next;
			delete current;
		}

		first = last = NULL;
	}

	void print(void(*print_node)(T)) {
		LinkedListNode<T>* head = this->first;

		while (head) {
			print_node(*head->key);
			head = head->next;
		}
		printf("\n");
	}

	void insert_last(T key) {
		LinkedListNode<T>* new_node = new LinkedListNode<T>(new T(key));

		if (last != NULL)
			last->next = new_node;
		last = new_node;
		if (first == NULL)
			first = new_node;
		_size++;
	}

	void remove_first() {
		auto current = this->first;

		this->first = this->first->next;
		if (this->first == NULL)
			this->last = NULL;
		this->_size--;
		delete current;
	}

	void insert_last(LinkedListNode<T>* new_node) {
		if (last != NULL)
			last->next = new_node;
		last = new_node;
		if (first == NULL)
			first = new_node;
		_size++;
	}

	void insert_multiple_last(T* args, int size) {
		for (int i = 0; i < size; i++)
			this->insert_last(args[i]);
	}

	uint32_t size() { return this->_size;  }

private:
	uint32_t _size = 0;
};

#endif