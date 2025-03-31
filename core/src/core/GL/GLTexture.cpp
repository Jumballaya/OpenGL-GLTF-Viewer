#include "GLTexture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "GLTexture.hpp"
#include <iostream>

bool Core::GL::GLTexture::loadFromFile(const std::string &filePath)
{
    glBindTexture(textureType, *textureId);

    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &channels, 4);
    if (!data)
    {
        std::cerr << "Failed to load texture: " << filePath << std::endl;
        return false;
    }

    glTexImage2D(textureType, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(textureType);
    stbi_image_free(data);

    glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_REPEAT);

    unbind();

    return true;
}

void Core::GL::GLTexture::bind(GLuint unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(textureType, *textureId);
}

void Core::GL::GLTexture::unbind() const
{
    glBindTexture(textureType, 0);
}