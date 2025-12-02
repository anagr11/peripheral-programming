#include "Mesh.hpp"

#include "MeshParser.hpp"

namespace gl {

    Mesh::~Mesh() {
        destroy();
    }

    Mesh::Mesh(Mesh&& other) noexcept {
        moveFrom(std::move(other));
    }

    Mesh& Mesh::operator=(Mesh&& other) noexcept {
        if (this != &other)
            moveFrom(std::move(other));
        return *this;
    }

    void Mesh::upload(const MeshData& data) {
        this->upload(data.vertices, data.indices, data.layout);
    }

    void Mesh::upload(const std::vector<float>& vertexData,
        const std::vector<unsigned int>& indices,
        const vertex_layout& layout) {
        m_stride = layout.stride();
        m_vertexCount = static_cast<GLsizei>(vertexData.size() * sizeof(float) / m_stride);
        m_indexCount = static_cast<GLsizei>(indices.size());
        m_hasEBO = !indices.empty();

        if (!m_vao) glGenVertexArrays(1, &m_vao);
        if (!m_vbo) glGenBuffers(1, &m_vbo);

        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

        if (m_hasEBO) {
            if (!m_ebo) glGenBuffers(1, &m_ebo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
        }

        layout.enable();
        glBindVertexArray(0);
    }

    void Mesh::draw(GLenum mode) const {
        if (!m_vao) return;
        glBindVertexArray(m_vao);
        if (m_hasEBO)
            glDrawElements(mode, m_indexCount, GL_UNSIGNED_INT, nullptr);
        else
            glDrawArrays(mode, 0, m_vertexCount);
        glBindVertexArray(0);
    }

    void Mesh::destroy() {
        if (m_vbo) glDeleteBuffers(1, &m_vbo);
        if (m_ebo) glDeleteBuffers(1, &m_ebo);
        if (m_vao) glDeleteVertexArrays(1, &m_vao);

        m_vao = m_vbo = m_ebo = 0;
        m_hasEBO = false;
        m_vertexCount = m_indexCount = 0;
        m_stride = 0;
    }

    void Mesh::moveFrom(Mesh&& o) noexcept {
        m_vao = o.m_vao; o.m_vao = 0;
        m_vbo = o.m_vbo; o.m_vbo = 0;
        m_ebo = o.m_ebo; o.m_ebo = 0;

        m_hasEBO = o.m_hasEBO; o.m_hasEBO = false;

        m_vertexCount = o.m_vertexCount; o.m_vertexCount = 0;
        m_indexCount = o.m_indexCount; o.m_indexCount = 0;

        m_stride = o.m_stride; o.m_stride = 0;
    }

    std::string Mesh::getMeta(const std::string& key) const {
        return metadata.at(key);
    }

    void Mesh::setMeta(const std::string& key, const std::string& val) {
        metadata[key] = val;
    }

}
