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
#include "Mesh.h"

//#define WIREFRAME
//#define SHOW_FPS

#define WIDTH 800
#define HEIGHT 800

// Vertices coordinates
Vertex vertices[] =
        { //               COORDINATES           /       NORMALS         /            COLORS          /           TexCoord         //
                Vertex{glm::vec3(-1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),
                       glm::vec2(0.0f, 0.0f)},
                Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),
                       glm::vec2(0.0f, 1.0f)},
                Vertex{glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),
                       glm::vec2(1.0f, 1.0f)},
                Vertex{glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f),
                       glm::vec2(1.0f, 0.0f)}
        };

// Indices for vertices order
GLuint indices[] =
        {
                0, 1, 2,
                0, 2, 3
        };

Vertex lightVertices[] =
        { //     COORDINATES     //
                Vertex{glm::vec3(-0.1f, -0.1f, 0.1f)},
                Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
                Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
                Vertex{glm::vec3(0.1f, -0.1f, 0.1f)},
                Vertex{glm::vec3(-0.1f, 0.1f, 0.1f)},
                Vertex{glm::vec3(-0.1f, 0.1f, -0.1f)},
                Vertex{glm::vec3(0.1f, 0.1f, -0.1f)},
                Vertex{glm::vec3(0.1f, 0.1f, 0.1f)}
        };

GLuint lightIndices[] =
        {
                0, 1, 2,
                0, 2, 3,
                0, 4, 7,
                0, 7, 3,
                3, 7, 6,
                3, 6, 2,
                2, 6, 5,
                2, 5, 1,
                1, 5, 4,
                1, 4, 0,
                4, 5, 6,
                4, 6, 7
        };

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

    Texture textures[]
    {
        Texture("resources/textures/planks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
        Texture("resources/textures/planksSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
    };

    Shader shaderProgram("resources/shaders/default.vert", "resources/shaders/default.frag");

    std::vector <Vertex> vertexVector(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
    std::vector <GLuint> indexVector(indices, indices + sizeof(indices) / sizeof(GLuint));
    std::vector <Texture> textureVector(textures, textures + sizeof(textures) / sizeof(Texture));

    Mesh floor(vertexVector, indexVector, textureVector);

    // Shader for light cube
    Shader lightShader("resources/shaders/light.vert", "resources/shaders/light.frag");
    // Store mesh data in vectors for the mesh
    std::vector <Vertex> lightVertexVector(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
    std::vector <GLuint> lightIndexVector(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
    std::vector <Texture> lightTextureVector{};
    // Crate light mesh
    Mesh light(lightVertexVector, lightIndexVector, lightTextureVector);

    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);

    glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 objectModel = glm::mat4(1.0f);
    objectModel = glm::translate(objectModel, objectPos);

    lightShader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
    glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    shaderProgram.Activate();
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
    glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

#ifdef WIREFRAME
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
#endif //WIREFRAME
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
        camera.updateMatrix(45.0f, 0.1f, 100.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draws different meshes
        floor.draw(shaderProgram, camera);
        light.draw(lightShader, camera);

        glfwSwapBuffers(window);

        // Poll Events (WinAPI) Otherwise Application wouldn't 'respond'
        glfwPollEvents();
        frame++;
    }
    shaderProgram.Delete();
    lightShader.Delete();

    // Destroy Window
    glfwDestroyWindow(window);
    // Free Memory
    glfwTerminate();
    return 0;
}
