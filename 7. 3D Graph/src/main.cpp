#include "main.hpp"
#include "controls.hpp"

#include <iostream>
#include <numbers>

#include "gl/Shader.hpp"
#include "gl/Mesh.hpp"
#include "gl/MeshParser.hpp"
#include "gl/governor.hpp"
#include "gl/Model.hpp"
#include "gl/TexModel.hpp"

gl::Window window;
gl::Camera camera;

const float fov = 75;
glm::mat4 mat_persp, mat_view;

float lastTime, currTime, deltaTime;

int main() {
    // Initialize window
    window.init(800, 600, "UPY YUPI");

    // Load Shaders
    gl::Shader s_colors;
    s_colors.attach("./shaders/colors");
    s_colors.use();
    auto unif_matrix = s_colors.getUniform("matrix");

    gl::Shader s_cube;
    s_cube.attach("./shaders/cube");

    // Load models
    gl::Mesh m_cube = gl::MeshParser::loadModel("./models/cube.mo");

    gl::Model model_cube2(std::move(gl::MeshParser::loadModel("./models/cube2.mo")));
    model_cube2.setPosition({ -3, 0, 3 });

    gl::TexModel t_cats, t_fav, t_bliss, t_code;
    t_cats.load("./imgs/cats.jpg", 4);
    t_cats.setPosition({-100,0,0});
    t_cats.rotateY(90);

    t_fav.load("./imgs/favicon.jpg", 5);
    t_fav.setPosition({200,0,0});
    t_fav.rotateY(-90);

    t_bliss.load("./imgs/xp-bliss.jpg", 3);
    t_bliss.setPosition({0,0,-500});

    t_code.load("./imgs/code.png", 3);
    t_code.setPosition({0,0,500});
    t_code.rotateY(180);

    // SETUP VIEW MATRICES
    mat_persp = glm::perspective((float)(fov * std::numbers::pi / 180.f), 8.f / 6.f, 0.001f, 1000.f);

    // SETUP GL...
    glEnable(GL_DEPTH_TEST); // test for depth...
    //glEnable(GL_CULL_FACE); // Draw only front triangles..

    const int wX = 28;
    const int wY = 5;
    bool wall[wY][wX] = {
        {1,0,1,0,1,1,1,0,1,0,1,0,0,1,0,1,0,1,0,1,0,1,1,1,0,1,0,1},
        {1,0,1,0,1,0,1,0,1,0,1,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1},
        {1,0,1,0,1,1,1,0,0,1,0,0,0,0,1,0,0,1,0,1,0,1,1,1,0,0,1,0},
        {1,0,1,0,1,0,0,0,0,1,0,0,0,0,1,0,0,1,0,1,0,1,0,0,0,0,1,0},
        {1,1,1,0,1,0,0,0,0,1,0,0,0,0,1,0,0,1,1,1,0,1,0,0,0,0,1,0},
    };

    setupControls();

    gl::governor gov;
    //gov.setDebug(true);
    gov.setFPS(60);

    lastTime = 0.0f;
    while (!window.shouldClose()) {
        gov.busy();

        currTime = glfwGetTime();
        deltaTime = currTime - lastTime;
        lastTime = currTime;

        glClearColor(0.4, 0, 0.8, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        window.pollEvents();
        processControls();

        // draw letters
        s_colors.use();
        for(int i = 0; i < wX; i++) {
            for(int j = 0; j < wY; j++) {
                if(!wall[j][i]) continue;
                int jp = wY - j - 1;
                glm::mat4 m = glm::translate(glm::mat4(1), {i, jp, 0});
                unif_matrix = mat_persp * mat_view * m;
                m_cube.draw();
            }
        }

        // Draw a cube
        s_cube.use();
        s_cube.setUniform("u_mode", 1); // MODE: Solid Color

        // draw axes, well, sort of
        s_cube.setUniform("u_solidColor", glm::vec4(1, 0, 0, 1));
        for(int i = 0; i < 50; i += 2) {
            glm::mat4 m = glm::translate(glm::mat4(1), {i, -10, 0});
            s_cube.setUniform("matrix", mat_persp * mat_view * m);
            model_cube2.draw();
        }
        s_cube.setUniform("u_solidColor", glm::vec4(0, 0, 1, 1));
        for(int i = 0; i < 50; i += 2) {
            glm::mat4 m = glm::translate(glm::mat4(1), {0, -10, i});
            s_cube.setUniform("matrix", mat_persp * mat_view * m);
            model_cube2.draw();
        }

        
        s_cube.setUniform("u_solidColor", glm::vec4(0.5, 0.6, 1, 1));
        s_cube.setUniform("matrix", mat_persp * mat_view * model_cube2.modelMatrix());
        model_cube2.draw();

        // MODE: Textured
        s_cube.setUniform("u_mode", 2);

        s_cube.setUniform("matrix", mat_persp * mat_view * t_cats.modelMatrix());
        t_cats.draw();

        s_cube.setUniform("matrix", mat_persp * mat_view * t_fav.modelMatrix());
        t_fav.draw();
        t_fav.rotateX(10 * deltaTime);
        t_fav.rotateY(30 * deltaTime);

        s_cube.setUniform("matrix", mat_persp * mat_view * t_bliss.modelMatrix());
        t_bliss.draw();

        s_cube.setUniform("matrix", mat_persp * mat_view * t_code.modelMatrix());
        t_code.draw();

        window.swapBuffers();
    }
}