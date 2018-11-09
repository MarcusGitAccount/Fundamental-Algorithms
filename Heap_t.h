#ifndef HEAP_T_H
# define HEAP_T_H

#	include "../../Vector_t.h"
# include "../../Profiler.h"
#	include <stdint.h>

#	define left(x)   (2 * (x) + 1)
#	define right(x)  (2 * (x) + 2)
#	define parent(x) (((x) - 1) / 2)

Profiler profiler("heap-construction");

template<typename T>
class Heap {
public:
	Heap(bool (*cmp)(T, T), uint32_t size = 5) {
		this->container = Vector_t<T>(size);
		this->cmp = cmp;
	}

	Heap(T* container, bool(*cmp)(T, T), uint32_t size) {
		this->container = Vector_t<T>(container, size);
		this->cmp = cmp;
	}

	uint32_t dimension() {
		return this->container.size();
	}

	bool(*cmp)(T, T);

	void push(T element, const char* line = NULL, uint32_t size = 0) {
		uint32_t index = this->dimension();

		container.push_back(element);
		while (index > 0 && 
					this->cmp(this->container[index], this->container[parent(index)]) == true) {
			if (line)
				profiler.countOperation(line, size, 4);
			swap_(&this->container[index], &this->container[parent(index)]);
			index = parent(index);
		}
	}

	void print_heap(void(*print_element)(T element)) {
		for (uint32_t i = 0; i < this->container.size(); i++)
			print_element(this->container[i]);
	}

	T* top() {
		return this->container.head();
	}

	T* pop(const char* line = NULL, uint32_t size = 0) {
		T* top = this->top();
		T* last = this->container.pop();

		if (last != NULL && this->container.size() > 0) {
			this->container[0] = *last;
			this->heapify(0, line, size, true);
		}

		return top;
	}

	bool isEmpty() {
		return this->container.size() == 0;
	}

	T* get_container(uint32_t* n) {
		return this->container.get_container(n);
	}

	/*
	** This will destroy the heap.
	*/
	void push_container(T element) {
		this->container.push_back(element);
	}

	void push_container(T* elements, uint32_t size) {
		for (uint32_t i = 0; i < size; i++)
			this->push_container(elements[i]);
	}

	// Top down construction of the heap.
	// Assumes elements are already present in the heap container.
	void build_heap(uint32_t size, const char* line = NULL) {
		uint32_t n = this->dimension();

		if (line != NULL)
			profiler.countOperation(line, size, 0);

		for (uint32_t i = 1; i < n; i++) {
			uint32_t index = i;

			while (index > 0 &&
				this->cmp(this->container[index], this->container[parent(index)]) == true) {

				if (line != NULL)
					profiler.countOperation(line, size, 1);

				swap_(&this->container[index], &this->container[parent(index)]);
				index = parent(index);
			}
		}
	}

	// Bottom up construction of the heap
	void make_heap(uint32_t n = 0, const char* line = NULL, bool count_all = false) {
		/*
			Starting from the last node that is not a leaf.
			Being a complete binary tree there are n / 2 nodes and n / 2 leaves. (geometric progression)
		*/
		for (uint32_t i = this->dimension() / 2; i > 0; i--)
			this->heapify(i, line, n, count_all);
		this->heapify(0, line, n, count_all); // for some reason it doesn't work in the loop, lol
	}

	// Sorts the inner container based on the cmp function provided initially.
	void HeapSort(uint32_t size = 0, const char* line = NULL, bool count_all = false) {
		this->make_heap(size, line, count_all);

		uint32_t n = this->dimension();

		for (uint32_t i = n - 1; i > 0; i--) {
			T first = *this->container.head();
			T last  = *this->container.pop();

			// calling pop() on the vector also decreases its size
			if (line != NULL)
				profiler.countOperation(line, size, 1);

			this->container[this->dimension()] = first;
			this->container[0] = last;
			this->heapify(0, line, size, count_all);
		}
	}

private:
	Vector_t<T> container;

	void heapify(uint32_t index, const char* line = NULL, uint32_t n = 0, bool count_all = false) {
		// profiler
		if (line != NULL)
			profiler.countOperation(line, n, 1);

		uint32_t next = index;
		const uint32_t l = left(index);
		const uint32_t r = right(index);

		if (l < this->dimension() && this->cmp(container[l], container[next]) == true)
			next = l;
		if (r < this->dimension() && this->cmp(container[r], container[next]) == true)
			next = r;
		// profiler
		if (line && count_all)
			profiler.countOperation(line, n, 4);

		if (next == index)
			return;
		// profiler
		if (line && count_all)
			profiler.countOperation(line, n, 3);
		swap_(&container[next], &container[index]);
		this->heapify(next, line, n);
	}
};

#endif // !HEAP_T_H
