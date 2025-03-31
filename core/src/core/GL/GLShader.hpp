#pragma once
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <optional>

#include <glm/vec3.hpp>
#include <glm/matrix.hpp>

namespace Core::GL
{
    class GLShader
    {
    private:
        GLuint programId = 0;
        std::unordered_map<std::string, GLint> uniformCache;

        static std::optional<std::string> loadFile(const std::string &filePath)
        {
            std::ifstream file(filePath);
            if (!file)
                return "Failed to open file: " + filePath;

            std::stringstream buffer;
            buffer << file.rdbuf();
            return buffer.str();
        }

        static std::optional<std::string> compileShader(GLuint shader, const std::string &source)
        {
            const char *src = source.c_str();
            glShaderSource(shader, 1, &src, nullptr);
            glCompileShader(shader);

            GLint success;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                char log[1024];
                glGetShaderInfoLog(shader, 1024, nullptr, log);
                return std::string("Shader compilation error: ") + log;
            }
            return std::nullopt;
        }

        static std::optional<std::string> linkProgram(GLuint program)
        {
            glLinkProgram(program);

            GLint success;
            glGetProgramiv(program, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                char log[1024];
                glGetProgramInfoLog(program, 1024, nullptr, log);
                return std::string("Program linking error: ") + log;
            }
            return std::nullopt;
        }

    public:
        GLShader(const std::string &vertexPath, const std::string &fragmentPath)
        {
            auto vertexSource = loadFile(vertexPath);
            auto fragmentSource = loadFile(fragmentPath);

            if (!vertexSource || !fragmentSource)
            {
                std::cerr << "Shader File Error: " << *vertexSource << "\n"
                          << *fragmentSource << std::endl;
                return;
            }

            GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
            GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

            if (auto error = compileShader(vertexShader, *vertexSource))
            {
                std::cerr << *error << std::endl;
                glDeleteShader(vertexShader);
                return;
            }
            if (auto error = compileShader(fragmentShader, *fragmentSource))
            {
                std::cerr << *error << std::endl;
                glDeleteShader(vertexShader);
                glDeleteShader(fragmentShader);
                return;
            }

            programId = glCreateProgram();
            glAttachShader(programId, vertexShader);
            glAttachShader(programId, fragmentShader);

            if (auto error = linkProgram(programId))
            {
                std::cerr << *error << std::endl;
                glDeleteShader(vertexShader);
                glDeleteShader(fragmentShader);
                return;
            }

            glDetachShader(programId, vertexShader);
            glDetachShader(programId, fragmentShader);
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
        }

        ~GLShader()
        {
            if (programId)
            {
                glDeleteProgram(programId);
            }
        }

        void use() const noexcept
        {
            glUseProgram(programId);
        }

        GLint getUniformLocation(const std::string &name)
        {
            if (uniformCache.contains(name))
                return uniformCache[name];

            GLint location = glGetUniformLocation(programId, name.c_str());
            uniformCache[name] = location;
            return location;
        }

        void setUniform(const std::string &name, float value)
        {
            glUniform1f(getUniformLocation(name), value);
        }

        void setUniform(const std::string &name, int value)
        {
            glUniform1i(getUniformLocation(name), value);
        }

        void setUniform(const std::string &name, const glm::vec3 &value)
        {
            glUniform3fv(getUniformLocation(name), 1, &value[0]);
        }

        void setUniform(const std::string &name, const glm::mat4 &value)
        {
            glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &value[0][0]);
        }
    };
}