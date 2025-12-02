#include "Camera.hpp"

namespace gl {

    Camera::Camera(
        const glm::vec3& position,
        const glm::vec3& up,
        const glm::vec3& front)
        : m_position(position),
        m_front(glm::normalize(front)),
        m_up(glm::normalize(up)) {
        updateVectors();
    }

    //
    // Movement
    //
    void Camera::moveAbs(const glm::vec3& delta) {
        m_position += delta;
    }

    void Camera::move(const glm::vec3& delta) {
        m_position += m_right * delta.x;
        m_position += m_up * delta.y;
        m_position += m_front * delta.z;
    }

    //
    // Rotations
    //
    void Camera::rotateEuler(float yawDeg, float pitchDeg, float rollDeg) {
        glm::quat q =
            glm::angleAxis(glm::radians(yawDeg), glm::vec3(0, 1, 0)) *
            glm::angleAxis(glm::radians(pitchDeg), glm::vec3(1, 0, 0)) *
            glm::angleAxis(glm::radians(rollDeg), glm::vec3(0, 0, 1));

        m_front = glm::normalize(q * m_front);
        m_up = glm::normalize(q * m_up);

        updateVectors();
    }

    void Camera::rotateAxisAngle(const glm::vec3& axis, float angleDeg) {
        glm::quat q = glm::angleAxis(glm::radians(angleDeg),
            glm::normalize(axis));

        m_front = glm::normalize(q * m_front);
        m_up = glm::normalize(q * m_up);

        updateVectors();
    }

    //
    // Local axis rotations
    //
    void Camera::rotateX(float angleDeg) {
        glm::quat q = glm::angleAxis(glm::radians(angleDeg), m_right);
        m_front = glm::normalize(q * m_front);
        m_up = glm::normalize(q * m_up);
        updateVectors();
    }

    void Camera::rotateY(float angleDeg) {
        glm::quat q = glm::angleAxis(glm::radians(angleDeg), m_up);
        m_front = glm::normalize(q * m_front);
        m_right = glm::normalize(q * m_right);
        updateVectors();
    }

    void Camera::rotateZ(float angleDeg) {
        glm::quat q = glm::angleAxis(glm::radians(angleDeg), m_front);
        m_up = glm::normalize(q * m_up);
        m_right = glm::normalize(q * m_right);
        updateVectors();
    }

    //
    // Setters
    //
    void Camera::setPosition(const glm::vec3& pos) {
        m_position = pos;
    }

    void Camera::setFront(const glm::vec3& fwd) {
        m_front = glm::normalize(fwd);
        updateVectors();
    }

    void Camera::setUp(const glm::vec3& up) {
        m_up = glm::normalize(up);
        updateVectors();
    }

    //
    // Getters
    //
    const glm::vec3& Camera::getPosition() const { return m_position; }
    const glm::vec3& Camera::getFront()    const { return m_front; }
    const glm::vec3& Camera::getUp()       const { return m_up; }
    const glm::vec3& Camera::getRight()    const { return m_right; }

    //
    // View Matrix
    //
    glm::mat4 Camera::getMatrix() const {
        return glm::lookAt(m_position, m_position + m_front, m_up);
    }

    //
    // Internal vector recomputation
    //
    void Camera::updateVectors() {
        m_front = glm::normalize(m_front);
        m_right = glm::normalize(glm::cross(m_front, m_up));
        m_up = glm::normalize(glm::cross(m_right, m_front));
    }

} // namespace gl
