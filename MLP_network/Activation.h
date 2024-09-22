// Activation.h
#ifndef ACTIVATION_H
#define ACTIVATION_H

#include "Matrix.h"

typedef Matrix (*active)(const Matrix&);

// Insert Activation namespace here...
namespace activation
{
    /**
     * Perform softmax algorithm on m
     * @param m belongs to R**n
     * @return vector belongs to R**N
     */
    Matrix softmax(const Matrix &m);
    /**
     * Perform relu algorithm on m
     * @param m belongs to R**n
     * @return vector belongs to R**N
     */
    Matrix relu(const Matrix &m);
}

#endif //ACTIVATION_H