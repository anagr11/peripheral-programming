#include "TexModel.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <vector>
#include <iostream>
#include <numbers>

namespace gl {

    // --------------------------------------------------
    // Constructor
    // --------------------------------------------------
    TexModel::TexModel()
        : m_position(0.0f),
        m_rotation(1, 0, 0, 0),
        m_scale(1.0f),
        m_widthWorld(0),
        m_heightWorld(0) {
    }

    // --------------------------------------------------
    // Load image and create quad mesh
    // --------------------------------------------------
    bool TexModel::load(const std::string& path, float pixelsPerUnit, bool flipVertically) {
        std::cout << "is loading" << std::endl;
        if (!m_texture.loadFromFile(path, flipVertically)) {
            return false;
        }
        std::cout << "loading ok" << std::endl;

        int w = m_texture.width();
        int h = m_texture.height();

        // convert pixels → world units
        m_widthWorld = w / pixelsPerUnit;
        m_heightWorld = h / pixelsPerUnit;

        float hw = m_widthWorld * 0.5f;
        float hh = m_heightWorld * 0.5f;

        // quad vertex data
        //  pos(x,y,z)     uv
        std::vector<float> vertices = {
            - hw, -hh, 0.0f,   0.0f, 0.0f,
              hw, -hh, 0.0f,   1.0f, 0.0f,
              hw,  hh, 0.0f,   1.0f, 1.0f,
            - hw,  hh, 0.0f,   0.0f, 1.0f
        };

        std::vector<unsigned> indices = {
            0,1,2,
            2,3,0
        };

        vertex_layout layout;
        layout.add<float>(3); // pos
        layout.add<float>(2); // uv

        m_mesh.upload(vertices, indices, layout);

        return true;
    }

    // --------------------------------------------------
    // Transform methods
    // --------------------------------------------------
    void TexModel::setPosition(const glm::vec3& p) {
        m_position = p;
    }

    void TexModel::setRotation(const glm::quat& q) {
        m_rotation = q;
    }

    void TexModel::rotateX(float deg) {
        float radians = deg * std::numbers::pi / 180.f;
        m_rotation = glm::angleAxis(radians, glm::vec3(1, 0, 0)) * m_rotation;
    }

    void TexModel::rotateY(float deg) {
        float radians = deg * std::numbers::pi / 180.f;
        m_rotation = glm::angleAxis(radians, glm::vec3(0, 1, 0)) * m_rotation;
    }

    void TexModel::rotateZ(float deg) {
        float radians = deg * std::numbers::pi / 180.f;
        m_rotation = glm::angleAxis(radians, glm::vec3(0, 0, 1)) * m_rotation;
    }

    void TexModel::setScale(const glm::vec3& s) {
        m_scale = s;
    }

    // --------------------------------------------------
    // Create model matrix
    // --------------------------------------------------
    glm::mat4 TexModel::modelMatrix() const {
        glm::mat4 m(1.0f);
        m = glm::translate(m, m_position);
        m *= glm::mat4_cast(m_rotation);
        m = glm::scale(m, m_scale);
        return m;
    }

    // --------------------------------------------------
    // Draw textured quad
    // --------------------------------------------------
    void TexModel::draw(GLenum mode) const {
        m_texture.bind(0);

        glActiveTexture(GL_TEXTURE0);
        m_mesh.draw(mode);
    }
}