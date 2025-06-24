#include <iostream>
#include "../main.hpp"

int main() {
	std::cout << "started\n";
	dynarr arr(10);
	arr.push_back(2);
	arr.push_back('g');
	std::cout << "arr.size = " << arr.getSize() << std::endl;
	std::cout << "arr[0] = " << arr.at<int>(0) << std::endl;
	std::cout << "arr[1] = " << arr.at<char>(1) << std::endl;
	arr.push_back('b');
	arr.pop(0);
	std::cout << "arr[0] = " << arr.at<char>(0) << std::endl;
}
