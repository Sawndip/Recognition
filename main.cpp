#include <iostream>
#include "Recognition.h"

const char *directoryPath;
const char *noisyPath;

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Program was launched with wrong amount of parameters");
        exit(EXIT_FAILURE);
    }
    directoryPath = strdup(argv[1]);
    noisyPath = strdup(argv[2]);
    Recognition r(directoryPath, noisyPath);
    return 0;
}