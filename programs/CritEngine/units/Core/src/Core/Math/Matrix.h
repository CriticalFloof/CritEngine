#pragma once

#include "../Core/Base.h"
#include <initializer_list>

namespace Engine {

	template<typename T, size_t Rows, size_t Columns>
	struct Matrix
	{
		static_assert(std::is_arithmetic<T>::value, "Matrix elements must be of a numeric type!");
		//Matrix Data is stored in column major order
		T data[Rows * Columns];

		Matrix() : data{} {};

		Matrix(T data[Rows * Columns]) {
			for (size_t i = 0; i < Rows * Columns; i++)
			{
				this->data[i] = data[i];
			}
		};

		Matrix(std::initializer_list<T> data) {
			for (size_t i = 0; i < Rows * Columns; i++)
			{
				this->data[i] = data.begin()[i];
			}
		};

		Matrix(const Matrix& base) { 
			for (size_t i = 0; i < Rows * Columns; i++)
			{
				this->data[i] = base.data[i];
			}
		};

		Matrix& operator=(const Matrix& other)
		{
			if (this != &other)
			{
				for (size_t i = 0; i < Rows * Columns; i++)
				{
					this->data[i] = other.data[i];
				}
			}
			return *this;
		}

		template<int R = Rows, int C = Columns>
		static std::enable_if_t<R == C, Matrix<T, Rows, Columns>> Identity()
		{
			Matrix out = Matrix();

			for (size_t i = 0; i < Rows; i++)
			{
				out.data[i * Rows + i] = 1;
			};

			return out;
		}

		//Static Math Operations

		static Matrix<T, Rows, Columns> Add(const Matrix<T, Rows, Columns>& first, const Matrix<T, Rows, Columns>& second)
		{
			Matrix out = Matrix();

			for (size_t i = 0; i < Rows * Columns; i++)
			{
				out.data[i] = first.data[i] + second.data[i];
			}

			return out;
		}

		template<size_t OtherColumns>
		static Matrix<T, Rows, OtherColumns> Mul(const Matrix<T, Rows, Columns>& first, const Matrix<T, Columns, OtherColumns>& second)
		{
			// Naive implementation
			Matrix<T, Rows, OtherColumns> out = Matrix<T, Rows, OtherColumns>();

			for (size_t i = 0; i < Rows; i++)
			{
				for (size_t j = 0; j < OtherColumns; j++)
				{
					out.data[j * Rows + i] = 0;
					for (size_t k = 0; k < Columns; k++)
					{
						out.data[j * Rows + i] += first.data[k * Rows + i] * second.data[j * Columns + k];
					}
				}
			}

			return out;
		}

		static Matrix<T, Rows, Columns> Add(const Matrix<T, Rows, Columns>& first, const T scalar)
		{
			Matrix out = Matrix();

			for (size_t i = 0; i < Rows * Columns; i++)
			{
				out.data[i] = first.data[i] + scalar;
			}

			return out;
		}

		static Matrix<T, Rows, Columns> Mul(const Matrix& base, const T scalar)
		{
			Matrix out = Matrix();

			for (size_t i = 0; i < Rows * Columns; i++)
			{
				out.data[i] = base.data[i] * scalar;
			}

			return out;
		}

		static Matrix<T, Columns, Rows> Transpose(const Matrix<T, Rows, Columns>& base)
		{
			Matrix out = Matrix<T, Columns, Rows>();

			for (size_t i = 0; i < Columns; i++)
			{
				for (size_t j = 0; j < Rows; j++)
				{
					out.data[i * Rows + j] = base.data[j * Columns + i];
				}
			}

			return out;
		}

