#pragma once
#include <iostream>
#include "LAR_export.h"
#include "Algorithms.h"

namespace LAR
{
	template <typename DataType>
	class LAR_EXPORT Matrix
	{
	public:
#pragma region Utility Functions
		class RowView
		{
		public:
			RowView(DataType* data, const int cols)
				: mData(data), mCols(cols)
			{}
			DataType& operator[](const int col)
			{
				return mData[col];
			}
			DataType operator[](const int col) const
			{
				return mData[col];
			}
		private:
			DataType* mData;
			int mCols;
		};

		class ColView
		{
		public:
			ColView(DataType* data, const int rows, const int cols)
				: mData(data), mRows(rows), mCols(cols)
			{
			}
			DataType& operator[](const int row)
			{
				return mData[row * mCols];
			}
			DataType operator[](const int row) const
			{
				return mData[row * mCols];
			}
		private:
			DataType* mData;
			int mRows;
			int mCols;
		};

		Matrix(const int rows, const int cols)
			: mNumRows(rows), mNumCols(cols), mData(new DataType[rows * cols])
		{
		}
		Matrix(const Matrix& other)
			: mNumRows(other.mNumRows), mNumCols(other.mNumCols), mData(new DataType[other.mNumRows * other.mNumCols])
		{
			for (int i = 0; i < mNumRows * mNumCols; ++i)
			{
				mData[i] = other.mData[i];
			}
		}
		virtual ~Matrix()
		{
			delete[] mData;
		}
		Matrix& operator=(const Matrix& other)
		{
			if (this != &other)
			{
				delete[] mData;
				mNumRows = other.mNumRows;
				mNumCols = other.mNumCols;
				mData = new DataType[mNumRows * mNumCols];
				for (int i = 0; i < mNumRows * mNumCols; ++i)
				{
					mData[i] = other.mData[i];
				}
			}
			return *this;
		}
		DataType& operator()(const int row, const int col)
		{
			return mData[row * mNumCols + col];
		}
		DataType operator()(const int row, const int col) const
		{
			return mData[row * mNumCols + col];
		}

		RowView operator[](const int row)
		{
			return RowView(mData + row * mNumCols, mNumCols);
		}
		RowView operator[](const int row) const
		{
			return RowView(mData + row * mNumCols, mNumCols);
		}
		ColView getCol(const int col)
		{
			return ColView(mData + col, mNumRows, mNumCols);
		}
		ColView getCol(const int col) const
		{
			return ColView(mData + col, mNumRows, mNumCols);
		}

		int getRows() const
		{
			return mNumRows;
		}
		int getCols() const
		{
			return mNumCols;
		}

		friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix)
		{
			for (int i = 0; i < matrix.mNumRows; ++i)
			{
				for (int j = 0; j < matrix.mNumCols; ++j)
				{
					os << matrix.mData[i * matrix.mNumCols + j] << " ";
				}
				os << std::endl;
			}
			return os;
		}

		bool operator==(const Matrix& other) const
		{
			if (mNumRows != other.mNumRows || mNumCols != other.mNumCols)
			{
				return false;
			}
			for (int i = 0; i < mNumRows * mNumCols; ++i)
			{
				if (mData[i] != other.mData[i])
				{
					return false;
				}
			}
			return true;
		}
		
		bool operator!=(const Matrix& other) const
		{
			return !(*this == other);
		}

#pragma endregion Utility Functions

		bool IsSquare() const
		{
			return mNumRows == mNumCols;
		}

		bool CanMultiply(const Matrix& other) const
		{
			return mNumCols == other.mNumRows;
		}

		bool SameSize(const Matrix& other) const
		{
			return mNumRows == other.mNumRows && mNumCols == other.mNumCols;
		}

		template<typename OtherDataType>
		Matrix operator+(const Matrix<OtherDataType>& other) const
		{
			if (!SameSize(other))
			{
				throw std::invalid_argument("Matrices must be the same size to add them.");
			}
			Matrix result(mNumRows, mNumCols);
			for (int i = 0; i < mNumRows * mNumCols; ++i)
			{
				result.mData[i] = mData[i] + other.mData[i];
			}
			return result;
		}

