#pragma once

#include <glad/glad.h>
#include <vector>
#include <span>
#include <memory>
#include <optional>

namespace Core::GL
{
    enum class BufferType : GLenum
    {
        Vertex = GL_ARRAY_BUFFER,
        Index = GL_ELEMENT_ARRAY_BUFFER,
        Uniform = GL_UNIFORM_BUFFER
    };

    class GLBuffer
    {
    private:
        std::unique_ptr<GLuint, void (*)(GLuint *)> bufferId;
        GLenum type;
        size_t size = 0;

    public:
        explicit GLBuffer(BufferType bufferType)
            : type(static_cast<GLenum>(bufferType)),
              bufferId(new GLuint(0), [](GLuint *id)
                       { if (id && *id) {
                glDeleteBuffers(1, id);
                delete id;
              } })
        {
            glGenBuffers(1, bufferId.get());
        }

        // The following prevents copying, but allows moving
        GLBuffer(const GLBuffer &) = delete;
        GLBuffer &operator=(const GLBuffer &) = delete;
        GLBuffer(GLBuffer &&other) noexcept = default;
        GLBuffer &operator=(GLBuffer &&other) noexcept = default;

        ~GLBuffer() = default;

        void bind() const noexcept
        {
            glBindBuffer(type, *bufferId);
        }

        void unbind() const noexcept
        {
            glBindBuffer(type, 0);
        }

        template <typename T>
        std::optional<std::string> setData(std::span<T> data, GLenum usage = GL_STATIC_DRAW)
        {
            if (data.empty())
            {
                return "Invalid buffer data";
            }

            size = data.size_bytes();
            bind();
            glBufferData(type, size, data.data(), usage);
            unbind();

            return std::nullopt;
        }

        std::optional<std::string> setData(size_t dataSize, const void *data, GLenum usage = GL_STATIC_DRAW)
        {
            if (!data || dataSize == 0)
            {
                return "Invalid buffer data";
            }
            size = dataSize;
            bind();
            glBufferData(type, size, data, usage);
            unbind();

            return std::nullopt;
        }

        std::optional<std::string> updateData(size_t offset, std::span<const std::byte> data)
        {
            if (offset + data.size_bytes() > size)
            {
                return "Buffer update out of bounds";
            }

            bind();
            glBufferSubData(type, offset, data.size_bytes(), data.data());
            unbind();
            return std::nullopt;
        }

        [[nodiscard]] GLuint getID() const noexcept { return *bufferId; }
        [[nodiscard]] size_t getSize() const noexcept { return size; }
    };
}