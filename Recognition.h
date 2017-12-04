#ifndef RECOGNITION_RECOGNITION_H
#define RECOGNITION_RECOGNITION_H
#include <fstream>
#include <dirent.h>
#include "MatrixClass.h"
#define N   7
#define TXT_EXTENSION "txt"


class Recognition {
private:
    std::vector<MatrixClass> templates;
    MatrixClass Y;
    MatrixClass W;

    std::vector<double> readFromFile(const char *file);
    void loadTemplates(const char *directoryPath);
    void calculateWeights();
    void recognition();
    void showImages();
    bool didFindAnswer(const MatrixClass &matrix);
public:
    Recognition(){}
    Recognition(const char *directoryPath, const char *noisyPath);
    inline std::vector<MatrixClass> getTemplates() { return this->templates; }
};


#endif //RECOGNITION_RECOGNITION_H
