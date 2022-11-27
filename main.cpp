#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "stb/stb_image.h"
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Texture.h"
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

    GLuint scaleUniform = glGetUniformLocation(shaderProgram.ID, "scale");

    // Texture
    Texture texture1("resources/textures/brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture::uv(shaderProgram, "tex0", 0);

    // Set Clear Color
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    // Clear Screen
    glClear(GL_COLOR_BUFFER_BIT);
    // Swap Screen Buffers to show drawn Buffer to the Screen
    glfwSwapBuffers(window);

    float rotation = 0.0f;

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
        rotation += 45.0f * (float) deltaTime;

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderProgram.Activate();

        glm::mat4 model = glm::mat4(1.0);
        glm::mat4 view = glm::mat4(1.0);
        glm::mat4 proj = glm::mat4(1.0);
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(1.0f, 1.0f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
        proj = glm::perspective(glm::radians(45.0f), (float)(WIDTH/HEIGHT), 0.1f, 100.0f);

        int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        int projLoc = glGetUniformLocation(shaderProgram.ID, "proj");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

        glUniform1f(scaleUniform, (frame % 300) / 300.0);
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
