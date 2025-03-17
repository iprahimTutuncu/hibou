#pragma once
#include "pch.h"
#include "thsan/hud/debug_hud/film_editor/selection_panel_debug.h"
#include "thsan/graphics/frame.h"
#include "thsan/ressource_manager/layer_manager.h"

#include "imgui.h"
#include <memory>



namespace Thsan
{
    std::shared_ptr<Frame>* pActiveFrame;

    SelectionPanelDebug::SelectionPanelDebug(Options* pOptions, std::shared_ptr<Frame>* pActiveFrame, Selected* select)
        : pOptions(pOptions), pActiveFrame(pActiveFrame), select(select)
    {
        assert(pActiveFrame && "pActiveFrame must not be null");
    }

    void SelectionPanelDebug::drawContent()
    {
        float screenHeight = pOptions->windowOptions.screenHeight;
        float panelWidth = 200.f;
        float panelHeight = screenHeight * 0.7f;
        float marginTop = 20.f - 1.f;

        // Set panel position and constraints
        ImGui::SetNextWindowPos(ImVec2(0, marginTop));
        ImGui::SetNextWindowSizeConstraints(ImVec2(panelWidth, panelHeight), ImVec2(panelWidth, panelHeight));

        // Window flags to prevent hiding, resizing, or moving
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoTitleBar;

        ImGui::Begin("Selection Panel", nullptr, windowFlags);

        if (ImGui::BeginTabBar("PanelTabs"))
        {
            std::shared_ptr<Frame> frame = (pActiveFrame && *pActiveFrame) ? *pActiveFrame : nullptr;
            if (ImGui::BeginTabItem("Light"))
            {
                if (frame && frame->getLightCount() > 0)
                {
                    static int selectedLight = -1;

                    for (size_t i = 0; i < frame->getLightCount(); i++)
                    {
                        if (frame->getLight(i))
                        {
                            std::string label = "Light " + std::to_string(i + 1);
                            if (ImGui::Selectable(label.c_str(), selectedLight == static_cast<int>(i)))
                            {
                                select->isLightSelected = true;
                                select->pLight = frame->getLight(i);
                                selectedLight = static_cast<int>(i);
                            }
                        }
                    }
                }
                else
                {
                    ImGui::Text("No lights available.");
                }

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Depth Layer")) {
                if (frame && frame->getLayerCount() > 0) {
                    static std::string selectedLayer = "";

                    ImGuiTableFlags flags2 =  ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Borders;

                    if (ImGui::BeginTable("table_context_menu_2", 2, flags2)) {
                        // Set up columns
                        ImGui::TableSetupColumn("Layer", ImGuiTableColumnFlags_WidthFixed, 147.0f);
                        ImGui::TableSetupColumn("Visible");

                        // Populate rows with layers and visibility toggles
                        for (const std::string& layerName : frame->getLayersName()) {
                            auto layer = RessourceManager::LayerManager::get(layerName);

                            if (layer) {
                                bool isSelected = (selectedLayer == layerName);

                                // First column: Layer name
                                ImGui::TableNextRow();
                                ImGui::TableNextColumn();
                                if (ImGui::Selectable(layerName.c_str(), isSelected)) {
                                    select->isLayerSelected = true;

                                    delete[] select->layerName; // Free previously allocated memory
                                    select->layerName = new char[layerName.size() + 1];
                                    strcpy_s(select->layerName, layerName.size() + 1, layerName.c_str());

                                    selectedLayer = layerName;
                                }

                                // Second column: Visibility toggle
                                ImGui::TableNextColumn();
                                bool visible = frame->isLayerVisible(layerName);
                                if (ImGui::Checkbox(("##visible_" + layerName).c_str(), &visible)) {
                                    frame->setLayerVisibility(layerName, visible);
                                }
                            }
                        }

                        ImGui::EndTable();
                    }
                }
                ImGui::EndTabItem();
            }


            else
            {
                ImGui::Text("Depth editing goes here.");
            }

            ImGui::EndTabBar();
        }

        ImGui::End();
    }
}
