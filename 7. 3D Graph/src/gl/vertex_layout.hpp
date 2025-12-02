#pragma once

#include <glad/glad.h>
#include <vector>
#include <cstddef>

namespace gl {

    /**
     * @brief A helper class for defining and applying OpenGL vertex attribute layouts.
     *
     * Usage:
     *   vertex_layout layout(sizeof(Vertex));
     *   layout.add<float>(3); // position
     *   layout.add<float>(2); // texCoord
     *   layout.apply();
     */
    class vertex_layout {
    public:
        struct Attribute {
            GLuint index;
            GLint size;
            GLenum type;
            GLboolean normalized;
            std::size_t offset;
        };

        vertex_layout();

        template<typename T>
        vertex_layout& add(GLint count, bool normalized = false);

        void enable() const;
        void disable() const;

        std::size_t stride() const;

    private:
        std::vector<Attribute> m_attributes;
        std::size_t m_currentOffset;
        GLuint m_nextIndex;
    };

    // Explicit template specializations for supported types
    template<> vertex_layout& vertex_layout::add<float>(GLint count, bool normalized);
    template<> vertex_layout& vertex_layout::add<int>(GLint count, bool normalized);
    template<> vertex_layout& vertex_layout::add<unsigned int>(GLint count, bool normalized);
    template<> vertex_layout& vertex_layout::add<double>(GLint count, bool normalized);

}