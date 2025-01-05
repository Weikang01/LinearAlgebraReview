#pragma once
#include <type_traits>
#include "Rational.h"

namespace LAR
{
	template<typename DataType>
	typename std::enable_if<std::is_arithmetic<DataType>::value, DataType>::type
	RandomValue(const DataType min, const DataType max)
	{
		return min + static_cast<DataType>(rand()) / (static_cast<DataType>(RAND_MAX / (max - min)));
	}
} // namespace LAR