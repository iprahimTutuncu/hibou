#include "pch.h"
#include "thsan/hud/debug_hud/exemple_panel_debug.h"

#include "imgui.h"

namespace Thsan {

    ExamplePanelDebug::ExamplePanelDebug()
    {
    }

    void ExamplePanelDebug::drawContent() {
        ImGui::Text("Panel debug test");
        ImGui::Separator();

        if (ImGui::Checkbox("Show Demo Window", &showDemoWindow)) {
            // Handle checkbox state change
        }

        if (ImGui::Button("Button")) {
            // Handle button press
        }

        ImGui::InputFloat("Float", &floatValue, 0.1f, 1.0f);
        ImGui::InputInt("Integer", &intValue);

        if (ImGui::CollapsingHeader("Header")) {
            ImGui::Text("Content inside header");
        }

        if (showDemoWindow) {
            ImGui::ShowDemoWindow(&showDemoWindow);
        }
    }

}
