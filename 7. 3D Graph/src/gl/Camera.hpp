#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace gl {

    class Camera {
    public:
        Camera(
            const glm::vec3& position = glm::vec3(0.0f, 0.0f, 3.0f),
            const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f),
            const glm::vec3& front = glm::vec3(0.0f, 0.0f, -1.0f)
        );

        // Movement
        void moveAbs(const glm::vec3& delta);   // world-space translation
        void move(const glm::vec3& delta);      // local space (Along right/up/front)

        // Rotations
        void rotateEuler(float yawDeg, float pitchDeg, float rollDeg = 0.0f);
        void rotateAxisAngle(const glm::vec3& axis, float angleDeg);

        // Individual axis rotations (local camera axes)
        void rotateX(float angleDeg);
        void rotateY(float angleDeg);
        void rotateZ(float angleDeg);

        // Setters
        void setPosition(const glm::vec3& pos);
        void setFront(const glm::vec3& fwd);
        void setUp(const glm::vec3& up);

        // Getters
        const glm::vec3& getPosition() const;
        const glm::vec3& getFront()    const;
        const glm::vec3& getUp()       const;
        const glm::vec3& getRight()    const;

        // View matrix
        glm::mat4 getMatrix() const;

    private:
        void updateVectors();

        glm::vec3 m_position;
        glm::vec3 m_front;
        glm::vec3 m_up;
        glm::vec3 m_right;
    };

} // namespace gl
