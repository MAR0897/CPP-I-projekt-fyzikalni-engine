#include "header.h"

void errorCallback(int error, const char* description){
    std::cerr << "Error: " << description << std::endl;
}