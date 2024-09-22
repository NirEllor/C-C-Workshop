// Dense.h
#ifndef DENSE_H
#define DENSE_H
#include "Activation.h"
class Dense
{
public:
/**
* Constructor of a single layer in the network
* @param weights matrix
* @param bias matrix
*/
Dense(const Matrix &weights, const Matrix &bias,active Function);
/**
*
* @return The weights matrix
*/
const Matrix &get_weights() const;
/**
*
* @return The bias matrix (vector)
*/
const Matrix &get_bias() const;
/**
*
* @return pointer to the relevant activation function - whether relu
* or softmax
*/
active get_activation() const;
/**
* Perform ActivationFunction(weights * input + bias)
* @param input vector, in the first layer it's the picture itself
* @return a matrix which is f(w * x + n)
*/
Matrix operator()(Matrix input) const;
private:
    Matrix _weights;
    Matrix _bias;
    active _func;

};
#endif //DENSE_H