#include "MlpNetwork.h"
 using namespace activation;

 MlpNetwork::MlpNetwork(Matrix weights[MLP_SIZE], Matrix biases[MLP_SIZE]) :

 layer1(weights[0], biases[0],relu),
 layer2(weights[1], biases[1],relu),
 layer3(weights[2], biases[2],relu),
 layer4(weights[3], biases[3],softmax)
 {}


digit MlpNetwork::operator()(const Matrix &img)
{
    Matrix r1 = layer1(img);
    Matrix r2 = layer2(r1);
    // Error occurs
    Matrix r3 = layer3(r2);
    Matrix r4 = layer4(r3);
    unsigned int number_index = r4.argmax();
    digit chosen{number_index, r4[number_index]};
    return chosen;
}