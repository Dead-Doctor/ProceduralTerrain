#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "stb/stb_image.h"
#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Texture.h"
#include "Camera.h"
//#define SHOW_FPS

#define WIDTH 800
#define HEIGHT 800

// https://youtu.be/45MIykWJ-C4
int main() {
    // Initialize GLFW
    glfwInit();

    // Set OpenGL Version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create Window
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Procedural Terrain", nullptr, nullptr);
    // Check for errors
    if (window == nullptr) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return 69;
    }
    // window as OpenGL context
    glfwMakeContextCurrent(window);

    // Initialize OpenGL
    gladLoadGL();

    // Set Viewport
    glViewport(0, 0, WIDTH, HEIGHT);

    GLfloat vertices[] = {
    /*   POSITION               COLOR                 UV          */
        -0.5f,  0.0f,  0.5f,     0.83f, 0.70f, 0.44f,     0.0f, 0.0f,
        -0.5f,  0.0f, -0.5f,     0.83f, 0.70f, 0.44f,     5.0f, 1.0f,
         0.5f,  0.0f, -0.5f,     0.83f, 0.70f, 0.44f,     0.0f, 1.0f,
         0.5f,  0.0f,  0.5f,     0.83f, 0.70f, 0.44f,     5.0f, 0.0f,
         0.0f,  0.8f,  0.0f,     0.92f, 0.86f, 0.76f,     2.5f, 5.0f,
    };

    GLuint indices[] = {
        0, 1, 2,
        0, 2, 3,
        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4,
    };

    Shader shaderProgram("resources/shaders/default.vert", "resources/shaders/default.frag");

    VAO VAO1;
    VAO1.Bind();

    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void *) 0); // pos
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void *) (3 * sizeof(float))); // color
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void *) (6 * sizeof(float))); // uv

    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    // Texture
    Texture texture1("resources/textures/brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture::uv(shaderProgram, "tex0", 0);

    // Set Clear Color
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

    Camera camera(WIDTH, HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f));

    // Main Loop
    int frame = 0;
    double lastFrameTime = glfwGetTime();

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
        double frameTime = glfwGetTime();
        // Delta Time in Seconds
        double deltaTime = frameTime - lastFrameTime;
#ifdef SHOW_FPS
        cout << "FPS: " << 1.0 / deltaTime << std::endl;
#endif //SHOW_FPS
        lastFrameTime = frameTime;

        camera.inputs(window, deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgram.Activate();

        camera.matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

        texture1.Bind();

        VAO1.Bind();

        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);

        // Poll Events (WinAPI) Otherwise Application wouldn't 'respond'
        glfwPollEvents();
        frame++;
    }
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    texture1.Delete();
    shaderProgram.Delete();

    // Destroy Window
    glfwDestroyWindow(window);
    // Free Memory
    glfwTerminate();
    return 0;
}
