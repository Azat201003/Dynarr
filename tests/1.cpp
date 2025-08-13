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
	char* a = "abeme";;
	//arr.push_back(reinterpret_cast<long long>(a));
	arr.push_back(convptr(a));
	//std::cout << reinterpret_cast<long long>(a) << std::endl;
	arr.pop(0);
	arr.pop(0);
	std::cout << "arr.size = " << arr.getSize() << std::endl;
	std::cout << "arr[0] = " << std::string((arr.at<convptr<char>>(0)).ptr) << std::endl;
}
