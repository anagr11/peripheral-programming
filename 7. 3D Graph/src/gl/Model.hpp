#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Mesh.hpp"

namespace gl {

    class Model {
    private:
        Mesh m_mesh;

        glm::vec3 m_position;
        glm::quat m_rotation;
        glm::vec3 m_scale;

    public:
        Model();
        explicit Model(Mesh&& mesh);

        Model(const Model&) = delete;
        Model& operator=(const Model&) = delete;

        Model(Model&&) noexcept = default;
        Model& operator=(Model&&) noexcept = default;

        // Transform operations
        void setPosition(const glm::vec3& p);
        void setRotation(const glm::quat& q);
        void setEuler(float pitch, float yaw, float roll);
        void setScale(const glm::vec3& s);

        void translate(const glm::vec3& delta);

        void rotateEuler(float pitch, float yaw, float roll);
        void rotateQuat(const glm::quat& q);

        // Axis-rotation helpers
        void rotateX(float radians);
        void rotateY(float radians);
        void rotateZ(float radians);

        void scaleBy(const glm::vec3& factor);

        // Model Matrix
        glm::mat4 modelMatrix() const;

        // Mesh interface
        void upload(const std::vector<float>& v,
            const std::vector<unsigned int>& idx,
            const vertex_layout& layout);

        void upload(const MeshData& data);

        void draw(GLenum mode = GL_TRIANGLES) const;

        Mesh& mesh();
        const Mesh& mesh() const;
    };

}