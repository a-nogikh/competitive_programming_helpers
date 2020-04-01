/*
	A matrix class
*/

#include <cassert>

template<class T>
class Matrix
{
private:
	int rows = 0, cols = 0;
	vector<vector<T>> matrix;
public:
	Matrix(const int rows, const int cols)
	{
		this->cols = cols;
		this->rows = rows;
		matrix = vector<vector<T>>(rows, vector<T>(cols, 0));
	}

	Matrix(const std::initializer_list<std::initializer_list<T>> &data)
	{
		this->rows = data.size();
		if (this->rows == 0)
			return;

		matrix.resize(this->rows);

		auto ptr = data.begin();
		auto row_ptr = matrix.begin();

		this->cols = ptr->size();
		
		while (ptr != data.end())
		{
			if (ptr->size() != this->cols)
				throw exception("Inconsistent row sizes!");

			row_ptr->insert(row_ptr->end(), ptr->begin(), ptr->end());
			ptr++, row_ptr++;
		}
	}

	Matrix multiply(const Matrix& other) const
	{
		assert(cols == other.rows);

		Matrix ans(rows, other.cols);
		for (int row = 0; row < rows; row++)
		{
			for (int col = 0; col < other.cols; col++)
			{
				for (int k = 0; k < cols; k++)
				{
					ans.matrix[row][col] += matrix[row][k] * other.matrix[k][col];
				}
			}
		}

		return ans;
	}

	Matrix pow(const long long exp) const
	{
		if (exp == 1)
			return *this;

		if (exp % 2 == 0)
		{
			Matrix tmp = pow(exp / 2);
			return tmp.multiply(tmp);
		}
		else
		{
			Matrix tmp = pow(exp - 1);
			return multiply(tmp);
		}
	}

	Matrix operator * (const Matrix& other) const
	{
		return this->multiply(other);
	}

	vector<T>& operator[] (int row) 
	{
		return matrix[row];
	}
};