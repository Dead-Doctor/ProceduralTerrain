#ifndef OPENGLGAME_TEXTURE_H
#define OPENGLGAME_TEXTURE_H

#include <glad/glad.h>
#include "stb/stb_image.h"

#include "Shader.h"

class Texture {
public:
    GLuint ID;
    const char* type;
    GLuint unit;

    Texture(const char *image, const char* texType, GLuint slot, GLenum format, GLenum pixelType);

    static void Unit(Shader &shader, const char *uniform, GLuint unit);
    void Bind() const;
    void Unbind() const;
    void Delete();
};

#endif //OPENGLGAME_TEXTURE_H
