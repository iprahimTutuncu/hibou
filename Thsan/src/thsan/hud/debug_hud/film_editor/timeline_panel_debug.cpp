#include "pch.h"
#include <thsan/options.h>
#include <thsan/hud/debug_hud/film_editor/timeline_panel_debug.h>
#include <neo_sequencer/imgui_neo_sequencer.h>
#include <neo_sequencer/imgui_neo_internal.h>

namespace Thsan
{
	TimelinePanelDebug::TimelinePanelDebug(Options* pOptions):
		pOptions(pOptions)
	{

	}

    void TimelinePanelDebug::drawContent()
    {
        float screenHeight = pOptions->windowOptions.screenHeight;
        float screenWidth = pOptions->windowOptions.screenWidth;
        float panelWidth = screenWidth;
        float panelHeight = screenHeight * 0.3f; // 30% of screen height
        float marginTop = 20.f - 1.f + (screenHeight * 0.7f); // Below the selection menu

        // Set panel position and constraints
        ImGui::SetNextWindowPos(ImVec2(0, marginTop));
        ImGui::SetNextWindowSizeConstraints(ImVec2(panelWidth, panelHeight), ImVec2(panelWidth, panelHeight));

        // Window flags to prevent hiding, resizing, or moving
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoTitleBar;

        ImGui::Begin("Timeline", nullptr, windowFlags);
        
        // TODO, trouve un sequencer de timeline or something. Ou fait en un toi même
        // regarde ce qui existe. Idealement copy aesprite

        ImGui::End();
    }

}
