#pragma once
#include "MatrixBase.h"
#include "LAR_export.h"
#include "Algorithms.h"

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
			RowView(DataType* data, const int cols)
				: mData(data), mCols(cols)
			{
			}
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
			: MatrixBase<Matrix<DataType>, DataType>(rows, cols) {}

		Matrix(const MatrixBase<Matrix<DataType>, DataType>& other)
			: MatrixBase<Matrix<DataType>, DataType>(other) {}

		RowView operator[](const int row)
		{
			return RowView(this->mData + row * this->mNumCols, this->mNumCols);
		}
		RowView operator[](const int row) const
		{
			return RowView(this->mData + row * this->mNumCols, this->mNumCols);
		}
		ColView getCol(const int col)
		{
			return ColView(this->mData + col, this->mNumRows, this->mNumCols);
		}
		ColView getCol(const int col) const
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
			for (int i = 0; i < this->mNumRows; ++i)
			{
				for (int j = 0; j < other.mNumCols; ++j)
				{
					result(i, j) = 0;
					for (int k = 0; k < this->mNumCols; ++k)
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
			for (int i = 0; i < this->mNumRows; ++i)
			{
				result[i] = 0;
				for (int j = 0; j < this->mNumCols; ++j)
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
			for (int i = 0; i < mNumRows; ++i)
			{
				for (int j = 0; j < mNumCols; ++j)
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
			for (int i = 0; i < mNumRows; ++i)
			{
				for (int j = 0; j < mNumCols; ++j)
				{
					if (mData[i * mNumCols + j] != mData[j * mNumCols + i])
					{
						return false;
					}
				}
			}
			return true;
		}

		Matrix Minor(const int row, const int col) const
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
			for (int i = 0; i < mNumRows; ++i)
			{
				for (int j = 0; j < mNumCols; ++j)
				{
					if (i != row && j != col)
					{
						int iOffset = i < row ? 0 : -1;
						int jOffset = j < col ? 0 : -1;
						result(i + iOffset, j + jOffset) = mData[i * mNumCols + j];
					}
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
		static Matrix Fill(const int rows, const int cols, const DataType value)
		{
			Matrix result(rows, cols);
			for (int i = 0; i < rows * cols; ++i)
			{
				result.mData[i] = value;
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

		DataType Trace() const
		{
			if (!IsSquare())
			{
				throw std::invalid_argument("Matrix must be square to find the trace.");
			}
			DataType sum = 0;
			for (int i = 0; i < mNumRows; ++i)
			{
				sum += mData[i * mNumCols + i];
			}
			return sum;
		}

		void SwapRows(const int row1, const int row2)
		{
			if (row1 < 0 || row1 >= mNumRows || row2 < 0 || row2 >= mNumRows)
			{
				throw std::invalid_argument("Rows must be within the bounds of the matrix.");
			}
			for (int i = 0; i < mNumCols; ++i)
			{
				std::swap(mData[row1 * mNumCols + i], mData[row2 * mNumCols + i]);
			}
		}

		void SwapCols(const int col1, const int col2)
		{
			if (col1 < 0 || col1 >= mNumCols || col2 < 0 || col2 >= mNumCols)
			{
				throw std::invalid_argument("Columns must be within the bounds of the matrix.");
			}
			for (int i = 0; i < mNumRows; ++i)
			{
				std::swap(mData[i * mNumCols + col1], mData[i * mNumCols + col2]);
			}
		}

		void ScaleRow(const int row, const DataType scalar)
		{
			if (row < 0 || row >= mNumRows)
			{
				throw std::invalid_argument("Row must be within the bounds of the matrix.");
			}
			for (int i = 0; i < mNumCols; ++i)
			{
				mData[row * mNumCols + i] *= scalar;
			}
		}

		void ScaleCol(const int col, const DataType scalar)
		{
			if (col < 0 || col >= mNumCols)
			{
				throw std::invalid_argument("Column must be within the bounds of the matrix.");
			}
			for (int i = 0; i < mNumRows; ++i)
			{
				mData[i * mNumCols + col] *= scalar;
			}
		}

		void AddRow(const int row1, const int row2, const DataType scalar)
		{
			if (row1 < 0 || row1 >= mNumRows || row2 < 0 || row2 >= mNumRows)
			{
				throw std::invalid_argument("Rows must be within the bounds of the matrix.");
			}
			for (int i = 0; i < mNumCols; ++i)
			{
				mData[row1 * mNumCols + i] += mData[row2 * mNumCols + i] * scalar;
			}
		}

		void AddCol(const int col1, const int col2, const DataType scalar)
		{
			if (col1 < 0 || col1 >= mNumCols || col2 < 0 || col2 >= mNumCols)
			{
				throw std::invalid_argument("Columns must be within the bounds of the matrix.");
			}
			for (int i = 0; i < mNumRows; ++i)
			{
				mData[i * mNumCols + col1] += mData[i * mNumCols + col2] * scalar;
			}
		}

		Matrix RowEchelonForm() const
		{
			Matrix result(*this);
			int lead = 0;
			for (int r = 0; r < result.mNumRows; ++r)
			{
				if (result.mNumCols <= lead)
				{
					break;
				}
				int i = r;
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
				for (int i = 0; i < result.mNumRows; ++i)
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