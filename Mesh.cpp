#include "Mesh.h"
#include "EBO.h"

Mesh::Mesh(std::vector<Vertex> &vertices, vector<GLuint> &indices, vector<Texture> &textures) {
    Mesh::vertices = vertices;
    Mesh::indices = indices;
    Mesh::textures = textures;

    VAO.Bind();

    VBO VBO(vertices);
    EBO EBO(indices);

    VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void *) 0); // pos
    VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void *) (3 * sizeof(float))); // normal
    VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void *) (6 * sizeof(float))); // color
    VAO.LinkAttrib(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void *) (9 * sizeof(float))); // uv

    VAO.Unbind();
    VBO.Unbind();
    EBO.Unbind();
}

void Mesh::draw(Shader &shader, Camera &camera) {
    shader.Activate();
    VAO.Bind();

    unsigned int numDiffuse = 0;
    unsigned int numSpecular = 0;

    for (unsigned int i = 0; i < textures.size(); i++) {
        std::string num;
        std::string type = textures[i].type;

        if (type == "diffuse") {
            num = std::to_string(numDiffuse++);
        } else if (type == "specular") {
            num = std::to_string(numSpecular++);
        }
        // Uniform
        textures[i].Unit(shader, (type + num).c_str(), i);
        textures[i].Bind();
    }

    glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.position.x, camera.position.y, camera.position.z);
    camera.uploadMatrix(shader, "camMatrix");

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
}


