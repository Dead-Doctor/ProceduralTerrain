#include "Texture.h"

Texture::Texture(const char *image, GLenum texType, GLuint slot, GLenum format, GLenum pixelType) {
    type = texType;
    int widthImg, heightImg, numColCh;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 4);

    glGenTextures(1, &ID);
    glActiveTexture(GL_TEXTURE0 + slot);
    unit = slot;
    glBindTexture(texType, ID);

    glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // blur (GL_LINEAR) or pixelate (GL_NEAREST) on scale
    glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // blur (GL_LINEAR) or pixelate (GL_NEAREST) on scale
    glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT); // repeat x axis
    glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT); // repeat y axis

    glTexImage2D(texType, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
    glGenerateMipmap(texType);

    stbi_image_free(bytes);
    glBindTexture(texType, 0);
}

void Texture::uv(Shader &shader, const char *uniform, GLuint unit) {
    GLuint texUni = glGetUniformLocation(shader.ID, uniform);
    shader.Activate();
    glUniform1i(texUni, unit);
}

void Texture::Bind() const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(type, ID);
}

void Texture::Unbind() const {
    glBindTexture(type, 0);
}

void Texture::Delete() {
    glDeleteTextures(1, &ID);
}