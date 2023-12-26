#pragma once
#include "thsan/window.h"
#include <functional>
#include <vector>
#include <optional>

#include "../vulkan/vk_swapchain.h"
#include "../vulkan/vk_initializer.h"

struct SDL_Window;
using SDL_GLContext = void*;


namespace Thsan {

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

    private:
        bool createContextOpenGL() override;
        bool createContextVulkan() override;
        void destroyContextOpenGL() override;
        void destroyContextVulkan() override;

        SDL_Window* window{ nullptr };
        std::function<void(int, int)> resizeCallback;

        //VULKAN SPECIFIQUE
        bool createInstance();
        bool createSurface();
        bool selectPhysicalDevice();
        bool createLogicalDevice();

        SwapChainBundle swapchainBundle;

        vk::Instance* instance{ nullptr };
        vk::SurfaceKHR* surface{ nullptr };
        vk::PhysicalDevice* physicalDevice{ nullptr };
        vk::Device* device{ nullptr };
        vk::Queue* graphicQueue{ nullptr };
        vk::Queue* presentQueue{ nullptr };



        //OPENGL SPECIFIQUE
        SDL_GLContext glContext{ nullptr };
    };
}