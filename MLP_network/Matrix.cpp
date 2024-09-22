#include "Matrix.h"
#define SMALL 0.1

Matrix::Matrix(int _rows, int _columns) : rows(_rows), columns(_columns)
{
    if (rows > 0 && columns > 0)
    {
        matrix = new float[rows * columns]();
    }
    else
    {
        throw exception();
    }
}
Matrix::Matrix(): Matrix(1, 1){}

Matrix::Matrix(const Matrix& other): rows(other.rows), columns(other.columns),
matrix(new float [other.rows * other.columns]())
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < columns; ++j)
        {
            matrix[i * columns + j] = other.matrix[i * columns + j];
        }
    }
}

Matrix::~Matrix()
{
    delete []matrix;
}

int Matrix::get_rows() const
{
    return rows;
}

int Matrix::get_cols() const
{
    return columns;
}

Matrix &Matrix::operator=(Matrix other)
{
    swap(rows, other.rows);
    swap(columns, other.columns);
    swap(matrix, other.matrix);
    return *this;
}

Matrix &Matrix::transpose()
{
    Matrix tmp_matrix(get_cols(), get_rows());
    for (int i = 0; i < get_rows(); ++i)
    {
        for (int j = 0; j < get_cols(); ++j)
        {
            tmp_matrix(j, i) = matrix[i * get_cols() + j];
        }
    }
    *this = tmp_matrix;
    return *this;

}


Matrix &Matrix::vectorize() // Flatten to row vector ant then transpose it
{
//    return *this;
    Matrix tmp(1, get_rows()* get_cols());
    int idx = 0;
    for (int i = 0; i < get_rows(); ++i)
    {
        for (int j = 0; j < get_cols(); ++j)
        {
            tmp.matrix[idx++] = matrix[i * get_cols() + j];

        }
    }
    *this = tmp.transpose();
    return *this;
}

void Matrix::plain_print() const
{
    for (int i = 0; i < get_rows(); ++i)
    {
        for (int j = 0; j < columns; ++j)
        {
            cout<<matrix[i * get_cols() + j]<<" ";
        }
        cout<<endl;
    }
}

Matrix Matrix::dot(Matrix &other) const
{
    Matrix dot_matrix(get_rows(), columns);
    for (int i = 0; i < get_rows(); ++i)
    {
        for (int j = 0; j < columns; ++j)
        {
            dot_matrix.matrix[i * get_cols() + j] =
                    matrix[i * get_cols() + j] *
                    other.matrix[i * other.get_cols() + j];
        }
    }
    return dot_matrix;
}

float Matrix::norm() const
{
    float sum = 0;
    for (int i = 0; i < get_rows(); ++i)
    {
        for (int j = 0; j < columns; ++j)
        {
            sum+=(pow(matrix[i * get_cols() + j], 2));
        }
    }
    return sqrt(sum);
}
void Matrix::swap_rows(int row1, int row2)
{
    for (int j = 0; j < columns; ++j)
    {
        swap(matrix[row1 * columns + j], matrix[row2 * columns + j]);
    }
}

void Matrix::scale_row(int row, float scalar)
{
    for (int j = 0; j < columns; ++j)
    {
        matrix[row * columns + j] *= scalar;
    }
}

void Matrix::add_multiple_of_row(int destRow, int sourceRow, float scalar)
{
    for (int j = 0; j < columns; ++j)
    {
        matrix[destRow * columns + j] += scalar * matrix[sourceRow * columns
                                                         + j];
    }
}

Matrix Matrix::rref() const
{
    Matrix reduced(*this);
    int lead = 0; // leading element at each column
    for (int r = 0; r < get_rows(); ++r) // through each row
    {
        if (lead >= columns) // We passed all columns
        {
            return reduced; // After no manipulations
        }
        int i = r; //current row index
        while (reduced.matrix[i * get_cols() + lead] == 0)
        {// Searching nonzero entry in the current column
            ++i; // move to next row
            if (get_rows() == i) // No nonzero entry found
            {
                i = r; // Reset i to r for the next column
                ++lead; // move to next column
                if (reduced.columns == lead) // Passed all columns
                {
                    return reduced; //  No more manipulations
                }
            }
        } // Nonzero entry found, r is the current row, "i" assigned to r
        reduced.swap_rows(i, r); //Nonzero entry row and current
        // row
        float r_entry = reduced.matrix[r * get_cols() + lead];//Entry in r
        reduced.scale_row(r, 1.0 / r_entry); // Make lv be 1
        for (int j = 0; j < get_rows(); ++j)
        { // Make all entries above and beyond lv to be 0
            if (j != r) // Except lv itself, which is in row r
            { // lv temp will store entry in r value for each other row
                float r_entry_temp = reduced.matrix[j * get_cols() + lead];
                reduced.add_multiple_of_row(j, r,
                                         -r_entry_temp);
            }
        }
        ++lead; // move to next column after manipulations
    }
    return reduced; // After manipulations
}

