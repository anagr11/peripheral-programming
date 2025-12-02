#include "Model.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace gl {

    // ---------------------------
    // Constructor
    // ---------------------------
    Model::Model()
        : m_position(0.0f), m_rotation(1, 0, 0, 0), m_scale(1.0f) {
    }

    Model::Model(Mesh&& mesh)
        : m_mesh(std::move(mesh)),
        m_position(0.0f),
        m_rotation(1, 0, 0, 0),
        m_scale(1.0f) {
    }

    // ---------------------------
    // Transform Setters
    // ---------------------------
    void Model::setPosition(const glm::vec3& p) { m_position = p; }
    void Model::setRotation(const glm::quat& q) { m_rotation = q; }
    void Model::setEuler(float pitch, float yaw, float roll) {
        m_rotation = glm::quat(glm::vec3(pitch, yaw, roll));
    }
    void Model::setScale(const glm::vec3& s) { m_scale = s; }

    // ---------------------------
    // Transform Operations
    // ---------------------------
    void Model::translate(const glm::vec3& delta) {
        m_position += delta;
    }

    void Model::rotateEuler(float pitch, float yaw, float roll) {
        glm::quat q = glm::quat(glm::vec3(pitch, yaw, roll));
        m_rotation = q * m_rotation;
    }

    void Model::rotateQuat(const glm::quat& q) {
        m_rotation = q * m_rotation;
    }

    // Axis-specific rotation helpers
    void Model::rotateX(float radians) {
        m_rotation = glm::angleAxis(radians, glm::vec3(1, 0, 0)) * m_rotation;
    }

    void Model::rotateY(float radians) {
        m_rotation = glm::angleAxis(radians, glm::vec3(0, 1, 0)) * m_rotation;
    }

    void Model::rotateZ(float radians) {
        m_rotation = glm::angleAxis(radians, glm::vec3(0, 0, 1)) * m_rotation;
    }

    void Model::scaleBy(const glm::vec3& factor) {
        m_scale *= factor;
    }

    // ---------------------------
    // Model Matrix
    // ---------------------------
    glm::mat4 Model::modelMatrix() const {
        glm::mat4 m(1.0f);

        m = glm::translate(m, m_position);
        m *= glm::mat4_cast(m_rotation);
        m = glm::scale(m, m_scale);

        return m;
    }

    // ---------------------------
    // Mesh Interface
    // ---------------------------
    void Model::upload(const std::vector<float>& v,
        const std::vector<unsigned int>& idx,
        const vertex_layout& layout) {
        m_mesh.upload(v, idx, layout);
    }

    void Model::upload(const MeshData& data) {
        m_mesh.upload(data);
    }

    void Model::draw(GLenum mode) const {
        m_mesh.draw(mode);
    }

    Mesh& Model::mesh() { return m_mesh; }
    const Mesh& Model::mesh() const { return m_mesh; }

}