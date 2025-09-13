#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include "Cryption.hpp"

int main(int argc, char *argv[]){
    if (argc != 2){
        cerr << "Usage: " << argv[0] << " <taskData>" << endl;
        return 1;
    }
    executeCryption(argv[1]);
    return 0;
}