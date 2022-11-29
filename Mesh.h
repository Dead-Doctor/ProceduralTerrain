#ifndef PROCEDURALTERRAIN_MESH_H
#define PROCEDURALTERRAIN_MESH_H

#include <vector>
#include "Texture.h"
#include "VAO.h"
#include "Camera.h"

class Mesh {
public:
    std::vector <Vertex> vertices;
    std::vector <GLuint> indices;
    std::vector <Texture> textures;

    VAO VAO;

    Mesh(std::vector<Vertex> &vertices, vector<GLuint> &indices, vector<Texture> &textures);

    void draw(Shader& shader, Camera& camera);
};

#endif //PROCEDURALTERRAIN_MESH_H
