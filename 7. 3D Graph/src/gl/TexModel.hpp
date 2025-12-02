#pragma once

#include "Texture.hpp"
#include "mesh.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace gl {

    class TexModel {
    private:
        Texture    m_texture;
        Mesh   m_mesh;

        glm::vec3 m_position;
        glm::quat m_rotation;
        glm::vec3 m_scale;

        float m_widthWorld;   // world-space width
        float m_heightWorld;  // world-space height

    public:
        TexModel();
        ~TexModel() = default;

        TexModel(const TexModel&) = delete;
        TexModel& operator=(const TexModel&) = delete;

        TexModel(TexModel&&) noexcept = default;
        TexModel& operator=(TexModel&&) noexcept = default;

        bool load(const std::string& path,
            float pixelsPerUnit = 100.0f,   // converts px → world units
            bool flipVertically = true);

        // Transform
        void setPosition(const glm::vec3& p);
        void setRotation(const glm::quat& q);
        void rotateX(float deg);
        void rotateY(float deg);
        void rotateZ(float deg);
        void setScale(const glm::vec3& s);

        glm::mat4 modelMatrix() const;

        // Draw
        void draw(GLenum mode = GL_TRIANGLES) const;

        // Getters
        float widthWorld()  const { return m_widthWorld; }
        float heightWorld() const { return m_heightWorld; }

        Texture& texture() { return m_texture; }
        const Texture& texture() const { return m_texture; }
    };
}