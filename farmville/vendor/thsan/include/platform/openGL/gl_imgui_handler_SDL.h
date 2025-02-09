#pragma once
#include <thsan/graphics/imgui_handler.h>

struct SDL_Window;
using SDL_GLContext = void*;

namespace Thsan {

    class GLimGuiHandlerSDL : public ImGuiHandler {
    public:
        GLimGuiHandlerSDL(SDL_Window* window, SDL_GLContext glContext);
        ~GLimGuiHandlerSDL() = default;

        bool init() override;
        void shutdown() override;
        void newFrame() override;
        void render() override;
    private:
        SDL_GLContext glContext;
        SDL_Window* window;
    };
}