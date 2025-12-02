#pragma once

#include <glad/glad.h>
#include <string>

namespace gl {

    class Texture {
    private:
        GLuint      m_id;
        int         m_width;
        int         m_height;
        int         m_channels;

    public:
        Texture();
        ~Texture();

        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;

        Texture(Texture&& other) noexcept;
        Texture& operator=(Texture&& other) noexcept;

        bool loadFromFile(const std::string& path, bool flipVertically = true, GLint minFilter = GL_LINEAR_MIPMAP_LINEAR, GLint magFilter = GL_LINEAR);

        void bind(GLuint unit = 0) const;
        void unbind() const;

        void destroy();

        GLuint id() const { return m_id; }
        int width()  const { return m_width; }
        int height() const { return m_height; }
        int channels() const { return m_channels; }
    };

}