#pragma once

#include <glad/glad.h>
#include <vector>
#include <cstddef>
#include <string>
#include <unordered_map>

#include "vertex_layout.hpp"

namespace gl {

    class MeshData;

    class Mesh {
    private:
        GLuint m_vao = 0;
        GLuint m_vbo = 0;
        GLuint m_ebo = 0;
        bool   m_hasEBO = false;

        GLsizei m_vertexCount = 0;
        GLsizei m_indexCount = 0;
        std::size_t m_stride = 0;

        std::unordered_map<std::string, std::string> metadata;

    public:
        Mesh() = default;
        ~Mesh();

        Mesh(const Mesh&) = delete;
        Mesh& operator=(const Mesh&) = delete;
        Mesh(Mesh&& other) noexcept;
        Mesh& operator=(Mesh&& other) noexcept;

        void upload(const std::vector<float>& vertexData,
            const std::vector<unsigned int>& indices,
            const vertex_layout& layout);
        
        void upload(const MeshData& data);

        void draw(GLenum mode = GL_TRIANGLES) const;
        void destroy();

        GLuint vao() const { return m_vao; }
        GLuint vbo() const { return m_vbo; }
        GLuint ebo() const { return m_ebo; }
        GLsizei vertexCount() const { return m_vertexCount; }
        GLsizei indexCount() const { return m_indexCount; }

        std::string getMeta(const std::string& key) const;

        void setMeta(const std::string& key, const std::string& val);

    private:
        void moveFrom(Mesh&& o) noexcept;
    };

}
