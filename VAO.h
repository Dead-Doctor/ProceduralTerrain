#ifndef OPENGLGAME_VAO_H
#define OPENGLGAME_VAO_H

#include "glad/glad.h"
#include "VBO.h"

class VAO
{
public:
    GLuint ID;
    VAO();

    void LinkAttrib(VBO VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
    void Bind();
    void Unbind();
    void Delete();
};

#endif //OPENGLGAME_VAO_H