		template<int R = Rows, int C = Columns>
		static std::enable_if_t<R == C, T> Determinant(const Matrix<T, Rows, Columns>& base)
		{

			Matrix m = Matrix(base);

			T det = 1.0;
			T temp;
			for (int i = 0; i < Rows; i++)
			{
				int pivot = i;
				for (size_t j = i + 1; j < Rows; j++)
				{
					if (abs(m.data[j * Rows + i]) > abs(m.data[pivot * Rows + i]))
					{
						pivot = j;
					}
				}
				if (pivot != i)
				{
					for (size_t k = 0; k < Rows; k++)
					{
						temp = m.data[i * Rows + k];
						m.data[i * Rows + k] = m.data[pivot * Rows + k];
						m.data[pivot * Rows + k] = temp;
					}
					det *= -1;
				}
				if (m.data[i * Rows + i] == 0)
				{
					return 0;
				}
				det *= m.data[i * Rows + i];
				for (size_t j = i + 1; j < Rows; j++)
				{
					T factor = m.data[j * Rows + i] / m.data[i * Rows + i];
					for (size_t k = i + 1; k < Rows; k++)
					{
						m.data[j * Rows + k] -= factor * m.data[i * Rows + k];
					}
				}
			}
			return det;
		}

		template<int R = Rows, int C = Columns>
		static std::enable_if_t<R == C, Matrix<T, Rows, Columns>> Inverse(const Matrix& base)
		{

			Matrix input = Matrix(base);
			Matrix inverse = Matrix::Identity();

			for (size_t i = 0; i < Rows; i++)
			{
				T pivot = input.data[i * Rows + i];
				ASSERT(pivot != 0, "Inverse matrix doesn't exist!");

				for (size_t j = 0; j < Rows; j++)
				{
					input.data[i * Rows + j] /= pivot;
					inverse.data[i * Rows + j] /= pivot;
				}

				for (size_t j = 0; j < Rows; j++)
				{
					if (i != j)
					{
						T scale = input.data[j * Rows + i];
						for (size_t k = 0; k < Rows; k++)
						{
							input.data[j * Rows + k] -= scale * input.data[i * Rows + k];
							inverse.data[j * Rows + k] -= scale * inverse.data[i * Rows + k];
						}
					}
				}
			}

			return inverse;
		}



		static bool IsEqual(const Matrix<T, Rows, Columns>& first, const Matrix<T, Rows, Columns>& second)
		{
			for (size_t i = 0; i < Rows * Columns; i++)
			{
				if (first.data[i] != second.data[i]) return false;
			}
			return true;
		}

		//Object Math Operations
		Matrix Add(Matrix& other) const { return Matrix::Add(*this, other); }
		Matrix Mul(Matrix& other) const { return Matrix::Mul(*this, other); }

		Matrix Add(const T scalar) const { return Matrix::Add(*this, scalar); }
		Matrix Mul(const T scalar) const { return Matrix::Mul(*this, scalar); }

		Matrix Transpose() const { return Matrix::Transpose(*this); }
		template<size_t R = Rows, size_t C = Columns>
		std::enable_if_t<R == C, T> Determinant() const { return Matrix::Determinant(*this); }
		template<size_t R = Rows, size_t C = Columns>
		std::enable_if_t<R == C, Matrix<T, Rows, Columns>> Inverse() const { return Matrix::Inverse(*this); }

		bool IsEqual(Matrix& other) const { return Matrix::IsEqual(*this, other); }

		template<size_t OtherColumns>
		Matrix<T, Rows, OtherColumns> operator*(const Matrix<T, Columns, OtherColumns>& other) { return Matrix::Mul(*this, other); }
	};

	
	using Matrix2f = Matrix<float, 2, 2>;
	template struct ENGINE_API Matrix<float, 2, 2>;
	using Matrix3f = Matrix<float, 3, 3>;
	template struct ENGINE_API Matrix<float, 3, 3>;
	using Matrix4f = Matrix<float, 4, 4>;
	template struct ENGINE_API Matrix<float, 4, 4>;

	using Matrix2d = Matrix<double, 2, 2>;
	template struct ENGINE_API Matrix<double, 2, 2>;
	using Matrix3d = Matrix<double, 3, 3>;
	template struct ENGINE_API Matrix<double, 3, 3>;
	using Matrix4d = Matrix<double, 4, 4>;
	template struct ENGINE_API Matrix<double, 4, 4>;
}