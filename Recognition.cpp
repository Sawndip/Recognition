#include "Recognition.h"

Recognition::Recognition(const char *directoryPath, const char *noisyPath) {
    loadTemplates(directoryPath);
    this->Y = MatrixClass(getVectorFromFile(noisyPath));
    std::cout << "input vector: " << std::endl;
    Y.show();
    std::cout << "Image to recognize: " << std::endl;
    Y.beautifulVisualization(N);
    this->W = MatrixClass(N , N);
    showImages();
    calculateWeights();
    recognition();
}

void Recognition::loadTemplates(const char *directoryPath) {
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (directoryPath)) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            std::string file = ent->d_name;
            if (file.substr(file.find_last_of('.') + 1) == TXT_EXTENSION) {
                std::string filePath = directoryPath;
                filePath += "/" + file;
                std::vector<double> tmp = getVectorFromFile(filePath.c_str());
                MatrixClass pattern(tmp);
                templates.push_back(pattern);
            }
        }
        closedir (dir);
    } else {
        perror ("could not open directory");
        exit(EXIT_FAILURE);
    }
}

std::vector<double> Recognition::getVectorFromFile(const char *file) {
    std::vector<double> vector;
    std::ifstream input(file);
    if (!input.is_open()) {
        std::logic_error wrongFileName("File reading error");
        exit(EXIT_FAILURE);
    } else {
        double number;
        while (input >> number) {
            vector.push_back(number);
        }
        input.close();
    }
    return vector;
}

void Recognition::calculateWeights() {
    bool isRelaxation;
    do {
        for (MatrixClass image : templates) {
            W = W + image.transpose() * (image - image * W )  * (0.8 / N);
        }
        W.nullifyMainDiagonal();
        isRelaxation = isNetworkInRelaxation();
    } while (!isRelaxation);
}

void Recognition::recognition() {
    bool didRecognize;
    int iteration = 0;
    do {
        MatrixClass Y_ = Y * W;
        Y = Y_.activationFunction();
        didRecognize = didFindAnswer(Y);
        history.push_back(Y);
        iteration++;
        std::cout<< "\nIteration: " << iteration << std::endl;
        Y.beautifulVisualization(N); printf("\n");
    } while(!didRecognize);
    showAnswer(iteration);
}

void Recognition::showAnswer(int iteration) {
    auto size = history.size();
    MatrixClass Xi = history[size - 2];
    MatrixClass Xi_plus1 = history[size - 1];
    if (Xi != Xi_plus1) {
        printf("You got dynamic attractor!\n");
        std::cout<< "After " << iteration - 1 << " iteration(s) Xi = "; Xi.show();
        Xi.beautifulVisualization(N);
        std::cout<< "After " << iteration << " iteration(s) Xi+1 = "; Xi_plus1.show();
        Xi_plus1.beautifulVisualization(N);
    }
    else {
        printf("You got static attractor!\n");
        std::cout << "After " << iteration << " iteration(s) recognized image is: " << std::endl;
        Y = history[history.size() - 2];
        Y.beautifulVisualization(N);
        std::cout << "Output vector is: "; Y.show();
    }
}

bool Recognition::didFindAnswer(const MatrixClass &matrix) {
    for (const MatrixClass &image : history) {
        if (matrix == image) {
            return true;
        }
    }
    return false;
}

void Recognition::showImages() {
    std::cout << "Images-templates are: " << std::endl;
    for (MatrixClass image : templates) {
        image.beautifulVisualization(N);
        std::cout << std::endl;
    }
}

bool Recognition::isNetworkInRelaxation() {
    for (MatrixClass image : templates) {
        printf("Expected: "); printf("\n");
        image.beautifulVisualization(N); printf("\n");
        printf("Got: "); printf("\n");
        MatrixClass image_ = (image * W).activationFunction();
        image_.beautifulVisualization(N); printf("\n");
        if (image.activationFunction() != image_) {
            return false;
        }
    }
    return true;
}