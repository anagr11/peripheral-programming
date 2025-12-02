#include "Texture.hpp"

#include "../ext/stb_image.h"

#include <iostream>

namespace gl {

    // --------------------------------------
    // Constructor / Destructor
    // --------------------------------------
    Texture::Texture()
        : m_id(0), m_width(0), m_height(0), m_channels(0) {
    }

    Texture::~Texture() {
        destroy();
    }

    // --------------------------------------
    // Move Operations
    // --------------------------------------
    Texture::Texture(Texture&& other) noexcept {
        m_id = other.m_id;
        m_width = other.m_width;
        m_height = other.m_height;
        m_channels = other.m_channels;

        other.m_id = 0;
    }

    Texture& Texture::operator=(Texture&& other) noexcept {
        if (this != &other) {
            destroy();

            m_id = other.m_id;
            m_width = other.m_width;
            m_height = other.m_height;
            m_channels = other.m_channels;

            other.m_id = 0;
        }
        return *this;
    }

    // --------------------------------------
    // Load Texture Using stb_image
    // --------------------------------------
    bool Texture::loadFromFile(const std::string& path, bool flipVertically, GLint minFilter, GLint magFilter) {
        // Destroy old texture if any
        destroy();

        //std::cout << "loading in texture..." << std::endl;
        stbi_set_flip_vertically_on_load(flipVertically);
        unsigned char* data = stbi_load(path.c_str(), &m_width, &m_height, &m_channels, 0);
        //std::cout << "stbi ok..." << std::endl;

        if (!data) {
            std::cerr << "[Texture] Failed to load: " << path << "\n";
            return false;
        }
        
        // MUST ensure m_width > 0 && m_height > 0
        if (m_width == 0 || m_height == 0) {
            std::cerr << "Bad texture size!\n";
            stbi_image_free(data);
            return false;
        }

        GLenum format = 0;
        if (m_channels == 1) format = GL_RED;
        else if (m_channels == 3) format = GL_RGB;
        else if (m_channels == 4) format = GL_RGBA;
        else {
            std::cerr << "Unsupported channel count: " << m_channels << "\n";
            stbi_image_free(data);
            return false;
        }

        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Upload to GPU
        //std::cout << "gpu loading..." << std::endl;
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);
        //std::cout << "gpu mipmap..." << std::endl;
        glGenerateMipmap(GL_TEXTURE_2D);
        //std::cout << "gpu loading ok..." << std::endl;

        glBindTexture(GL_TEXTURE_2D, 0);

        //std::cout << "stbi free..." << std::endl;
        stbi_image_free(data);
        //std::cout << "stbi ok 2..." << std::endl;

        return true;
    }

    // --------------------------------------
    // Bind / Unbind
    // --------------------------------------
    void Texture::bind(GLuint unit) const {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, m_id);
    }

    void Texture::unbind() const {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // --------------------------------------
    // Destroy
    // --------------------------------------
    void Texture::destroy() {
        if (m_id != 0) {
            glDeleteTextures(1, &m_id);
            m_id = 0;
        }
    }

}