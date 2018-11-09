#ifndef VECTOR_T_H
#	define VECTOR_T_H

	# include <stdlib.h>
  # include <stdint.h>

	template<typename T>
	class Vector_t {
	public:
		T operator [] (int index) const {
			if (index > this->capacity)
				this->resize(index);
			return this->container[index];
		}

		T& operator [] (int index) {
			if (index > this->capacity)
				this->resize(index);
			return this->container[index];
		}

		Vector_t(uint32_t capacity = 1) {
			this->capacity = capacity;
			this->container = new T[this->capacity];
			this->_size = 0;
		}

		Vector_t(T* container, uint32_t size) {
			this->capacity = size;
			this->_size = size;
			this->container = container;
		}

		~Vector_t() {
			if (this->container == NULL)
				delete this->container;
		}

		uint32_t size() { return this->_size; }

		bool push_back(T newElement) {
			if (this->_size + 1 >= this->capacity) {
				if (this->resize(this->capacity * 2) == false)
					return false;
			}

			this->container[this->_size++] = newElement;
			return true;
		}

		bool insert_at_index(uint32_t index, T element) {
			if (index >= this->capacity) {
				if (this->resize(index + 1) == false)
					return false;
			}

			this->container[index] = element;
			this->_size = index + 1;
			return true;
		}

		T* head() {
			if (this->_size == 0)
				return NULL;
			return &(this->container[0]);
		}

		T* last() {
			if (this->_size == 0)
				return NULL;
			return &(this->container[this->_size - 1]);
		}

		T* pop() {
			T* tail = this->last();

			if (tail == NULL)
				return tail;
			if (this->_size > 1)
				this->_size -= 1;
			return tail;
		}

		T* get_container(uint32_t* n) {
			if (n != NULL)
				*n = this->_size;
			return this->container;
		}

	private:
		T* container = NULL;
		uint32_t capacity = 0;
		uint32_t _size = 0;

		bool resize(uint32_t newCapacity) {
			if (newCapacity <= this->capacity)
				return false;
			if (newCapacity < 0)
				return false;

			this->capacity = newCapacity;
			
			T* newContainer = new T[this->capacity];

			for (uint32_t i = 0; i < this->size(); i++)
				newContainer[i] = this->container[i];

			delete[] this->container;

			this->container = newContainer;

			return true;
		}
	};

#endif
