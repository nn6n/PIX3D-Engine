#include "GLShader.h"
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <Core/Core.h>

namespace PIX3D
{
    namespace GL
    {

        GLShader::~GLShader()
        {
            if (m_ProgramID != 0)
                glDeleteProgram(m_ProgramID);
        }

        bool GLShader::LoadFromFile(const std::string & vertexPath, const std::string & fragmentPath)
        {
            std::string vertexCode;
            std::string fragmentCode;

            try {
                // Read vertex shader code from file
                std::ifstream vShaderFile(vertexPath);
                std::stringstream vShaderStream;
                vShaderStream << vShaderFile.rdbuf();
                vertexCode = vShaderStream.str();
                vShaderFile.close();

                // Read fragment shader code from file
                std::ifstream fShaderFile(fragmentPath);
                std::stringstream fShaderStream;
                fShaderStream << fShaderFile.rdbuf();
                fragmentCode = fShaderStream.str();
                fShaderFile.close();
            }
            catch (std::ifstream::failure& e) {
                std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
                return false;
            }

            return CompileAndLink(vertexCode, fragmentCode);
        }

        bool GLShader::LoadFromString(const std::string& vertexCode, const std::string& fragmentCode)
        {
            return CompileAndLink(vertexCode, fragmentCode);
        }

        bool GLShader::CompileAndLink(const std::string & vertexCode, const std::string & fragmentCode)
        {
            const char* vShaderCode = vertexCode.c_str();
            const char* fShaderCode = fragmentCode.c_str();

            GLuint vertex, fragment;
            int success;
            char infoLog[512];

            // Vertex Shader
            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vShaderCode, NULL);
            glCompileShader(vertex);

            // Check for vertex shader compile errors
            glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(vertex, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
                return false;
            }

            // Fragment Shader
            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fShaderCode, NULL);
            glCompileShader(fragment);

            // Check for fragment shader compile errors
            glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(fragment, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
                return false;
            }

            // Shader Program
            m_ProgramID = glCreateProgram();
            glAttachShader(m_ProgramID, vertex);
            glAttachShader(m_ProgramID, fragment);
            glLinkProgram(m_ProgramID);

            // Check for linking errors
            glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(m_ProgramID, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
                return false;
            }

            // Delete shaders as they're linked into our program now and no longer necessary
            glDeleteShader(vertex);
            glDeleteShader(fragment);

            return true;
        }

        void GLShader::Bind() const
        {
            glUseProgram(m_ProgramID);
        }

        void GLShader::Destroy()
        {
            glDeleteProgram(m_ProgramID);
        }

        void GLShader::SetFloat(const std::string & name, float value) const
        {
            GLint Location = glGetUniformLocation(m_ProgramID, name.c_str());
            PIX_ASSERT_MSG(Location >= 0, "Unknown Uniform");
            glUniform1f(Location, value);
        }

        void GLShader::SetVec3(const std::string& name, const glm::vec3& value) const
        {
            GLint Location = glGetUniformLocation(m_ProgramID, name.c_str());
            PIX_ASSERT_MSG(Location >= 0, "Unknown Uniform");
            glUniform3f(Location, value.x, value.y, value.z);
        }

        void GLShader::SetVec4(const std::string& name, const glm::vec4& value) const
        {
            GLint Location = glGetUniformLocation(m_ProgramID, name.c_str());
            PIX_ASSERT_MSG(Location >= 0, "Unknown Uniform");
            glUniform4f(Location, value.x, value.y, value.z, value.w);
        }

        void GLShader::SetMat4(const std::string& name, const glm::mat4& matrix) const
        {
            GLint Location = glGetUniformLocation(m_ProgramID, name.c_str());
            PIX_ASSERT_MSG(Location >= 0, "Unknown Uniform");
            glUniformMatrix4fv(Location, 1, GL_FALSE, glm::value_ptr(matrix));
        }

        void GLShader::SetMat3(const std::string& name, const glm::mat3& matrix) const
        {
            GLint Location = glGetUniformLocation(m_ProgramID, name.c_str());
            PIX_ASSERT_MSG(Location >= 0, "Unknown Uniform");
            glUniformMatrix3fv(Location, 1, GL_FALSE, glm::value_ptr(matrix));
        }
    }
}
