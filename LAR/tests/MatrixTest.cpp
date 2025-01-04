#include "LAR/LAR.h"
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>
#include <thread>
#include <chrono>

TEST_CASE("MatrixTest", "[MatrixTest]")
{
	LAR::Matrix<int> m1 = LAR::Matrix<int>::Random(3, 3, 0, 20);
	LAR::Matrix<int> m2 = LAR::Matrix<int>::Random(3, 3, 0, 20);
	std::cout << "m1: " << m1 << std::endl;
	std::cout << "m2: " << m2 << std::endl;
	auto m3 = m1 + m2;
	std::cout << "m1 + m2: " << m3 << std::endl;
	auto m4 = m1 - m2;
	std::cout << "m1 - m2: " << m4 << std::endl;
	auto m5 = m1 * m2;
	std::cout << "m1 * m2: " << m5 << std::endl;
	auto m6 = m1 * 2;
	std::cout << "m1 * 2: " << m6 << std::endl;
	auto m7 = 2 * m1;
	std::cout << "2 * m1: " << m7 << std::endl;
	auto m8 = m1 / 2;
	std::cout << "m1 / 2: " << m8 << std::endl;
	auto m9 = m1.Transpose();
	std::cout << "m1.transpose(): " << m9 << std::endl;
}