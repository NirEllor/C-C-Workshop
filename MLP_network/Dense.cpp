 #include "Dense.h"
 Dense::Dense(const Matrix &weights, const Matrix &bias, active Function) :
 _weights(weights), _bias(bias), _func(Function) {}

 const Matrix &Dense::get_weights() const
 {
    return _weights;
 }

 const Matrix &Dense::get_bias() const
 {
    return _bias;
 }

 active Dense::get_activation() const
 {
    return _func;
 }

 Matrix Dense::operator()(Matrix input) const
 {
    Matrix result = get_weights() * input + get_bias();
    result = _func(result);
    return result;
}