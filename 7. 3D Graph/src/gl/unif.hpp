#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace gl {

    class unif {
    private:
        GLuint m_programID;
        GLint m_location;
        std::string m_name;

    public:
        unif(GLuint program = 0, const std::string& name = "", GLint location = -1);

        // -------------------- Setters --------------------
        unif& operator=(int v);
        unif& operator=(float v);
        unif& operator=(bool v);
        unif& operator=(const glm::vec2& v);
        unif& operator=(const glm::vec3& v);
        unif& operator=(const glm::vec4& v);
        unif& operator=(const glm::mat3& v);
        unif& operator=(const glm::mat4& v);

        // -------------------- Getters (casts) --------------------
        operator int() const;
        operator float() const;
        operator glm::vec2() const;
        operator glm::vec3() const;
        operator glm::vec4() const;
        operator glm::mat3() const;
        operator glm::mat4() const;

        // -------------------- Info --------------------
        GLint location() const;
        const std::string& name() const;
        bool valid() const;
    };

}