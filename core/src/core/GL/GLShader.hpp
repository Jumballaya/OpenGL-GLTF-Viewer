#pragma once

#include <glad/glad.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <glm/glm.hpp>

#include "GLTexture.hpp"

namespace Core::GL
{

    class GLShader
    {
    public:
        struct Attribute
        {
            std::string name;
            GLenum type;
            GLint location;
        };

        struct Uniform
        {
            std::string name;
            GLenum type;
            GLint location;
            GLint size;
        };

        struct UniformBlock
        {
            std::string name;
            GLuint index;
            GLint size;
            GLint binding;
        };

    private:
        GLuint programID;
        std::vector<Attribute> attributes;
        std::vector<Uniform> uniforms;
        std::vector<UniformBlock> uniformBlocks;
        std::unordered_map<std::string, GLint> uniformCache;

    public:
        GLShader(const std::string &vertexPath, const std::string &fragmentPath)
            : programID(0)
        {
            if (auto error = compileShader(vertexPath, fragmentPath); error)
            {
                std::cerr << "Shader Compilation Error: " << *error << '\n';
            }
            else
            {
                reflectShader();
            }
        }

        ~GLShader()
        {
            if (programID)
            {
                glDeleteProgram(programID);
            }
        }

        void use() const
        {
            glUseProgram(programID);
        }

        GLuint getID() const { return programID; }

        const std::vector<Attribute> &getAttributes() const { return attributes; }
        const std::vector<Uniform> &getUniforms() const { return uniforms; }
        const std::vector<UniformBlock> &getUniformBlocks() const { return uniformBlocks; }

        void setUniform(const std::string &name, int value) { glUniform1i(getUniformLocation(name), value); }
        void setUniform(const std::string &name, float value) { glUniform1f(getUniformLocation(name), value); }
        void setUniform(const std::string &name, const glm::vec2 &value) { glUniform2fv(getUniformLocation(name), 1, &value[0]); }
        void setUniform(const std::string &name, const glm::vec3 &value) { glUniform3fv(getUniformLocation(name), 1, &value[0]); }
        void setUniform(const std::string &name, const glm::vec4 &value) { glUniform4fv(getUniformLocation(name), 1, &value[0]); }
        void setUniform(const std::string &name, const glm::mat4 &value) { glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &value[0][0]); }

        void setTexture(const std::string &name, const GLTexture &texture, GLuint unit)
        {
            glUniform1i(getUniformLocation(name), unit);
            texture.bind(unit);
        }

    private:
        std::optional<std::string> compileShader(const std::string &vertexPath, const std::string &fragmentPath)
        {
            std::string vertexCode = loadShaderSource(vertexPath);
            std::string fragmentCode = loadShaderSource(fragmentPath);

            GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
            GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

            compileSingleShader(vertexShader, vertexCode, "Vertex Shader");
            compileSingleShader(fragmentShader, fragmentCode, "Fragment Shader");

            programID = glCreateProgram();
            glAttachShader(programID, vertexShader);
            glAttachShader(programID, fragmentShader);
            glLinkProgram(programID);

            GLint success;
            glGetProgramiv(programID, GL_LINK_STATUS, &success);
            if (!success)
            {
                char infoLog[512];
                glGetProgramInfoLog(programID, sizeof(infoLog), nullptr, infoLog);
                return std::string("Shader Program Linking Failed: ") + infoLog;
            }

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            return std::nullopt;
        }

        std::string loadShaderSource(const std::string &path)
        {
            std::ifstream file(path);
            if (!file)
            {
                std::cerr << "Failed to load shader: " << path << '\n';
                return "";
            }

            std::stringstream buffer;
            buffer << file.rdbuf();
            return buffer.str();
        }

        void compileSingleShader(GLuint shader, const std::string &source, const std::string &name)
        {
            const char *sourceCStr = source.c_str();
            glShaderSource(shader, 1, &sourceCStr, nullptr);
            glCompileShader(shader);

            GLint success;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                char infoLog[512];
                glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
                std::cerr << name << " Compilation Failed: " << infoLog << '\n';
            }
        }

        void reflectShader()
        {
            reflectAttributes();
            reflectUniforms();
            reflectUniformBlocks();
        }

        void reflectAttributes()
        {
            GLint attributeCount = 0;
            glGetProgramiv(programID, GL_ACTIVE_ATTRIBUTES, &attributeCount);

            for (GLint i = 0; i < attributeCount; ++i)
            {
                GLchar name[256];
                GLenum type;
                GLint size;
                glGetActiveAttrib(programID, i, sizeof(name), nullptr, &size, &type, name);
                GLint location = glGetAttribLocation(programID, name);

                attributes.push_back({name, type, location});
            }
        }

        void reflectUniforms()
        {
            GLint uniformCount = 0;
            glGetProgramiv(programID, GL_ACTIVE_UNIFORMS, &uniformCount);

            for (GLint i = 0; i < uniformCount; ++i)
            {
                GLchar name[256];
                GLenum type;
                GLint size;
                glGetActiveUniform(programID, i, sizeof(name), nullptr, &size, &type, name);
                GLint location = glGetUniformLocation(programID, name);

                uniforms.push_back({name, type, location, size});
                uniformCache[name] = location;
            }
        }

        void reflectUniformBlocks()
        {
            GLint blockCount = 0;
            glGetProgramiv(programID, GL_ACTIVE_UNIFORM_BLOCKS, &blockCount);

            for (GLint i = 0; i < blockCount; ++i)
            {
                GLchar name[256];
                glGetActiveUniformBlockName(programID, i, sizeof(name), nullptr, name);

                GLint size, binding;
                GLuint index = static_cast<GLuint>(i);
                glGetActiveUniformBlockiv(programID, index, GL_UNIFORM_BLOCK_DATA_SIZE, &size);
                glGetActiveUniformBlockiv(programID, index, GL_UNIFORM_BLOCK_BINDING, &binding);

                uniformBlocks.push_back({name, index, size, binding});
            }
        }

        GLint getUniformLocation(const std::string &name)
        {
            if (uniformCache.contains(name))
            {
                return uniformCache[name];
            }

            GLint location = glGetUniformLocation(programID, name.c_str());
            if (location == -1)
            {
                std::cerr << "Warning: Uniform '" << name << "' not found in shader\n";
            }
            else
            {
                uniformCache[name] = location;
            }
            return location;
        }
    };

}