int Matrix::argmax()
{
    int max_idx = 0;
    float max_val = 0;
    for (int i = 0; i < get_rows(); ++i)
    {
        for (int j = 0; j < columns; ++j)
        {
            float cur = matrix[i * get_cols() + j];
            if ( cur > max_val)
            {
                max_val = cur;
                max_idx = i * get_cols() + j;
            }
        }
    }
    return max_idx;
}

float Matrix::sum()
{
    float sum_val = 0;
    for (int i = 0; i < get_rows(); ++i)
    {
        for (int j = 0; j < columns; ++j)
        {
            sum_val += matrix[i * get_cols() + j];
        }
    }
    return sum_val;
}

Matrix& operator+=(Matrix &lhs, const Matrix &rhs)
{
    if (lhs.get_rows() != rhs.get_rows() ||
    lhs.columns != rhs.columns) // Addition not defined
    {
        throw exception();
    }
    for (int i = 0; i < lhs.get_rows(); ++i)
    {
        for (int j = 0; j < lhs.columns; ++j)
        {
          int idx = i * lhs.get_cols() + j;
          lhs.matrix[idx] += rhs.matrix[idx];
        }
    }
    return lhs;
}

Matrix operator+(const Matrix &lhs, const Matrix &rhs)
{
    Matrix result(lhs);
    result += rhs;
    return result;
}


Matrix operator*(const Matrix &lhs, const Matrix &rhs)
{
    if (lhs.columns != rhs.get_rows()) // Multiplication is not defined
    {
        throw exception();
    }
    Matrix multiplication(lhs.get_rows(), rhs.columns);
    for (int i = 0; i < multiplication.get_rows(); ++i)
    {
        for (int j = 0; j < multiplication.columns; ++j)
        {
            float num = 0.0;
            for (int k = 0; k < lhs.columns; ++k)
            {
                num += lhs.matrix[i * lhs.get_cols() + k] *
                        rhs.matrix[k * rhs.get_cols() + j];
            }
            multiplication.matrix[i * multiplication.get_cols() + j] = num;
        }
    }
    return multiplication;
}

Matrix Matrix::operator*(float scalar) const
{
    Matrix result(get_rows(), columns);
    for (int i = 0; i < get_rows(); ++i)
    {
        for (int j = 0; j < columns; ++j)
        {
            result.matrix[i * get_cols() + j] =
                    matrix[i * get_cols() + j] * scalar;
        }
    }
    return result;
}

Matrix operator*(float scalar, Matrix rhs)
{
    Matrix result(rhs.get_rows(), rhs.columns);
    for (int i = 0; i < rhs.get_rows(); ++i)
    {
        for (int j = 0; j < rhs.columns; ++j)
        {
            result.matrix[i * rhs.get_cols() + j] =
                    scalar * rhs.matrix[i * rhs.get_cols() + j];
        }
    }
    return result;
}

float &Matrix::operator()(int row, int column)
{
    if (row >= get_rows() || column >= columns)//Index out of range
    {
        throw exception();
    }
    return matrix[row * get_cols() + column];
}

const float &Matrix::operator()(int row, int column) const
{
    if (row > get_rows() || column > columns)//Index out of range
    {
        throw exception();
    }
    return matrix[row * get_cols() + column];
}

float &Matrix::operator[](int idx)
{
    if (idx >= get_rows() * columns) //Index out of range
    {
        throw exception();
    }
    return matrix[idx];
}

const float &Matrix::operator[](int idx) const
{
    if (idx >= get_rows() * columns)//Index out of range
    {
        throw exception();
    }
    return matrix[idx];
}

ostream &operator<<(ostream &out, const Matrix &rhs)
{
    for (int i = 0; i < rhs.get_rows(); ++i)
    {
        for (int j = 0; j < rhs.columns; ++j)
        {
            if (rhs(i, j) > SMALL)
            {
                out<<"**";
            }
            else
            {
                out<<"  ";
            }
        }
        out<<endl;

    }
    return out;
}

ifstream &operator>>(ifstream &in, Matrix &rhs)
{
    rhs.read_from_stream(in);
    return in;
}

void Matrix::read_from_stream(ifstream &in)
{
    if (!in.is_open())
    {
        throw exception(); //Couldn't open the file
    }
    in.seekg (0, std::ios_base::end);
    size_t length = in.tellg();
    in.seekg (0, std::ios_base::beg);
    if (length < get_rows() * columns * sizeof(float))
    {
        throw exception(); // Matrix is larger than binary input
    }
    char *buffer = new char [length];
    in.read(buffer, length);
    if (!in)
    {
        delete[] buffer;
        throw exception(); // Couldn't read all the file
    }
    float* float_buffer = reinterpret_cast<float*>(buffer);
    for (size_t i = 0; i < length/ sizeof(float); ++i)
    {
        if (typeid(float_buffer[i]) != typeid(float))
        {
            delete[] buffer;
            throw exception(); // Element in the buffer is not a float
        }
    }
    for (int i = 0; i < get_rows(); ++i)
    {
        for (int j = 0; j < columns; ++j)
        {
            (*this)(i, j) = float_buffer[i * get_cols() + j];
        }
    }
    delete[] buffer;
}
