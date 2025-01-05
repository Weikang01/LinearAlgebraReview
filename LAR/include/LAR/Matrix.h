#pragma once
#include "MatrixBase.h"
#include "LAR_export.h"
#include "Algorithms.h"
#include <vector>

namespace LAR
{
	template<typename DataType>
	class LAR_EXPORT Matrix : public MatrixBase<Matrix<DataType>, DataType>
	{
	public:
		using MatrixBase<Matrix<DataType>, DataType>::MatrixBase; // Inherit constructors
		using MatrixBase<Matrix<DataType>, DataType>::operator*;

		class RowView
		{
		public:
			RowView(DataType* data, const size_t cols)
				: mData(data), mCols(cols)
			{
			}
			DataType& operator[](const size_t col)
			{
				return mData[col];
			}
			DataType operator[](const size_t col) const
			{
				return mData[col];
			}
		private:
			DataType* mData;
			size_t mCols;
		};

		class ColView
		{
		public:
			ColView(DataType* data, const size_t rows, const size_t cols)
				: mData(data), mRows(rows), mCols(cols)
			{
			}
			DataType& operator[](const size_t row)
			{
				return mData[row * mCols];
			}
			DataType operator[](const size_t row) const
			{
				return mData[row * mCols];
			}
		private:
			DataType* mData;
			size_t mRows;
			size_t mCols;
		};

		Matrix(const size_t rows, const size_t cols)
			: MatrixBase<Matrix<DataType>, DataType>(rows, cols) {}

		Matrix(const MatrixBase<Matrix<DataType>, DataType>& other)
			: MatrixBase<Matrix<DataType>, DataType>(other) {}

		Matrix(const DataType* data, const size_t rows, const size_t cols)
			: MatrixBase<Matrix<DataType>, DataType>(rows, cols)
		{
			for (size_t i = 0; i < rows * cols; ++i)
			{
				this->mData[i] = data[i];
			}
		}

		Matrix(const std::vector<std::vector<DataType>>& data)
			: MatrixBase<Matrix<DataType>, DataType>(data.size(), data[0].size())
		{
			for (size_t i = 0; i < this->mNumRows; ++i)
			{
				for (size_t j = 0; j < this->mNumCols; ++j)
				{
					(*this)(i, j) = data[i][j];
				}
			}
		}

		RowView operator[](const size_t row)
		{
			return RowView(this->mData + row * this->mNumCols, this->mNumCols);
		}
		RowView operator[](const size_t row) const
		{
			return RowView(this->mData + row * this->mNumCols, this->mNumCols);
		}
		ColView getCol(const size_t col)
		{
			return ColView(this->mData + col, this->mNumRows, this->mNumCols);
		}
		ColView getCol(const size_t col) const
		{
			return ColView(this->mData + col, this->mNumRows, this->mNumCols);
		}

		template<typename OtherDataType>
		auto operator*(const Matrix<OtherDataType>& other) const
		{
			if (this->mNumCols != other.mNumRows)
			{
				throw std::invalid_argument("Number of columns in first matrix must match number of rows in second matrix.");
			}
			Matrix<decltype(DataType()* OtherDataType())> result(this->mNumRows, other.mNumCols);
			for (size_t i = 0; i < this->mNumRows; ++i)
			{
				for (size_t j = 0; j < other.mNumCols; ++j)
				{
					result(i, j) = 0;
					for (size_t k = 0; k < this->mNumCols; ++k)
					{
						result(i, j) += (*this)(i, k) * other(k, j);
					}
				}
			}
			return result;
		}

		template<typename OtherDataType, bool OtherRowVector>
		auto operator*(const Vector<OtherDataType, OtherRowVector>& vector) const
		{
			if (this->mNumCols != vector.GetSize())
			{
				throw std::invalid_argument("Number of columns in matrix must match size of vector.");
			}
			Vector<decltype(DataType()* OtherDataType()), true> result(this->mNumRows);
			for (size_t i = 0; i < this->mNumRows; ++i)
			{
				result[i] = 0;
				for (size_t j = 0; j < this->mNumCols; ++j)
				{
					result[i] += (*this)(i, j) * vector[j];
				}
			}
			return result;
		}

		bool IsSquare() const
		{
			return mNumRows == mNumCols;
		}
		bool IsIdentity() const
		{
			if (!IsSquare())
			{
				return false;
			}
			for (size_t i = 0; i < mNumRows; ++i)
			{
				for (size_t j = 0; j < mNumCols; ++j)
				{
					if (i == j && mData[i * mNumCols + j] != 1)
					{
						return false;
					}
					else if (i != j && mData[i * mNumCols + j] != 0)
					{
						return false;
					}
				}
			}
			return true;
		}
		bool IsSymmetric() const
		{
			if (!IsSquare())
			{
				return false;
			}
			for (size_t i = 0; i < mNumRows; ++i)
			{
				for (size_t j = 0; j < mNumCols; ++j)
				{
					if (mData[i * mNumCols + j] != mData[j * mNumCols + i])
					{
						return false;
					}
				}
			}
			return true;
		}

