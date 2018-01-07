//
// Created by Alexei Sevko on 11/2/17.
//

#include "MatrixClass.h"

void MatrixClass::beautifulVisualization(const double &n) {
    MatrixClass thisT = *this;
    thisT = thisT.transpose();
    MatrixClass toVisualize(thisT , (unsigned int) sqrt(n));
    toVisualize.visualize();
}

void MatrixClass::visualize() {
    for (unsigned int i = 0; i < sizeX; i++) {
        for (unsigned int j = 0; j < sizeY; ++j) {
            if (matrix[sizeX * i + j] > 0) {
                std::cout << "#";
            } else {
                std::cout << ".";
            }
            if (j == sizeY - 1)
                std::cout << std::endl;
        }
    }
}

void MatrixClass::show() {
    for (unsigned int i = 0; i < sizeX; i++) {
        for (unsigned int j = 0; j < sizeY; ++j) {
            if (matrix[sizeX * i + j] < 0)
                std::cout << matrix[sizeX * i + j] << " ";
            else
                std::cout << " " << matrix[sizeX * i + j] << " ";
            if (j == sizeY - 1)
                std::cout << std::endl;
        }
    }
}

MatrixClass MatrixClass::transpose() {
    MatrixClass result(sizeY, sizeX);
    for (unsigned int i = 0; i < result.sizeX; i++) {
        for (unsigned int j = 0; j < result.sizeY; j++) {
            result(i, j) = matrix[sizeX * i + j];
        }
    }
    return result;
}

MatrixClass& MatrixClass::activationFunction() {
    for (unsigned int i = 0; i < this->sizeX * this->sizeY; i ++) {
        this->matrix[i] = tanh(this->matrix[i]);
    }
    return *this;
}

void MatrixClass::resize(unsigned int sizeX, unsigned int sizeY) {
    if (sizeX < 1 || sizeY < 1 ||
            sizeX < this->sizeX ||
            sizeY < this->sizeY) {
        throw std::logic_error("Negative matrix sizes");
    }
    this->sizeX = sizeX;
    this->sizeY = sizeY;
    this->matrix.resize(sizeX * sizeY);
}

void MatrixClass::insert(const double& value) {
    this->sizeY += 1;
    this->matrix.resize(this->sizeX * this->sizeY);
    this->matrix.insert(this->matrix.begin(), value);
}

void MatrixClass::nullifyMainDiagonal() {
    if (this->sizeX != this->sizeY) {
        throw std::logic_error("Not square matrix");
    }
    for (unsigned int i = 0; i < this->sizeX; i++) {
        for (unsigned int j = 0; j < this->sizeY; j++) {
            if (i == j ) {
                matrix[sizeX * i + j] = 0;
            }
        }
    }
}

double MatrixClass::sumABS() {
    double result = 0;
    for (unsigned int i = 0; i < sizeX * sizeY; i++) {
        result += abs(matrix[i]);
    }
    return result;
}