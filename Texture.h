#ifndef OPENGLGAME_TEXTURE_H
#define OPENGLGAME_TEXTURE_H

#include <glad/glad.h>
#include "stb/stb_image.h"

#include "Shader.h"

class Texture {
public:
    GLuint ID;
    GLenum type;
    GLuint unit;

    Texture(const char *image, GLenum texType, GLuint slot, GLenum format, GLenum pixelType);

    static void uv(Shader &shader, const char *uniform, GLuint unit);
    void Bind() const;
    void Unbind() const;
    void Delete();
};

#endif //OPENGLGAME_TEXTURE_H