		Matrix Minor(const size_t row, const size_t col) const
		{
			if (mNumRows <= 1 || mNumCols <= 1)
			{
				throw std::invalid_argument("Matrix must be at least 2x2 to find a minor.");
			}

			if (row < 0 || row >= mNumRows || col < 0 || col >= mNumCols)
			{
				throw std::invalid_argument("Row and column must be within the bounds of the matrix.");
			}

			Matrix result(mNumRows - 1, mNumCols - 1);
			for (size_t i = 0; i < mNumRows; ++i)
			{
				for (size_t j = 0; j < mNumCols; ++j)
				{
					if (i != row && j != col)
					{
						size_t iOffset = i < row ? 0 : 1;
						size_t jOffset = j < col ? 0 : 1;
						result(i - iOffset, j - jOffset) = mData[i * mNumCols + j];
					}
				}
			}
			return result;
		}

		static Matrix Identity(const size_t size)
		{
			Matrix result(size, size);
			for (size_t i = 0; i < size; ++i)
			{
				result.mData[i * size + i] = 1;
			}
			return result;
		}
		static Matrix Random(const size_t rows, const size_t cols, const DataType min, const DataType max)
		{
			Matrix result(rows, cols);
			for (size_t i = 0; i < rows * cols; ++i)
			{
				result.mData[i] = RandomValue(min, max);
			}
			return result;
		}
		static Matrix Fill(const size_t rows, const size_t cols, const DataType value)
		{
			Matrix result(rows, cols);
			for (size_t i = 0; i < rows * cols; ++i)
			{
				result.mData[i] = value;
			}
			return result;
		}
		static Matrix Magic(const size_t size)
		{
			Matrix result(size, size);
			size_t row = 0;
			size_t col = size / 2;
			for (size_t i = 1; i <= size * size; ++i)
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
		static Matrix Vandermonde(const size_t rows, const size_t cols, const DataType start, const DataType step)
		{
			Matrix result(rows, cols);
			for (size_t i = 0; i < rows; ++i)
			{
				for (size_t j = 0; j < cols; ++j)
				{
					result(i, j) = std::pow(start + i, j);
				}
			}
			return result;
		}

		DataType Trace() const
		{
			if (!IsSquare())
			{
				throw std::invalid_argument("Matrix must be square to find the trace.");
			}
			DataType sum = 0;
			for (size_t i = 0; i < mNumRows; ++i)
			{
				sum += mData[i * mNumCols + i];
			}
			return sum;
		}

		void SwapRows(const size_t row1, const size_t row2)
		{
			if (row1 < 0 || row1 >= mNumRows || row2 < 0 || row2 >= mNumRows)
			{
				throw std::invalid_argument("Rows must be within the bounds of the matrix.");
			}
			for (size_t i = 0; i < mNumCols; ++i)
			{
				std::swap(mData[row1 * mNumCols + i], mData[row2 * mNumCols + i]);
			}
		}

		void SwapCols(const size_t col1, const size_t col2)
		{
			if (col1 < 0 || col1 >= mNumCols || col2 < 0 || col2 >= mNumCols)
			{
				throw std::invalid_argument("Columns must be within the bounds of the matrix.");
			}
			for (size_t i = 0; i < mNumRows; ++i)
			{
				std::swap(mData[i * mNumCols + col1], mData[i * mNumCols + col2]);
			}
		}

		void ScaleRow(const size_t row, const DataType scalar)
		{
			if (row < 0 || row >= mNumRows)
			{
				throw std::invalid_argument("Row must be within the bounds of the matrix.");
			}
			for (size_t i = 0; i < mNumCols; ++i)
			{
				mData[row * mNumCols + i] *= scalar;
			}
		}

		void ScaleCol(const size_t col, const DataType scalar)
		{
			if (col < 0 || col >= mNumCols)
			{
				throw std::invalid_argument("Column must be within the bounds of the matrix.");
			}
			for (size_t i = 0; i < mNumRows; ++i)
			{
				mData[i * mNumCols + col] *= scalar;
			}
		}

		void AddRow(const size_t row1, const size_t row2, const DataType scalar)
		{
			if (row1 < 0 || row1 >= mNumRows || row2 < 0 || row2 >= mNumRows)
			{
				throw std::invalid_argument("Rows must be within the bounds of the matrix.");
			}
			for (size_t i = 0; i < mNumCols; ++i)
			{
				mData[row1 * mNumCols + i] += mData[row2 * mNumCols + i] * scalar;
			}
		}

		void AddCol(const size_t col1, const size_t col2, const DataType scalar)
		{
			if (col1 < 0 || col1 >= mNumCols || col2 < 0 || col2 >= mNumCols)
			{
				throw std::invalid_argument("Columns must be within the bounds of the matrix.");
			}
			for (size_t i = 0; i < mNumRows; ++i)
			{
				mData[i * mNumCols + col1] += mData[i * mNumCols + col2] * scalar;
			}
		}

		Matrix RowEchelonForm() const
		{
			Matrix result(*this);
			size_t lead = 0;
			for (size_t r = 0; r < result.mNumRows; ++r)
			{
				if (result.mNumCols <= lead)
				{
					break;
				}
				size_t i = r;
				while (result(i, lead) == 0)
				{
					++i;
					if (result.mNumRows == i)
					{
						i = r;
						++lead;
						if (result.mNumCols == lead)
						{
							break;
						}
					}
				}
				if (result.mNumCols == lead)
				{
					break;
				}
				result.SwapRows(i, r);
				if (result(r, lead) != 0)
				{
					result.ScaleRow(r, 1 / result(r, lead));
				}
				for (size_t i = 0; i < result.mNumRows; ++i)
				{
					if (i != r)
					{
						result.AddRow(i, r, -result(i, lead));
					}
				}
				++lead;
			}
			return result;
		}
	};
} // namespace LAR