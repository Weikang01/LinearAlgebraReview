#include "LAR/LAR.h"
#include <iostream>
#include <thread>
#include <chrono>

int main(int argc, char* argv[])
{
	std::vector<std::vector<LAR::Rational>> data = {
		{1, 2, 3, 4},
		{5, 6, 7, 8},
		{9, 10, 11, 12}
	};

	LAR::Matrix<LAR::Rational> m1 = data;

	std::cout << "m1:\n" << m1 << std::endl;

	auto m2 = m1.RowEchelonForm();

	std::cout << "m1.RowEchelonForm():\n" << m2 << std::endl;

	return 0;
}