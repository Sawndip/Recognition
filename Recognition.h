#ifndef RECOGNITION_RECOGNITION_H
#define RECOGNITION_RECOGNITION_H
#include <fstream>
#include <algorithm>
#include <dirent.h>
#include <random>
#include "MatrixClass.h"
#define N   49
#define TXT_EXTENSION "txt"


class Recognition {
private:
    std::vector<MatrixClass> templates;
    std::vector<unsigned int> randomIndexes;
    MatrixClass X;
    MatrixClass W;
    int iterations;
    int recIterations;

    std::vector<double> getVectorFromFile(const char *file);
    void loadTemplates(const char *directoryPath);
    void calculateWeights();
    void recognition();
    void showImages();
    void showAnswer(int i);
    void generateRandomIndexes();
    unsigned int getRandomIndex(int index);
    void doIteration(MatrixClass &X);
public:
    Recognition(){}
    Recognition(const char *directoryPath, const char *noisyPath);
};


#endif //RECOGNITION_RECOGNITION_H
