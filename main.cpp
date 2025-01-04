#include "LAR/LAR.h"
#include <iostream>
#include <thread>
#include <chrono>

int main(int argc, char* argv[])
{
	LAR::Vector<int, true> v1(3);
	v1[0] = 1;
	v1[1] = 2;
	v1[2] = 3;

	std::cout << "v1: " << v1 << std::endl;

	LAR::Matrix<int> m1 = LAR::Matrix<int>::Random(3, 3, 0, 20);

	std::cout << "m1: " << m1 << std::endl;

	auto v2 = v1 * m1;

	std::cout << "v1 * m1: " << v2 << std::endl;

	return 0;
}