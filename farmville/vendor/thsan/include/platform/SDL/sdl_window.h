#pragma once
#include "thsan/window.h"
#include <functional>
#include <vector>
#include <optional>

struct SDL_Window;
using SDL_GLContext = void*;

namespace Thsan {
    class ImGuiHandler;

    class SDLWindow : public Window {
    public:
        SDLWindow();
        ~SDLWindow();

        bool create(const int width, const int height, const char* title) override;
        void setTitle(const char* title) override;
        void setSize(const int width, const int height) override;
        void close() override;
        bool isRunning() override;
        void swapBuffers() override;
        std::vector<Event> pollEvent() override;
        void setResizeCallback(std::function<void(int, int)> callback) override;

        //private and only available for debugHUDmanager singleton?
        // //demander a chatGPT pour un meilleur no no cap

        std::unique_ptr<ImGuiHandler> generateImGuiHandler() override;

    private:
        bool createContextOpenGL() override;
        void destroyContextOpenGL() override;

        SDL_Window* window{ nullptr };
        std::function<void(int, int)> resizeCallback;

        //OPENGL SPECIFIQUE
        SDL_GLContext glContext{ nullptr };

    };
}