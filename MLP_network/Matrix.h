// Matrix.h
#ifndef MATRIX_H
#define MATRIX_H
#include <iostream>
#include <istream>
#include <cmath>
#include <fstream>
using namespace std;

// You don't have to use the struct. Can help you with MlpNetwork.h
struct matrix_dims {
    int rows, cols;
};


// Insert Matrix class here...
class Matrix
{
public:
    /**
     * Constructor of a mxn 1D matrix
     * @param rows
     * @param columns
     */
    Matrix(int rows, int columns);
    /**
     * Default constructor of a 1x1 1D matrix
     */
    Matrix();
    /**
     * Copy constructor
     * @param other mxn matrix
     */
    Matrix(const Matrix& other);
    /**
     * Destructor
     */
    ~Matrix();
    /**
     * Assignment operator
     * @param other matrix
     * @return
     */
    Matrix &operator=(Matrix other);
    /**
     *
     * @return number of rows
     */
    int get_rows() const;
    /**
     *
     * @return number of columns
     */
    int get_cols() const;
    /**
     *
     * @return reference to the given matrix, transposed
     */
    Matrix &transpose();
    /**
     *
     * @return reference to the given matrix, vectorized
     */
    Matrix &vectorize();
    /**
     * Prints the matrix elements
     */
    void plain_print() const;
    /**
     * Returns hadamard product of this' matrix and other's matrix
     * @param other
     * @return new dot_matrix
     */
    Matrix dot(Matrix &other) const;
    /**
     *
     * @return the norm of the matrix
     */
    float norm() const;
    /**
     *
     * @return reduced row echelon form of the matrix
     */
    Matrix rref() const;
    /**
     *
     * @return index with max value
     */
    int argmax();
    /**
     *
     * @return sum of all the elements in the matrix
     */
    float sum();
    friend Matrix& operator+=(Matrix &lhs, const Matrix &rhs);
    friend Matrix operator+(const Matrix &lhs, const Matrix &rhs);
    friend Matrix operator*(const Matrix &lhs, const Matrix &rhs);
    /**
     *
     * @param scalar
     * @return new matrix, scalar multiplied
     */
    Matrix operator*(float scalar) const;
    friend Matrix operator*(float scalar, Matrix rhs);
    /**
     *
     * @param row
     * @param column
     * @return element in matrix[row][column]
     */
    float& operator()(int row, int column);
    /**
     *
     * @param row
     * @param column
     * @return const element in matrix[row][column]
     */
    const float& operator()(int row, int column) const;
    /**
     *
     * @param idx
     * @return matrix[idx] after vectorized
     */
    float& operator[](int idx);
    /**
     *
     * @param idx
     * @return const matrix[idx] after vectorized
     */
    const float& operator[](int idx) const;

    friend ostream& operator<<(ostream& out, const Matrix &rhs);

    friend ifstream& operator>>(ifstream & in, Matrix &rhs);


private:
    int rows;
    int columns;
    float * matrix;
    void swap_rows(int row1, int row2);
    void scale_row(int row, float scalar);
    void add_multiple_of_row(int destRow, int sourceRow, float scalar);
    void read_from_stream(ifstream & in);

};
/**
 *
 * @param lhs
 * @param rhs
 * @return lhs, added by rhs
 */
Matrix& operator+=(const Matrix &lhs, const Matrix &rhs);
/**
 *
 * @param lhs
 * @param rhs
 * @return new matrix, lhs + rhs
 */
Matrix operator+(const Matrix &lhs, const Matrix &rhs);
/**
 *
 * @param lhs
 * @param rhs
 * @return new matrix, lhs * rhs
 */
Matrix operator*(const Matrix &lhs, const Matrix &rhs);
/**
 *
 * @param scalar
 * @param rhs
 * @return scalar multiplied by rhs
 */
Matrix operator*(float scalar, Matrix rhs);
/**
 *
 * @param out
 * @param rhs
 * @return pretty print of the image
 */
ostream& operator<<(ostream& out, const Matrix &rhs);
/**
 *
 * @param in
 * @param rhs
 * @return fills matrix cells by reading a binary file,
 * doesn't assume it's valid
 */
ifstream& operator>>(ifstream & in, const Matrix &rhs);
#endif //MATRIX_H