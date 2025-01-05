#include "LAR/LAR.h"
#include <iostream>
#include <thread>
#include <chrono>

int main(int argc, char* argv[])
{
	LAR::Matrix<LAR::Rational> m1 = LAR::Matrix<LAR::Rational>::Random(3, 4, -10, 10);

	std::cout << "m1:\n" << m1 << std::endl;

	auto m2 = m1.RowEchelonForm();

	std::cout << "m1.RowEchelonForm():\n" << m2 << std::endl;

	return 0;
}