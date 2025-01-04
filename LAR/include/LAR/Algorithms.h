#pragma once

namespace LAR
{
	template<typename DataType>
	DataType RandomValue(DataType min, DataType max)
	{
		return min + static_cast<DataType>(rand()) / (static_cast<DataType>(RAND_MAX / (max - min)));
	}
} // namespace LAR