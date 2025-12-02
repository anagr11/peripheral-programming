#include "Window.hpp"

#include <iostream>

namespace gl {

    Window::Window()
        : handle(nullptr) {
    }

    Window::Window(int width, int height, const std::string& title, bool fullscreen)
        : handle(nullptr), width(width), height(height), title(title), fullscreen(fullscreen) {
        init(width, height, title, fullscreen);
    }

    Window::~Window() {
        terminate();
    }

    // -------------------- Initialization --------------------
    bool Window::init(int width, int height, const std::string& title, bool fullscreen) {
        this->width = width;
        this->height = height;
        this->title = title;
        this->fullscreen = fullscreen;

        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW\n";
            return false;
        }

        // Apply user-provided hints
        for (const auto& hint : hints) {
            glfwWindowHint(hint.first, hint.second);
        }

        handle = createGLFWwindow();
        if (!handle) {
            std::cerr << "Failed to create GLFW window\n";
            glfwTerminate();
            return false;
        }

        glfwMakeContextCurrent(handle);

        // LOAD OPENGL FUNCTIONS USING GLAD
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return -1;
        }

        return true;
    }

    void Window::terminate() {
        if (handle) {
            glfwDestroyWindow(handle);
            handle = nullptr;
        }
        glfwTerminate();
    }

    // -------------------- Private Helper --------------------
    GLFWwindow* Window::createGLFWwindow() {
        GLFWmonitor* monitor = fullscreen ? glfwGetPrimaryMonitor() : nullptr;
        return glfwCreateWindow(width, height, title.c_str(), monitor, nullptr);
    }

    // -------------------- Window Hints --------------------
    void Window::setHint(int hint, int value) {
        hints[hint] = value;
    }

    bool Window::isPressed(int key) {
        return glfwGetKey(handle, key) == GLFW_PRESS;
    }

    // -------------------- Accessors --------------------
    int Window::getWidth() const { return width; }
    int Window::getHeight() const { return height; }
    std::string Window::getTitle() const { return title; }
    bool Window::isFullscreen() const { return fullscreen; }
    GLFWwindow* Window::getHandle() const { return handle; }

    // -------------------- Mutators --------------------
    void Window::setSize(int width, int height) {
        this->width = width;
        this->height = height;
        if (handle) glfwSetWindowSize(handle, width, height);
    }

    void Window::setTitle(const std::string& title) {
        this->title = title;
        if (handle) glfwSetWindowTitle(handle, this->title.c_str());
    }

    // -------------------- Runtime Control --------------------
    bool Window::shouldClose() const {
        return handle && glfwWindowShouldClose(handle);
    }

    void Window::pollEvents() const {
        glfwPollEvents();
    }

    void Window::swapBuffers() const {
        if (handle) glfwSwapBuffers(handle);
    }

}