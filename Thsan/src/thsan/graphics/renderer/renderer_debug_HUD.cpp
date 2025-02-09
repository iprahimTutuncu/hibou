#include "pch.h"
#include "thsan/graphics/renderer/renderer_debug_HUD.h"
#include "imgui.h"

namespace Thsan {

    bool RendererDebugHUDImpl::init() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        return true;
    }

    void RendererDebugHUDImpl::close() {
        ImGui::DestroyContext();
    }

    void RendererDebugHUDImpl::clear() {

    }

    void RendererDebugHUDImpl::setView(std::shared_ptr<View> view) {

    }

    void RendererDebugHUDImpl::setViewport(int x, int y, int w, int h) {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
    }

    void RendererDebugHUDImpl::setDefaultViewport(int x, int y, int w, int h) {
        setViewport(x, y, w, h);
    }

    void RendererDebugHUDImpl::display() {
        if (isShowEnable) {
            //do something
        }
        ImGui::Render();
    }

    void RendererDebugHUDImpl::show() {
        isShowEnable = true;
    }

    void RendererDebugHUDImpl::hide() {
        isShowEnable = false;
    }

    RendererDebugHUD* RendererDebugHUD::create()
    {
        return new RendererDebugHUDImpl();
    }

} // namespace Thsan
