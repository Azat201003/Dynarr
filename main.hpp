#pragma once
#include <iostream>

template<typename T>
class convptr {
public:
	T* ptr;
	convptr(T* ptr) : ptr(ptr) {}
};

class dynarr {
private:
	size_t* sizes;
	void* current_pos;
	size_t size = 0;
	static const size_t give_size = 5252;
	size_t capacity;
	
	void* get_pos(size_t index) {
		unsigned long long p = reinterpret_cast<unsigned long long>(current_pos);
		for (int j = size; j > index; j--) {
			p -= sizes[j-1];
		}
		return reinterpret_cast<void*>(p);
	}
	
	void move_data(void* to) {
		size_t c = 0;
		for (size_t i = 0; i < size; i++) {
			for (size_t j = 0; j < sizes[i]; j++) {
				c++;
				char* ptr = static_cast<char*>(current_pos)+c;
				*(static_cast<char*>(to)+c) = *ptr;
			}
		}
		realloc(current_pos, capacity);
		current_pos = to;
	}

	void move_sizes(size_t* to) {
		for (int i = 0; i < size; i++) {
			to[i] = sizes[i];
		}
		realloc(sizes, capacity);
		sizes = to;
	}

	size_t calc_memory_size() {
		size_t c = 0;
		for (size_t i = 0; i < size; i++) {
			c+=sizes[i];
		}
		return c;
	}

	void add_memory(size_t size=give_size) { 
		void* ptr1 = malloc(capacity+size);
		move_data(ptr1);
		size_t* ptr2 = static_cast<size_t*>(malloc(capacity+size));
		move_sizes(ptr2);
		capacity += size;
	}
public:
	dynarr(size_t capacity = give_size) : capacity(capacity) {
		this->capacity = capacity;
		sizes = new size_t(capacity);
		current_pos = malloc(capacity);
	}

	~dynarr() {
		realloc(get_pos(0), capacity);
		realloc(sizes, capacity);
	}

	template <typename T>
	void push_back(T obj) {
		while (calc_memory_size()+sizeof(T) >= capacity) {
			add_memory();
		}
		if (size == 0){
			T* first = new T(obj);
			current_pos = first;
		}
		T* p = static_cast<T*>(current_pos);
		(*p) = obj;
		current_pos += sizeof(T);
		sizes[size] = sizeof(T);
		size++; 
	}
	template <typename T>
	T at(size_t index) {
		return *static_cast<T*>(at(index));
	}

	void* at(size_t index) {
		return get_pos(index);
	}
	void pop(size_t index) {
		size_t deleted_size = sizes[index];
		for (char* p=static_cast<char*>(get_pos(index)); p<=current_pos-deleted_size+10; p++) {
			(*p) = *(p+deleted_size);
		}
		
		size--;
	
		for (int j = index; j < size; j++) {
			sizes[j] = sizes[j+1];
        }
		current_pos = reinterpret_cast<void*>(reinterpret_cast<unsigned long long>(current_pos) - deleted_size); // ponos
	}
	void pop() {
		pop(size);
	}
	size_t getSize() {
		return size;
	}
};
