#include "Shader.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

namespace gl {

    // -------------------- Constructors --------------------
    Shader::Shader()
        : m_programID(0), m_isLinked(false) {
    }

    Shader::~Shader() {
        unload();
    }

    // -------------------- Program Management --------------------
    bool Shader::createProgram() {
        if (m_programID == 0)
            m_programID = glCreateProgram();

        return m_programID != 0;
    }

    void Shader::unload() {
        if (m_programID != 0) {
            for (auto& [type, shader] : m_shaderObjects) {
                glDetachShader(m_programID, shader);
                glDeleteShader(shader);
            }
            m_shaderObjects.clear();
            glDeleteProgram(m_programID);
            m_programID = 0;
            m_isLinked = false;
        }
    }

    // -------------------- Shader Attachments --------------------
    bool Shader::attach(GLenum type, const std::filesystem::path& filePath) {
        if (!std::filesystem::exists(filePath)) {
            std::cerr << "Shader file not found: " << filePath << "\n";
            return false;
        }

        std::string source = loadFileContent(filePath);
        return attach(type, source);
    }

    bool Shader::attach(GLenum type, const std::string& source) {
        if (m_programID == 0 && !createProgram()) {
            std::cerr << "Failed to create shader program\n";
            return false;
        }

        GLuint shader = glCreateShader(type);
        if (shader == 0) {
            std::cerr << "Failed to create shader of type: " << type << "\n";
            return false;
        }

        if (!compileShader(shader, source)) {
            glDeleteShader(shader);
            return false;
        }

        glAttachShader(m_programID, shader);
        m_shaderObjects[type] = shader;
        return true;
    }

    // -------------------- Linking --------------------
    bool Shader::linkProgram() {
        if (m_programID == 0) {
            std::cerr << "Cannot link: program not created\n";
            return false;
        }

        glLinkProgram(m_programID);

        GLint success = 0;
        glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[1024];
            glGetProgramInfoLog(m_programID, sizeof(infoLog), nullptr, infoLog);
            std::cerr << "Shader linking failed:\n" << infoLog << "\n";
            return false;
        }

        m_isLinked = true;
        return true;
    }

    void Shader::use() {
        if(!m_isLinked) {
            linkProgram();
        }
        glUseProgram(m_programID);
    }

    bool gl::Shader::attach(const std::filesystem::path& directory) {
        if (!std::filesystem::exists(directory)) {
            std::cerr << "Shader directory does not exist: " << directory << "\n";
            return false;
        }

        bool success = true;

        for (const auto& entry : std::filesystem::directory_iterator(directory)) {
            if (!entry.is_regular_file())
                continue;

            const auto ext = entry.path().extension().string();
            GLenum type = 0;

            if (ext == ".vs")       type = GL_VERTEX_SHADER;
            else if (ext == ".fs")  type = GL_FRAGMENT_SHADER;
            else if (ext == ".gs")  type = GL_GEOMETRY_SHADER;
            else if (ext == ".cs")  type = GL_COMPUTE_SHADER;
            else if (ext == ".tcs") type = GL_TESS_CONTROL_SHADER;
            else if (ext == ".tes") type = GL_TESS_EVALUATION_SHADER;
            else continue; // Skip unrelated files

            std::cout << "Loading shader: " << entry.path().filename().string() << "\n";

            if (!attach(type, entry.path())) {
                std::cerr << "Failed to attach shader: " << entry.path() << "\n";
                success = false;
            }
        }

        if (m_shaderObjects.empty()) {
            std::cerr << "No valid shader files found in directory: " << directory << "\n";
            return false;
        }

        return success;
    }


    // -------------------- Shader Compilation --------------------
    bool Shader::compileShader(GLuint shader, const std::string& source) {
        const char* src = source.c_str();
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        GLint success = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success) {
            char infoLog[1024];
            glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
            std::cerr << "Shader compilation failed:\n" << infoLog << "\n";
            return false;
        }

        return true;
    }

    // -------------------- File Reading --------------------
    std::string Shader::loadFileContent(const std::filesystem::path& filePath) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "Failed to open shader file: " << filePath << "\n";
            return "";
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    // -------------------- Accessors --------------------
    GLuint Shader::getProgramID() const {
        return m_programID;
    }

    bool Shader::isLinked() const {
        return m_isLinked;
    }

    bool Shader::hasShaderType(GLenum type) const {
        return m_shaderObjects.find(type) != m_shaderObjects.end();
    }

    // -------------------- Mutators --------------------
    void Shader::detachShader(GLenum type) {
        auto it = m_shaderObjects.find(type);
        if (it != m_shaderObjects.end()) {
            glDetachShader(m_programID, it->second);
            glDeleteShader(it->second);
            m_shaderObjects.erase(it);
        }
    }

    // -------------------- Uniform Utilities --------------------
    void gl::Shader::setUniform(const std::string& name, int value) const {
        GLint loc = glGetUniformLocation(m_programID, name.c_str());
        if (loc == -1) {
            std::cerr << "Uniform not found: " << name << "\n";
            return;
        }
        glUniform1i(loc, value);
    }

    void gl::Shader::setUniform(const std::string& name, float value) const {
        GLint loc = glGetUniformLocation(m_programID, name.c_str());
        if (loc == -1) {
            std::cerr << "Uniform not found: " << name << "\n";
            return;
        }
        glUniform1f(loc, value);
    }

    void gl::Shader::setUniform(const std::string& name, bool value) const {
        GLint loc = glGetUniformLocation(m_programID, name.c_str());
        if (loc == -1) {
            std::cerr << "Uniform not found: " << name << "\n";
            return;
        }
        glUniform1i(loc, value ? 1 : 0);
    }

    void gl::Shader::setUniform(const std::string& name, const glm::vec2& value) const {
        GLint loc = glGetUniformLocation(m_programID, name.c_str());
        if (loc == -1) {
            std::cerr << "Uniform not found: " << name << "\n";
            return;
        }
        glUniform2fv(loc, 1, glm::value_ptr(value));
    }

    void gl::Shader::setUniform(const std::string& name, const glm::vec3& value) const {
        GLint loc = glGetUniformLocation(m_programID, name.c_str());
        if (loc == -1) {
            std::cerr << "Uniform not found: " << name << "\n";
            return;
        }
        glUniform3fv(loc, 1, glm::value_ptr(value));
    }

    void gl::Shader::setUniform(const std::string& name, const glm::vec4& value) const {
        GLint loc = glGetUniformLocation(m_programID, name.c_str());
        if (loc == -1) {
            std::cerr << "Uniform not found: " << name << "\n";
            return;
        }
        glUniform4fv(loc, 1, glm::value_ptr(value));
    }

    void gl::Shader::setUniform(const std::string& name, const glm::mat3& value) const {
        GLint loc = glGetUniformLocation(m_programID, name.c_str());
        if (loc == -1) {
            std::cerr << "Uniform not found: " << name << "\n";
            return;
        }
        glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(value));
    }

    void gl::Shader::setUniform(const std::string& name, const glm::mat4& value) const {
        GLint loc = glGetUniformLocation(m_programID, name.c_str());
        if (loc == -1) {
            std::cerr << "Uniform not found: " << name << "\n";
            return;
        }
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
    }

    gl::unif gl::Shader::getUniform(const std::string& name) {
        if (!m_isLinked) {
            std::cerr << "Warning: getting uniform before program is linked.\n";
        }
        GLint loc = glGetUniformLocation(m_programID, name.c_str());
        return gl::unif(m_programID, name, loc);
    }


}
