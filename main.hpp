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
	void* first;
	void* end;
	size_t size = 0;
	static const size_t give_size = 2*sizeof(size_t);
	size_t capacity;
	
	size_t calc_memory_size() {
		size_t c = 0;
		for (size_t i = 0; i < size; i++) {
			c+=sizes[i];
		}
		return c;
	}
	
	void* get_pos(size_t index) const {
		unsigned long long p = reinterpret_cast<unsigned long long>(end);
		for (int j = size; j > index; j--) {
			p -= sizes[j-1];
		}
		return reinterpret_cast<void*>(p);
	}
	
	void* get_pos(size_t index) {
		unsigned long long p = reinterpret_cast<unsigned long long>(end);
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
				char* ptr = static_cast<char*>(first)+c;
				*(static_cast<char*>(to)+c) = *ptr;
			}
		}
		realloc(first, capacity);
		first = to;
		end = to+capacity;
	}

	void move_sizes(size_t* to) {
		for (int i = 0; i < size; i++) {
			to[i] = sizes[i];
		}
		realloc(sizes, capacity);
		sizes = to;
	}

	void add_memory(size_t size=give_size) { 
		void* ptr1 = malloc(capacity+size);
		move_data(ptr1);
		size_t* ptr2 = static_cast<size_t*>(malloc((capacity+size)/sizeof(size_t)));
		move_sizes(ptr2);
		capacity += size;
		end = reinterpret_cast<void*>(reinterpret_cast<long long>(end)+size);
	}
public:
	dynarr(size_t capacity = give_size) : capacity(capacity) {
		this->capacity = capacity;
		sizes = new size_t(capacity/sizeof(size_t));
		first = malloc(capacity);
		end = first;
	}

	~dynarr() {
		realloc(first, capacity);
		realloc(sizes, capacity/sizeof(size_t));
	}

	template <typename T>
	void push_back(T obj) {
		while (calc_memory_size()+sizeof(T) >= capacity) {
			add_memory();
		}
		T* p = static_cast<T*>(end);
		(*p) = obj;
		end += sizeof(T);
		sizes[size] = sizeof(T);
		size++; 
	}

	template <typename T>
	T at(size_t index) const {
		return *static_cast<T*>(at(index));
	}
	
	void* at(size_t index) const {
		return get_pos(index);
	}

	void* at(size_t index) {
		return get_pos(index);
	}
	
	void* operator[] (size_t index) const {
		return at(index);
	}
	
	void* operator[] (size_t index) {
		return at(index);
	}

	void pop(size_t index) {
		size_t deleted_size = sizes[index];
		for (char* p=static_cast<char*>(get_pos(index)); p<=end-deleted_size+10; p++) {
			(*p) = *(p+deleted_size);
		}
		
		size--;
	
		for (int j = index; j < size; j++) {
			sizes[j] = sizes[j+1];
        }
		end = reinterpret_cast<void*>(reinterpret_cast<unsigned long long>(end) - deleted_size);
	}
	void pop() {
		pop(size);
	}
	size_t getSize() {
		return size;
	}
};
