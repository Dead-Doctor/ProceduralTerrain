cmake_minimum_required(VERSION 3.22)
project(ProceduralTerrain)

set(CMAKE_CXX_STANDARD 17)

add_executable(ProceduralTerrain main.cpp Shader.h Shader.cpp VBO.cpp VBO.h EBO.cpp EBO.h VAO.cpp VAO.h Texture.cpp Texture.h Camera.cpp Camera.h)

add_subdirectory(glfw)
add_subdirectory(glad)
add_subdirectory(glm)
add_subdirectory(stb)

target_include_directories(ProceduralTerrain PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include)
target_link_libraries(ProceduralTerrain glfw glad glm stb)