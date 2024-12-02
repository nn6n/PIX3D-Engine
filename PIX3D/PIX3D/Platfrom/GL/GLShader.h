#pragma once
#include <glm/glm.hpp>

namespace PIX3D
{
    namespace GL
    {
        class GLShader
        {
        public:
            GLShader()
                : m_ProgramID(0)
            {}
            
            ~GLShader();

            bool LoadFromFile(const std::string& vertexPath, const std::string& fragmentPath);

            bool LoadFromString(const std::string& vertexCode, const std::string& fragmentCode);

            void Bind() const;

            void SetFloat(const std::string& name, float value) const;
            void SetVec3(const std::string& name, const glm::vec3& value) const;
            void SetVec4(const std::string& name, const glm::vec4& value) const;
            void SetMat4(const std::string& name, const glm::mat4& matrix) const;
            void SetMat3(const std::string& name, const glm::mat3& matrix) const;

            // Get program ID
            uint32_t GetProgramID() const { return m_ProgramID; }

        private:
            uint32_t m_ProgramID;
            bool CompileAndLink(const std::string& vertexCode, const std::string& fragmentCode);
        };
    }
}
