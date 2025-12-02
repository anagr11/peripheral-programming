#pragma once

#include "unif.hpp"
#include <string>
#include <filesystem>
#include <unordered_map>
#include <glad/glad.h>  // Make sure to include glad or GLEW before GLFW
#include <glm/glm.hpp>

namespace gl {

    class Shader {
    private:
        GLuint m_programID;
        std::unordered_map<GLenum, GLuint> m_shaderObjects;
        bool m_isLinked;

    public:
        // Constructors / Destructor
        Shader();
        ~Shader();

        // Core functionality
        bool createProgram();
        bool attach(GLenum type, const std::filesystem::path& filePath);
        bool attach(GLenum type, const std::string& source);
        bool attach(const std::filesystem::path& directory);
        bool linkProgram();
        void use();
        void unload();

        // Getters
        GLuint getProgramID() const;
        bool isLinked() const;
        bool hasShaderType(GLenum type) const;

        // Setters
        void detachShader(GLenum type);

    public:

        // Uniform utilities
        void setUniform(const std::string& name, int value) const;
        void setUniform(const std::string& name, float value) const;
        void setUniform(const std::string& name, bool value) const;

        void setUniform(const std::string& name, const glm::vec2& value) const;
        void setUniform(const std::string& name, const glm::vec3& value) const;
        void setUniform(const std::string& name, const glm::vec4& value) const;

        void setUniform(const std::string& name, const glm::mat3& value) const;
        void setUniform(const std::string& name, const glm::mat4& value) const;

        unif getUniform(const std::string& name);

    private:
        std::string loadFileContent(const std::filesystem::path& filePath);
        bool compileShader(GLuint shader, const std::string& source);
    };

}

