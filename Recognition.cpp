#include "Recognition.h"

#include <random>

Recognition::Recognition(const char *directoryPath, const char *noisyPath) {
    loadTemplates(directoryPath);
    this->X = MatrixClass(getVectorFromFile(noisyPath));
    std::cout << "input vector: " << std::endl;
    X.show();
    std::cout << "Image to recognize: " << std::endl;
    X.beautifulVisualization(N);
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
    double e = 0.000000001;
    double h = 0.8;
    double change;
    std::vector<double> change_;
    do {
        iterations++;
        change = 0;
        for (MatrixClass image : templates) {
            MatrixClass Xi = image.transpose();
            MatrixClass deltaW = (Xi - W * Xi ) * Xi.transpose()  * (h / N);
            deltaW.nullifyMainDiagonal();
            W = W + deltaW;
            change += deltaW.sumABS();
        }
        change_.push_back(change);
        if (change_.size() > 1) {
            change = abs(change - change_[change_.size() - 2]);
        }
    } while (change > e);
    std::cout << "Iteration: " << iterations << std::endl;
}

void Recognition::recognition() {
    srand (time(nullptr));
    bool relaxation = false;
    MatrixClass prev;
    do {
        generateRandomIndexes();
        doIteration(X);
        if (prev == X) {
            relaxation = true;
        } else {
            prev = X;
        }
        recIterations++;
    } while (!relaxation);
    showAnswer(recIterations);
}

void Recognition::doIteration(MatrixClass &X) {
    do {
        unsigned int index = getRandomIndex();
        MatrixClass Xi = X * W;
        X(0, index) = Xi(0, index);
        X.activationFunction();
    } while (!randomIndexes.empty());
}

void Recognition::showAnswer(int iteration) {
    printf("You got static attractor!\n");
    std::cout << "After " << iteration << " iteration(s) recognized image is: " << std::endl;
    X.beautifulVisualization(N);
    std::cout << "Output vector is: "; X.show();
}

void Recognition::showImages() {
    std::cout << "Images-templates are: " << std::endl;
    for (MatrixClass image : templates) {
        image.beautifulVisualization(N);
        std::cout << std::endl;
    }
}

void Recognition::generateRandomIndexes() {
    srand(time(nullptr));
    for(unsigned int i = 0; i < N; i ++) {
        randomIndexes.push_back(i);
        std::shuffle(randomIndexes.begin(), randomIndexes.end(), std::mt19937(std::random_device()()));
    }
    std::shuffle(randomIndexes.begin(), randomIndexes.end(), std::mt19937(std::random_device()()));
}

unsigned int Recognition::getRandomIndex() {
    srand(time(nullptr));
    double index = randomIndexes[randomIndexes.size() - 1];
    randomIndexes.pop_back();
    std::shuffle(randomIndexes.begin(), randomIndexes.end(), std::mt19937(std::random_device()()));
    return index;
}