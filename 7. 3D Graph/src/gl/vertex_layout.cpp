#include "vertex_layout.hpp"
#include <type_traits>
#include <stdexcept>

namespace gl {

    // -------------------------------
    // Internal static helpers
    // -------------------------------

    // Restrict visibility to this translation unit
    namespace {
        template<typename T>
        constexpr GLenum gl_type();

        template<> constexpr GLenum gl_type<float>() { return GL_FLOAT; }
        template<> constexpr GLenum gl_type<int>() { return GL_INT; }
        template<> constexpr GLenum gl_type<unsigned int>() { return GL_UNSIGNED_INT; }
        template<> constexpr GLenum gl_type<double>() { return GL_DOUBLE; }

        template<typename T>
        constexpr bool is_supported() {
            return std::is_same_v<T, float> ||
                std::is_same_v<T, int> ||
                std::is_same_v<T, unsigned int> ||
                std::is_same_v<T, double>;
        }
    }

    // -------------------------------
    // vertex_layout Implementation
    // -------------------------------

    vertex_layout::vertex_layout()
        : m_currentOffset(0), m_nextIndex(0) {
    }

    // Template specializations

    template<>
    vertex_layout& vertex_layout::add<float>(GLint count, bool normalized) {
        if (!is_supported<float>())
            throw std::runtime_error("Unsupported vertex attribute type (float).");

        Attribute attr{
            m_nextIndex++,
            count,
            gl_type<float>(),
            GLboolean(normalized ? GL_TRUE : GL_FALSE),
            m_currentOffset
        };

        m_attributes.push_back(attr);
        m_currentOffset += sizeof(float) * count;
        return *this;
    }

    template<>
    vertex_layout& vertex_layout::add<int>(GLint count, bool normalized) {
        if (!is_supported<int>())
            throw std::runtime_error("Unsupported vertex attribute type (int).");

        Attribute attr{
            m_nextIndex++,
            count,
            gl_type<int>(),
            GLboolean(normalized ? GL_TRUE : GL_FALSE),
            m_currentOffset
        };

        m_attributes.push_back(attr);
        m_currentOffset += sizeof(int) * count;
        return *this;
    }

    template<>
    vertex_layout& vertex_layout::add<unsigned int>(GLint count, bool normalized) {
        if (!is_supported<unsigned int>())
            throw std::runtime_error("Unsupported vertex attribute type (uint).");

        Attribute attr{
            m_nextIndex++,
            count,
            gl_type<unsigned int>(),
            GLboolean(normalized ? GL_TRUE : GL_FALSE),
            m_currentOffset
        };

        m_attributes.push_back(attr);
        m_currentOffset += sizeof(unsigned int) * count;
        return *this;
    }

    template<>
    vertex_layout& vertex_layout::add<double>(GLint count, bool normalized) {
        if (!is_supported<double>())
            throw std::runtime_error("Unsupported vertex attribute type (double).");

        Attribute attr{
            m_nextIndex++,
            count,
            gl_type<double>(),
            GLboolean(normalized ? GL_TRUE : GL_FALSE),
            m_currentOffset
        };

        m_attributes.push_back(attr);
        m_currentOffset += sizeof(double) * count;
        return *this;
    }

    void vertex_layout::enable() const {
        for (const auto& attr : m_attributes) {
            glEnableVertexAttribArray(attr.index);
            glVertexAttribPointer(
                attr.index,
                attr.size,
                attr.type,
                attr.normalized,
                m_currentOffset,
                reinterpret_cast<const void*>(attr.offset)
            );
        }
    }

    void vertex_layout::disable() const {
        for (const auto& attr : m_attributes) {
            glDisableVertexAttribArray(attr.index);
        }
    }

    std::size_t vertex_layout::stride() const {
        return m_currentOffset;
    }

}