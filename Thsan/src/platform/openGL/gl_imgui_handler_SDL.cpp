#include "pch.h"
#include "platform/openGL/gl_imgui_handler_SDL.h"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_sdl2.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include <platform/SDL/sdl_window.h>

namespace Thsan {
    GLimGuiHandlerSDL::GLimGuiHandlerSDL(SDL_Window* window, SDL_GLContext glContext):
        window(window),
        glContext(glContext)
    {
    }

    bool GLimGuiHandlerSDL::init() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        ImGui::StyleColorsDark();

        // Initialize ImGui for SDL and OpenGL
        if (!ImGui_ImplSDL2_InitForOpenGL(window, glContext)) {
            return false;
        }
        if (!ImGui_ImplOpenGL3_Init("#version 130")) {
            return false;
        }
        return true;
    }

    void GLimGuiHandlerSDL::shutdown() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
    }

    void GLimGuiHandlerSDL::newFrame() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
    }

    void GLimGuiHandlerSDL::render() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}
