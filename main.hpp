#pragma once
#include <iostream>

class dynarr {
private:
	size_t* sizes;
	size_t MAX_SIZE;
	void* currentPos;
	size_t size = 0;
	
	void* getPos(size_t index) {
		void* p = currentPos;
                for (int j = size; j > index; j--) {
                        p -= sizes[j-1];
                }
		return p;
	}
public:
	dynarr(size_t MAX_SIZE) {
		this->MAX_SIZE = MAX_SIZE;
		std::cout << "max size setted\n";
		size_t* sizes_buffer = new size_t[MAX_SIZE];
		std::cout << "buffer craeted\n";
		sizes = sizes_buffer;
	}
	template <typename T>
	void push_back(T obj) {
		if (size == 0){
			T* first = new T(obj);
			currentPos = first;
		}
		T* p = static_cast<T*>(currentPos);
		(*p) = obj;
		currentPos += sizeof(T);
		sizes[size] = sizeof(T);
		size++; 
	}
	template <typename T>
	T at(size_t index) {
		return *static_cast<T*>(at(index));
	}

	void* at(size_t index) {
		return getPos(index);
	}
	void pop(size_t index) {
		size_t deleted_size = sizes[index];
		for (void* p=getPos(index); p<=currentPos-deleted_size+10; p++) {
                       	(*static_cast<char*>(p)) = (*static_cast<char*>(p+deleted_size));
                }
		
		size--;
	
		for (int j = index; j < size; j++) {
                        sizes[j] = sizes[j+1];
                }
		currentPos -= deleted_size;
	}
	void pop() {
		pop(size);
	}
	size_t getSize() {
		return size;
	}
};
