#ifndef PROCEDURALTERRAIN_CAMERA_H
#define PROCEDURALTERRAIN_CAMERA_H

#include "Shader.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

class Camera {
public:
    glm::vec3 position{};
    glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    bool firstClick = true;

    int width;
    int height;

    float speed = 0.5f;
    float sensitivity = 50.0f;

    Camera(int width, int height, glm::vec3 position);

    void matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform);
    void inputs(GLFWwindow *window, double deltaTime);
};


#endif //PROCEDURALTERRAIN_CAMERA_H
