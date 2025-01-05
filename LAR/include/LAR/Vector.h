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

		Vector(const int size)
			: MatrixBase<Vector<DataType, RowVector>, DataType>(RowVector ? 1 : size, RowVector ? size : 1)
		{
		}

		template<typename OtherDerived>
		Vector(const MatrixBase<OtherDerived, DataType>& other)
			: MatrixBase<Vector<DataType, RowVector>, DataType>(other)
		{
		}

		template<typename OtherDataType, bool OtherRowVector>
		auto operator*(const Vector<OtherDataType, OtherRowVector>& other) const
		{
			if (GetSize() != other.GetSize())
			{
				throw std::invalid_argument("Vectors must be the same size to multiply them.");
			}
			DataType result = 0;
			for (int i = 0; i < GetSize(); ++i)
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
			for (int i = 0; i < other.GetCols(); ++i)
			{
				result[i] = 0;
				for (int j = 0; j < GetSize(); ++j)
				{
					result[i] += (*this)[j] * other(j, i);
				}
			}
			return result;
		}

		int GetSize() const
		{
			return mNumRows == 1 ? GetCols() : GetRows();
		}

		DataType& operator[](const int index)
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

		DataType operator[](const int index) const
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
			for (int i = 0; i < GetSize(); ++i)
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
			for (int i = 0; i < GetSize(); ++i)
			{
				for (int j = 0; j < other.GetSize(); ++j)
				{
					result(i, j) = (*this)[i] * other[j];
				}
			}
			return result;
		}
	};
}