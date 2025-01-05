#pragma once
#include <iostream>
#include "LAR_export.h"

namespace LAR
{
	template<typename DataType, bool RowVector>
	class Vector;

	template<typename DataType>
	class Matrix;

	template <typename Derived, typename DataType>
	class LAR_EXPORT MatrixBase
	{
	public:
#pragma region Utility Functions
		MatrixBase(const size_t rows, const size_t cols)
			: mNumRows(rows), mNumCols(cols), mData(new DataType[rows * cols])
		{
		}
		MatrixBase(const MatrixBase& other)
			: mNumRows(other.mNumRows), mNumCols(other.mNumCols), mData(nullptr) // Initialize mData to nullptr
		{
			if (mNumRows > 0 && mNumCols > 0)
			{
				mData = new DataType[mNumRows * mNumCols];

				for (size_t i = 0; i < mNumRows * mNumCols; ++i)
				{
					mData[i] = other.mData[i];
				}
			}
			else
			{
				mData = nullptr;
			}
		}

		virtual ~MatrixBase()
		{
			delete[] mData;
		}

		Derived& AsDerived() { return static_cast<Derived&>(*this); }
		const Derived& AsDerived() const { return static_cast<const Derived&>(*this); }

		MatrixBase& operator=(const MatrixBase& other)
		{
			if (this != &other)
			{
				delete[] mData;
				mNumRows = other.mNumRows;
				mNumCols = other.mNumCols;
				mData = new DataType[mNumRows * mNumCols];
				for (size_t i = 0; i < mNumRows * mNumCols; ++i)
				{
					mData[i] = other.mData[i];
				}
			}
			return *this;
		}

		size_t GetRows() const
		{
			return mNumRows;
		}
		size_t GetCols() const
		{
			return mNumCols;
		}

		bool operator==(const MatrixBase& other) const
		{
			if (mNumRows != other.mNumRows || mNumCols != other.mNumCols)
			{
				return false;
			}
			for (size_t i = 0; i < mNumRows * mNumCols; ++i)
			{
				if (mData[i] != other.mData[i])
				{
					return false;
				}
			}
			return true;
		}
		
		bool operator!=(const MatrixBase& other) const
		{
			return !(*this == other);
		}

		DataType& operator()(const size_t row, const size_t col)
		{
			return mData[row * mNumCols + col];
		}
		DataType operator()(const size_t row, const size_t col) const
		{
			return mData[row * mNumCols + col];
		}

#pragma endregion Utility Functions
		
		template<typename OtherDerived, typename OtherDataType>
		bool CanMultiply(const MatrixBase<OtherDerived, OtherDataType>& other) const
		{
			return mNumCols == other.mNumRows;
		}

		template<typename OtherDerived, typename OtherDataType>
		bool SameSize(const MatrixBase<OtherDerived, OtherDataType>& other) const
		{
			return mNumRows == other.mNumRows && mNumCols == other.mNumCols;
		}

		template<typename OtherDerived, typename OtherDataType>
		auto operator+(const MatrixBase<OtherDerived, OtherDataType>& other) const
		{
			if (!SameSize(other))
			{
				throw std::invalid_argument("Matrices must be the same size to add them.");
			}
			Derived result(mNumRows, mNumCols);
			for (size_t i = 0; i < mNumRows * mNumCols; ++i)
			{
				result.mData[i] = mData[i] + other.mData[i];
			}
			return result;
		}

		template<typename OtherDerived, typename OtherDataType>
		auto operator-(const MatrixBase<OtherDerived, OtherDataType>& other) const
		{
			if (!SameSize(other))
			{
				throw std::invalid_argument("Matrices must be the same size to subtract them.");
			}
			Derived result(mNumRows, mNumCols);
			for (size_t i = 0; i < mNumRows * mNumCols; ++i)
			{
				result.mData[i] = mData[i] - other.mData[i];
			}
			return result;
		}

		Derived operator*(const DataType scalar) const
		{
			Derived result(mNumRows, mNumCols);
			for (size_t i = 0; i < mNumRows * mNumCols; ++i)
			{
				result.mData[i] = mData[i] * scalar;
			}
			return result;
		}

		template<typename OtherDataType>
		Derived operator/(const OtherDataType scalar) const
		{
			Derived result(mNumRows, mNumCols);
			for (size_t i = 0; i < mNumRows * mNumCols; ++i)
			{
				result.mData[i] = mData[i] / scalar;
			}
			return result;
		}

		Derived operator-() const
		{
			Derived result(mNumRows, mNumCols);
			for (size_t i = 0; i < mNumRows * mNumCols; ++i)
			{
				result.mData[i] = -mData[i];
			}
			return result;
		}

		Derived Transpose() const
		{
			Derived result(mNumCols, mNumRows);
			for (size_t i = 0; i < mNumRows; ++i)
			{
				for (size_t j = 0; j < mNumCols; ++j)
				{
					result.mData[j * mNumRows + i] = mData[i * mNumCols + j];
				}
			}
			return result;
		}

		void TransposeInPlace() const
		{
			*this = Transpose();
		}

		size_t mNumRows;
		size_t mNumCols;
		DataType* mData;
	};

	template<typename Derived, typename DataType>
	std::ostream& operator<<(std::ostream& os, const MatrixBase<Derived, DataType>& matrix)
	{
		for (size_t i = 0; i < matrix.mNumRows; ++i)
		{
			for (size_t j = 0; j < matrix.mNumCols; ++j)
			{
				os << matrix.mData[i * matrix.mNumCols + j] << " ";
			}
			os << std::endl;
		}
		return os;
	}

	template<typename Derived, typename DataType>
	Derived operator*(const DataType scalar, const MatrixBase<Derived, DataType>& matrix)
	{
		return matrix * scalar; // Use already defined member operator
	}
}