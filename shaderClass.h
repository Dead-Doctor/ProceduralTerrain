#ifndef SHADERCLASS_H
#define SHADERCLASS_H

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstring>
#include "glad/glad.h"

using namespace std;

string readFile(const char *filename);

class Shader {

public:
    GLuint ID;

    Shader(const char *vertexFile, const char *fragmentFile);
    void Activate() const;
    void Delete() const;
    static void compileErrors(unsigned int shader, const char *type);
};

#endif //SHADERCLASS_H
