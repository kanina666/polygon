#ifndef POLYGON_LABEL_ENCODER_H
#define POLYGON_LABEL_ENCODER_H

#include <vector>
#include <string>
#include <fstream>

// Объявление функции label_encode
std::vector<int> label_encode(std::ifstream& input);

#include "label_encoder.cpp"

#endif // POLYGON_LABEL_ENCODER_H
