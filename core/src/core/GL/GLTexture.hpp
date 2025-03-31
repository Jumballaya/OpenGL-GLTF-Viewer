#pragma once

#include <glad/glad.h>
#include <string>
#include <memory>
#include <optional>

namespace Core::GL
{
    class GLTexture
    {
    private:
        std::unique_ptr<GLuint, void (*)(GLuint *)> textureId;
        GLenum textureType;
        int width;
        int height;
        int channels;

    public:
        explicit GLTexture(GLenum type = GL_TEXTURE_2D)
            : textureType(type),
              textureId(new GLuint(0), [](GLuint *id)
                        {
                if (id && *id) {
                    glDeleteTextures(1, id);
                    delete id;
                } })
        {
            glGenTextures(1, textureId.get());
        }

        bool loadFromFile(const std::string &filePath);
        void bind(GLuint uint = 0) const;
        void unbind() const;

        [[nodiscard]] GLuint getId() const { return *textureId; }
        [[nodiscard]] int getWidth() const { return width; }
        [[nodiscard]] int getHeight() const { return height; }
        [[nodiscard]] int getChannels() const { return channels; }
    };
}