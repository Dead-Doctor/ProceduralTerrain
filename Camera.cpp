#include "Camera.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/vector_angle.hpp"

Camera::Camera(int width, int height, glm::vec3 position) {
    Camera::width = width;
    Camera::height = height;
    Camera::position = position;
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane) {
    glm::mat4 view = glm::mat4(1.0);
    glm::mat4 proj = glm::mat4(1.0);

    view = glm::lookAt(position, position + orientation, up);
    proj = glm::perspective(glm::radians(FOVdeg), (float)(width)/height, nearPlane, farPlane);

    cameraMatrix = proj * view;
}

void Camera::uploadMatrix(Shader &shader, const char *uniform) {
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::inputs(GLFWwindow *window, double deltaTime) {
    //region Keyboard
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        position += (float)(speed * deltaTime) * orientation;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        position += (float)(speed * deltaTime) * -glm::normalize(glm::cross(orientation, up));
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        position += (float)(speed * deltaTime) * -orientation;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        position += (float)(speed * deltaTime) * glm::normalize(glm::cross(orientation, up));
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        position += (float)(speed * deltaTime) * up;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        position += (float)(speed * deltaTime) * -up;
    }


    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        speed = 2.0f;
    } else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE) {
        speed = 0.5f;
    }
    //endregion

    //region Mouse
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        if (firstClick) {
            firstClick = false;
            glfwSetCursorPos(window, (width / 2.0), (height / 2.0));
            return;
        }

        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);


        float rotationX = sensitivity * deltaTime * (float)(mouseY - height / 2.0) / height;
        float rotationY = sensitivity * deltaTime * (float)(mouseX - width / 2.0) / width;

        glm::vec3 newOrientation = glm::rotate(orientation, -rotationX, glm::cross(orientation, up));

        float pitch = glm::angle(newOrientation, up);
        if (glm::radians(5.0f) < pitch && pitch < glm::radians(175.0f)) {
            orientation = newOrientation;
        }

        orientation = glm::rotate(orientation, -rotationY, up);
        glfwSetCursorPos(window, (width / 2.0), (height / 2.0));

    } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        firstClick = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    //endregion
}