		template<typename OtherDataType>
		Matrix operator-(const Matrix<OtherDataType>& other) const
		{
			if (!SameSize(other))
			{
				throw std::invalid_argument("Matrices must be the same size to subtract them.");
			}
			Matrix result(mNumRows, mNumCols);
			for (int i = 0; i < mNumRows * mNumCols; ++i)
			{
				result.mData[i] = mData[i] - other.mData[i];
			}
			return result;
		}

		template<typename OtherDataType>
		Matrix operator*(const Matrix<OtherDataType>& other) const
		{
			if (!CanMultiply(other))
			{
				throw std::invalid_argument("Matrices must have the same number of columns as the other matrix has rows to multiply them.");
			}
			Matrix result(mNumRows, other.mNumCols);
			for (int i = 0; i < mNumRows; ++i)
			{
				for (int j = 0; j < other.mNumCols; ++j)
				{
					DataType sum = 0;
					for (int k = 0; k < mNumCols; ++k)
					{
						sum += mData[i * mNumCols + k] * other.mData[k * other.mNumCols + j];
					}
					result.mData[i * other.mNumCols + j] = sum;
				}
			}
			return result;
		}

		template<typename OtherDataType>
		Matrix operator*(const OtherDataType scalar) const
		{
			Matrix result(mNumRows, mNumCols);
			for (int i = 0; i < mNumRows * mNumCols; ++i)
			{
				result.mData[i] = mData[i] * scalar;
			}
			return result;
		}

		friend Matrix operator*(const DataType scalar, const Matrix& matrix)
		{
			return matrix * scalar;
		}

		template<typename OtherDataType>
		Matrix operator/(const OtherDataType scalar) const
		{
			Matrix result(mNumRows, mNumCols);
			for (int i = 0; i < mNumRows * mNumCols; ++i)
			{
				result.mData[i] = mData[i] / scalar;
			}
			return result;
		}

		Matrix operator-() const
		{
			Matrix result(mNumRows, mNumCols);
			for (int i = 0; i < mNumRows * mNumCols; ++i)
			{
				result.mData[i] = -mData[i];
			}
			return result;
		}

		Matrix Transpose() const
		{
			Matrix result(mNumCols, mNumRows);
			for (int i = 0; i < mNumRows; ++i)
			{
				for (int j = 0; j < mNumCols; ++j)
				{
					result.mData[j * mNumRows + i] = mData[i * mNumCols + j];
				}
			}
			return result;
		}

		static Matrix Identity(const int size)
		{
			Matrix result(size, size);
			for (int i = 0; i < size; ++i)
			{
				result.mData[i * size + i] = 1;
			}
			return result;
		}
		static Matrix Random(const int rows, const int cols, const DataType min, const DataType max)
		{
			Matrix result(rows, cols);
			for (int i = 0; i < rows * cols; ++i)
			{
				result.mData[i] = RandomValue(min, max);
			}
			return result;
		}
		static Matrix Magic(const int size)
		{
			Matrix result(size, size);
			int row = 0;
			int col = size / 2;
			for (int i = 1; i <= size * size; ++i)
			{
				result(row, col) = i;
				if (i % size == 0)
				{
					row++;
				}
				else
				{
					row = (row - 1 + size) % size;
					col = (col + 1) % size;
				}
			}
			return result;
		}

	private:
		int mNumRows;
		int mNumCols;
		DataType* mData;
	};

	template<typename DataType, bool RowVector = false>
	class LAR_EXPORT Vector : public Matrix<DataType>
	{
	public:
		Vector(const int size)
			: Matrix<DataType>(RowVector ? 1 : size, RowVector ? size : 1)
		{
		}

		Vector(const Matrix<DataType>& other)
			: Matrix<DataType>(other)
		{
		}

		DataType& operator[](const int index)
		{
			if (Matrix<DataType>::getRows() == 1)
			{
				return Matrix<DataType>::operator()(0, index);
			}
			else
			{
				return Matrix<DataType>::operator()(index, 0);
			}
		}

		DataType operator[](const int index) const
		{
			if (Matrix<DataType>::getRows() == 1)
			{
				return Matrix<DataType>::operator()(0, index);
			}
			else
			{
				return Matrix<DataType>::operator()(index, 0);
			}
		}
	};
}