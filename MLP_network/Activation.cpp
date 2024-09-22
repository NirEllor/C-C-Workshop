#include "Activation.h"

namespace activation
{
    Matrix softmax(const Matrix &m)
    {
        Matrix result(m.get_rows(), m.get_cols());

        // Compute the sum of exponential of all elements in the matrix
        float sum_exp = 0.0;
        for (int i = 0; i < m.get_rows(); ++i)
        {
            for (int j = 0; j < m.get_cols(); ++j)
            {
                sum_exp += exp(m(i, j)); // Access element
                // (i, j) in the matrix
            }
        }

        // Compute softmax for each element in the matrix
        for (int i = 0; i < m.get_rows(); ++i)
        {
            for (int j = 0; j < m.get_cols(); ++j)
            {
                result(i, j) = exp(m(i, j)) /
                        sum_exp;
            }
        }

        return result;
    }
    Matrix relu(const Matrix &m)
    {
        Matrix result(m.get_rows(), m.get_cols());
        for (int i = 0; i < m.get_cols(); ++i)
        {
            for (int j = 0; j < m.get_rows(); ++j)
            {
                result(j, i) = m(j, i) >= 0
                        ? m(j, i): 0;
            }
        }
        return result;
    }
}