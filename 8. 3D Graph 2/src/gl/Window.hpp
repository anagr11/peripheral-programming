#pragma once

#include <string>
#include <unordered_map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace gl {

    class Window {
    private:

        GLFWwindow* handle;
        int width;
        int height;
        std::string title;
        bool fullscreen;
        std::unordered_map<int, int> hints;

    public:

        Window();
        Window(int width, int height, const std::string& title, bool fullscreen = false);
        ~Window();

    public:

        // Initialization
        bool init(int width, int height, const std::string& title, bool fullscreen = false);
        void terminate();

        // Window hints
        void setHint(int hint, int value);

        bool isPressed(int key);

    private:

        GLFWwindow* createGLFWwindow();

    public:

        // Accessors
        int getWidth() const;
        int getHeight() const;
        std::string getTitle() const;
        bool isFullscreen() const;
        GLFWwindow* getHandle() const;

    public:

        // Mutators
        void setSize(int width, int height);
        void setTitle(const std::string& title);

    public:

        // Runtime control
        bool shouldClose() const;
        void pollEvents() const;
        void swapBuffers() const;

    };

}