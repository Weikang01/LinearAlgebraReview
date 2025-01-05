#pragma once
#include "MatrixBase.h"
#include "LAR_export.h"
#include <ostream>

namespace LAR
{
	template<typename DataType, bool RowVector = false>
	class LAR_EXPORT Vector : public MatrixBase<Vector<DataType, RowVector>, DataType>
	{
	public:
		using MatrixBase<Vector<DataType, RowVector>, DataType>::MatrixBase;
		using MatrixBase<Vector<DataType, RowVector>, DataType>::operator*;

		Vector(const size_t size)
			: MatrixBase<Vector<DataType, RowVector>, DataType>(RowVector ? 1 : size, RowVector ? size : 1)
		{
		}

		template<typename OtherDerived>
		Vector(const MatrixBase<OtherDerived, DataType>& other)
			: MatrixBase<Vector<DataType, RowVector>, DataType>(other)
		{
		}

		Vector(const std::initializer_list<DataType>& list)
			: MatrixBase<Vector<DataType, true>, DataType>(1, list.size())
		{
			size_t i = 0;
			for (const auto& value : list)
			{
				(*this)[i++] = value;
			}
		}

		Vector(const DataType* data, const size_t size, const bool rowVector = false)
			: MatrixBase<Vector<DataType, RowVector>, DataType>(rowVector ? 1 : size, rowVector ? size : 1)
		{
			for (size_t i = 0; i < size; ++i)
			{
				(*this)[i] = data[i];
			}
		}

		template<typename OtherDataType, bool OtherRowVector>
		auto operator*(const Vector<OtherDataType, OtherRowVector>& other) const
		{
			if (GetSize() != other.GetSize())
			{
				throw std::invalid_argument("Vectors must be the same size to multiply them.");
			}
			DataType result = 0;
			for (size_t i = 0; i < GetSize(); ++i)
			{
				result += (*this)[i] * other[i];
			}
			return result;
		}

		template<typename OtherDataType>
		auto operator*(const Matrix<OtherDataType>& other) const
		{
			if (GetCols() != other.GetRows())
			{
				throw std::invalid_argument("Number of columns in vector must match number of rows in matrix.");
			}
			Vector<decltype(DataType()* OtherDataType()), RowVector> result(other.GetCols());
			for (size_t i = 0; i < other.GetCols(); ++i)
			{
				result[i] = 0;
				for (size_t j = 0; j < GetSize(); ++j)
				{
					result[i] += (*this)[j] * other(j, i);
				}
			}
			return result;
		}

		size_t GetSize() const
		{
			return mNumRows == 1 ? GetCols() : GetRows();
		}

		DataType& operator[](const size_t index)
		{
			if (GetRows() == 1)
			{
				return this->AsDerived().operator()(0, index);
			}
			else
			{
				return this->AsDerived().operator()(index, 0);
			}
		}

		DataType operator[](const size_t index) const
		{
			if (GetRows() == 1)
			{
				return this->AsDerived()(0, index);
			}
			else
			{
				return this->AsDerived()(index, 0);
			}
		}

		template<typename OtherDataType, bool OtherRowVector>
		DataType DotProduct(const Vector<OtherDataType, OtherRowVector>& other) const
		{
			if (GetSize() != other.GetSize())
			{
				throw std::invalid_argument("Vectors must be the same size to calculate the dot product.");
			}
			DataType result = 0;
			for (size_t i = 0; i < GetSize(); ++i)
			{
				result += (*this)[i] * other[i];
			}
			return result;
		}

		template<typename OtherDataType, bool OtherRowVector>
		auto CrossProduct(const Vector<OtherDataType, OtherRowVector>& other) const
		{
			if (GetSize() != 3 || other.GetSize() != 3)
			{
				throw std::invalid_argument("Both vectors must be of size 3 to calculate the cross product.");
			}
			Vector<DataType, RowVector> result(3);
			result[0] = (*this)[1] * other[2] - (*this)[2] * other[1];
			result[1] = (*this)[2] * other[0] - (*this)[0] * other[2];
			result[2] = (*this)[0] * other[1] - (*this)[1] * other[0];
			return result;
		}

		template<typename OtherDataType, bool OtherRowVector>
		Matrix<DataType> OuterProduct(const Vector<OtherDataType, OtherRowVector>& other) const
		{
			Matrix<DataType> result(GetSize(), other.GetSize());
			for (size_t i = 0; i < GetSize(); ++i)
			{
				for (size_t j = 0; j < other.GetSize(); ++j)
				{
					result(i, j) = (*this)[i] * other[j];
				}
			}
			return result;
		}
	};
}