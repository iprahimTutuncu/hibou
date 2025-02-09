#pragma once

#include <memory>
#include <vector>
#include <functional>
#include <string>
#include <thsan/graphics/imgui_handler.h>

namespace Thsan {

    class GLimGuiHandlerSDL;
    class Window;
    class PanelDebug;
    class Game;

    class DebugHUDManager {
    public:
        DebugHUDManager(Window* window);
        ~DebugHUDManager();


        template<typename PanelDebugType, typename... Args>
        void add(const std::string& name, Args&&... args)
        {
            panels.emplace_back(name, std::make_unique<PanelDebugType>(std::forward<Args>(args)...));
        }

        void remove(const std::string& name);

    private:
        bool init();
        void shutdown();
        void render();

        std::unique_ptr<ImGuiHandler> imguiHandler;
        std::vector<std::pair<std::string, std::unique_ptr<PanelDebug>>> panels;

        friend class Game;
    };
}
