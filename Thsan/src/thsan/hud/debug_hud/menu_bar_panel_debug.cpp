#include "pch.h"
#include "thsan/hud/debug_hud/menu_bar_panel_debug.h"
#include "imgui.h"
#include "thsan/options.h"

Thsan::MenuBarPanelDebug::MenuBarPanelDebug(Options* pOptions):
    pOptions(pOptions)
{

}

void Thsan::MenuBarPanelDebug::drawContent()
{
    // Create the menu bar
    if (ImGui::BeginMainMenuBar()) // Start the main menu bar
    {
        // File menu
        if (ImGui::BeginMenu("File")) // Create a "File" menu
        {
            // Menu items under "File"
            if (ImGui::MenuItem("New"))
            {
                // Action for "New" item
                // e.g., create a new file or reset some state
            }
            if (ImGui::MenuItem("Open"))
            {
                // Action for "Open" item
                // e.g., open a file dialog
            }
            if (ImGui::MenuItem("Save"))
            {
                // Action for "Save" item
                // e.g., save the current state
            }
            if (ImGui::MenuItem("Exit"))
            {
                // Action for "Exit" item
                // e.g., close the application or terminate the program
                // Here, we'll use ImGui to close the application if needed.
                // ImGui::GetIO().WantCaptureMouse = false; // Uncomment to disable mouse capture
                // exit(0); // Use with caution, can terminate the program
            }

            ImGui::EndMenu(); // End the "File" menu
        }

        // Debug menu
        if (ImGui::BeginMenu("Debug")) // Create a "Debug" menu
        {
            if (ImGui::MenuItem("None", "", pOptions->renderOptions.debugRender == DebugRender::None))
            {
                pOptions->renderOptions.debugRender = DebugRender::None;
            }

            if (ImGui::MenuItem("Normal", "", pOptions->renderOptions.debugRender == DebugRender::Normal))
            {
                pOptions->renderOptions.debugRender = DebugRender::Normal;
            }

            if (ImGui::MenuItem("Depth", "", pOptions->renderOptions.debugRender == DebugRender::Depth))
            {
                pOptions->renderOptions.debugRender = DebugRender::Depth;
            }

            if (ImGui::MenuItem("Position", "", pOptions->renderOptions.debugRender == DebugRender::Position))
            {
                pOptions->renderOptions.debugRender = DebugRender::Position;
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}
