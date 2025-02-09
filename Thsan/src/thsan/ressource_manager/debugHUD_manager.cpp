#include "pch.h"
#include "thsan/ressource_manager/debugHUD_manager.h"
#include "thsan/window.h"
#include "thsan/log.h"
#include "thsan/hud/debug_hud/panel_debug.h"

namespace Thsan {

    DebugHUDManager::DebugHUDManager(Window* window) {
        // Assuming window has methods to get SDL_Window and SDL_GLContext
        imguiHandler = window->generateImGuiHandler();

        if (!init()) {
            TS_CORE_ERROR("Error: In, DebugHUDManager::DebugHUDManager(Window* window), initalisation failed");
        }
    }

    DebugHUDManager::~DebugHUDManager() {
        shutdown();
    }

    bool DebugHUDManager::init() {
        return imguiHandler->init();
    }

    void DebugHUDManager::shutdown() {
        imguiHandler->shutdown();
    }

    void DebugHUDManager::render() {
        imguiHandler->newFrame();

        for (const auto& panel : panels) {
            panel.second->drawContent();
        }

        imguiHandler->render();
    }

    void DebugHUDManager::remove(const std::string& name) {
        panels.erase(
            std::remove_if(panels.begin(), panels.end(),
                [&name](const std::pair<std::string, std::unique_ptr<PanelDebug>>& panel) {
                    return panel.first == name;
                }),
            panels.end());
    }
}
