#include <memory>
#include <optional>
#include <functional>
#include <glad/glad.h>
#include "GLBuffer.hpp"

namespace Core::GL
{

    class VAO
    {
    private:
        std::unique_ptr<GLuint> vaoID;
        std::optional<std::reference_wrapper<const GLBuffer>> ebo;

    public:
        VAO()
            : vaoID(std::make_unique<GLuint>(0))
        {
            glGenVertexArrays(1, vaoID.get());
        }

        ~VAO()
        {
            if (vaoID)
            {
                glDeleteVertexArrays(1, vaoID.get());
            }
        }

        void bind() const
        {
            glBindVertexArray(*vaoID);
        }

        void unbind() const
        {
            glBindVertexArray(0);
        }

        void addVertexBuffer(const GLBuffer &vbo, GLuint attribIndex, GLint componentCount, GLenum type, GLboolean normalized, GLsizei stride, size_t offset)
        {
            bind();
            vbo.bind();
            glEnableVertexAttribArray(attribIndex);
            glVertexAttribPointer(attribIndex, componentCount, type, normalized, stride, reinterpret_cast<const void *>(offset));
            vbo.unbind();
            unbind();
        }

        void setIndexBuffer(const GLBuffer &eboBuffer)
        {
            bind();
            eboBuffer.bind();
            ebo = eboBuffer;
            bind();
        }

        GLuint getID() const
        {
            return *vaoID;
        }
    };

}