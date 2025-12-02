#include "unif.hpp"
#include <iostream>

namespace gl {

    // -------------------- Constructor --------------------
    unif::unif(GLuint program, const std::string& name, GLint location)
        : m_programID(program), m_location(location), m_name(name) {
    }

    GLint unif::location() const {
        return m_location;
    }

    const std::string& unif::name() const {
        return m_name;
    }

    bool unif::valid() const { return m_location != -1; }

    // -------------------- Setters --------------------
    unif& unif::operator=(int v) {
        if (m_location != -1) glUniform1i(m_location, v);
        else std::cerr << "Uniform not found: " << m_name << "\n";
        return *this;
    }

    unif& unif::operator=(float v) {
        if (m_location != -1) glUniform1f(m_location, v);
        else std::cerr << "Uniform not found: " << m_name << "\n";
        return *this;
    }

    unif& unif::operator=(bool v) {
        if (m_location != -1) glUniform1i(m_location, v ? 1 : 0);
        else std::cerr << "Uniform not found: " << m_name << "\n";
        return *this;
    }

    unif& unif::operator=(const glm::vec2& v) {
        if (m_location != -1) glUniform2fv(m_location, 1, glm::value_ptr(v));
        else std::cerr << "Uniform not found: " << m_name << "\n";
        return *this;
    }

    unif& unif::operator=(const glm::vec3& v) {
        if (m_location != -1) glUniform3fv(m_location, 1, glm::value_ptr(v));
        else std::cerr << "Uniform not found: " << m_name << "\n";
        return *this;
    }

    unif& unif::operator=(const glm::vec4& v) {
        if (m_location != -1) glUniform4fv(m_location, 1, glm::value_ptr(v));
        else std::cerr << "Uniform not found: " << m_name << "\n";
        return *this;
    }

    unif& unif::operator=(const glm::mat3& v) {
        if (m_location != -1) glUniformMatrix3fv(m_location, 1, GL_FALSE, glm::value_ptr(v));
        else std::cerr << "Uniform not found: " << m_name << "\n";
        return *this;
    }

    unif& unif::operator=(const glm::mat4& v) {
        if (m_location != -1) glUniformMatrix4fv(m_location, 1, GL_FALSE, glm::value_ptr(v));
        else std::cerr << "Uniform not found: " << m_name << "\n";
        return *this;
    }

    // -------------------- Getters (cast operators) --------------------
    unif::operator int() const {
        int v = 0;
        if (m_location != -1) glGetUniformiv(m_programID, m_location, &v);
        return v;
    }

    unif::operator float() const {
        float v = 0.0f;
        if (m_location != -1) glGetUniformfv(m_programID, m_location, &v);
        return v;
    }

    unif::operator glm::vec2() const {
        glm::vec2 v(0.0f);
        if (m_location != -1) glGetUniformfv(m_programID, m_location, glm::value_ptr(v));
        return v;
    }

    unif::operator glm::vec3() const {
        glm::vec3 v(0.0f);
        if (m_location != -1) glGetUniformfv(m_programID, m_location, glm::value_ptr(v));
        return v;
    }

    unif::operator glm::vec4() const {
        glm::vec4 v(0.0f);
        if (m_location != -1) glGetUniformfv(m_programID, m_location, glm::value_ptr(v));
        return v;
    }

    unif::operator glm::mat3() const {
        glm::mat3 v(1.0f);
        if (m_location != -1) glGetUniformfv(m_programID, m_location, glm::value_ptr(v));
        return v;
    }

    unif::operator glm::mat4() const {
        glm::mat4 v(1.0f);
        if (m_location != -1) glGetUniformfv(m_programID, m_location, glm::value_ptr(v));
        return v;
    }

}