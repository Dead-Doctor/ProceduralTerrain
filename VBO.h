#ifndef OPENGLGAME_VBO_H
#define OPENGLGAME_VBO_H

#include <vector>
#include "glad/glad.h"
#include "glm/glm.hpp"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
    glm::vec2 uv;
};

class VBO
{
public:
    GLuint ID;
    VBO(std::vector<Vertex>& vertices);

    void Bind();
    void Unbind();
    void Delete();
};

#endif //OPENGLGAME_VBO_H
