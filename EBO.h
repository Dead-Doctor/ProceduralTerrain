#ifndef OPENGLGAME_EBO_H
#define OPENGLGAME_EBO_H

#include <vector>
#include "glad/glad.h"

class EBO
{
public:
    GLuint ID;
    EBO(std::vector<GLuint>& indices);

    void Bind();
    void Unbind();
    void Delete();
};

#endif //OPENGLGAME_EBO_H
