#include "pch.h"
#include <thsan/options.h>
#include <thsan/hud/debug_hud/film_editor/view_panel_debug.h>
#include <thsan/ressource_manager/layer_manager.h>
#include <imgui/imgui.h>
#include <gl/GL.h>
#include <thsan/graphics/compute_shader.h>
#include <thsan/ressource_manager/texture2D_manager.h>

std::shared_ptr<Thsan::Texture2D> resultRGBA{ nullptr };
std::shared_ptr<Thsan::ComputeShader> depthR16ConversionToRGBA{ nullptr };
std::shared_ptr<Thsan::ComputeShader> occludeRGBA8WithMaskCS{ nullptr };

namespace Thsan
{
    ViewPanelDebug::ViewPanelDebug(Options* pOptions, std::shared_ptr<Frame>* pActiveFrame) :
        pOptions(pOptions),
        pFrame(pActiveFrame)
    {
        //size may not change in case of refresh,
        //will definetely crash if a different resolution is reloaded

        RessourceManager::TextureDesc desc;
        desc.data = nullptr;
        desc.format = Thsan::TextureFormat::RGBA8;
        desc.size = glm::uvec2(RessourceManager::LayerManager::width, RessourceManager::LayerManager::height);
        resultRGBA = RessourceManager::Texture2DManager::create("resultRGBA", desc);

        depthR16ConversionToRGBA = Thsan::ComputeShader::create("media/compute_shader/depthR16ConversionToRGBA.cs");
        occludeRGBA8WithMaskCS = Thsan::ComputeShader::create("media/compute_shader/occludeRGBA8WithMask.cs");
    }

    void ViewPanelDebug::drawContent()
    {
        static int selectedTab = 0;  // Selected tab index
        static bool is3DView = false; // Toggle for 2D/3D view

        float screenHeight = pOptions->windowOptions.screenHeight;
        float screenWidth = pOptions->windowOptions.screenWidth;
        float panelWidth = screenWidth - 250.f - 200.f;
        float panelHeight = screenHeight * 0.7f;
        float marginTop = 20.f - 1.f;
        float marginLeft = 200.f;

        // Set panel position and constraints
        ImGui::SetNextWindowPos(ImVec2(marginLeft, marginTop));
        ImGui::SetNextWindowSizeConstraints(ImVec2(panelWidth, panelHeight), ImVec2(panelWidth, panelHeight));

        // Window flags to prevent hiding, resizing, or moving
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoTitleBar;

        ImGui::Begin("view", nullptr, windowFlags);

        // --- GUI Tabs ---
        drawTabs(selectedTab);

        // --- Render the Selected Texture ---
        std::shared_ptr<Frame> frame = *pFrame;
        if (frame)
        {
            uint32_t textureID = getSelectedTexture(selectedTab, frame);

            // Define the desired resolution (this could be the resolution of your image)
            float desiredImageWidth = RessourceManager::LayerManager::width;
            float desiredImageHeight = RessourceManager::LayerManager::height;

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5)); // Padding for the child window

            if (ImGui::BeginChild("ZoomableImagePanel", ImVec2(panelWidth*0.95, panelHeight * 0.87), true))
            {
                // Draw the zoomable image within the child panel
                drawZoomableImage(textureID, desiredImageWidth, desiredImageHeight, panelWidth, panelHeight);
            }
            ImGui::EndChild();

            ImGui::PopStyleVar();  // Restore padding

            // --- 2D / 3D Toggle Button ---
            draw3DViewToggle(is3DView);
        }

        ImGui::End();
    }

    void ViewPanelDebug::drawTabs(int& selectedTab)
    {
        const char* tabs[] = { "Diffuse", "Render", "Mask", "Depth", "Position" };
        if (ImGui::BeginTabBar("TextureTabs"))
        {
            for (int i = 0; i < 5; i++)
            {
                if (ImGui::BeginTabItem(tabs[i]))
                {
                    selectedTab = i;
                    ImGui::EndTabItem();
                }
            }
            ImGui::EndTabBar();
        }
    }

    uint32_t ViewPanelDebug::getSelectedTexture(int selectedTab, std::shared_ptr<Frame> frame)
    {
        uint32_t textureID = 0;
        auto index = frame->getIndex();
        auto layer = RessourceManager::LayerManager::get(frame->getLayersName()[0]);

        bool showDepth = false;
        bool showDiffuse = false;
        std::shared_ptr<Thsan::Texture2D> processedTexture = nullptr;

        switch (selectedTab)
        {
        case 0: // Diffuse
            if (auto diffuseTexture = layer->getTexture(LayerTextureType::Diffuse, index))
            {
                textureID = diffuseTexture->getSpecificRenderAPI_ID();
                showDiffuse = true;
                processedTexture = diffuseTexture;
            }
            break;

        case 1: // Ray Traced Result
            if (auto rayTracedTexture = frame->getTexture(FrameTextureType::RayTracedResult))
                textureID = rayTracedTexture->getSpecificRenderAPI_ID();
            break;

        case 2: // Mask
            if (auto maskTexture = layer->getTexture(LayerTextureType::Mask, index))
                textureID = maskTexture->getSpecificRenderAPI_ID();
            break;

        case 3: // Depth
            if (auto depthTexture = layer->getTexture(LayerTextureType::ModifiedDepth, index))
            {
                textureID = depthTexture->getSpecificRenderAPI_ID();
                showDepth = true;
                processedTexture = depthTexture;  // We'll process the depth texture
            }
            break;

        case 4: // Position
            if (auto positionTexture = layer->getTexture(LayerTextureType::Position, index))
                textureID = positionTexture->getSpecificRenderAPI_ID();
            break;
        }

        // Process depth texture if necessary
        if (showDepth && processedTexture)
        {
            depthR16ConversionToRGBA->use();

            unsigned int dispatchX = (processedTexture->getWidth() + 15) / 16;
            unsigned int dispatchY = (processedTexture->getHeight() + 15) / 16;
            unsigned int dispatchZ = 1;

            depthR16ConversionToRGBA->setImage2D(processedTexture, 0);  // Input
            depthR16ConversionToRGBA->setRWImage2D(resultRGBA, 1);  // Output

            depthR16ConversionToRGBA->dispatch(dispatchX, dispatchY, dispatchZ);

            depthR16ConversionToRGBA->wait();

            textureID = resultRGBA->getSpecificRenderAPI_ID();
        }

        // Process diffuse texture with mask
        if (showDiffuse && processedTexture)
        {
            depthR16ConversionToRGBA->use();

            unsigned int dispatchX = (processedTexture->getWidth() + 15) / 16;
            unsigned int dispatchY = (processedTexture->getHeight() + 15) / 16;
            unsigned int dispatchZ = 1;

            auto mask = layer->getTexture(LayerTextureType::Mask, 0);

            occludeRGBA8WithMaskCS->setImage2D(mask, 0);  // maskTexture
            occludeRGBA8WithMaskCS->setImage2D(processedTexture, 1);  // r16Texture
            occludeRGBA8WithMaskCS->setRWImage2D(resultRGBA, 2);  // modifiedR16Texture

            occludeRGBA8WithMaskCS->setVec4("occlusionColor", glm::vec4(0.f, 1.f, 0.f, 1.f));

            occludeRGBA8WithMaskCS->dispatch(dispatchX, dispatchY, dispatchZ);
            occludeRGBA8WithMaskCS->wait();

            textureID = resultRGBA->getSpecificRenderAPI_ID();
        }

        return textureID;
    }

    void ViewPanelDebug::drawZoomableImage(uint32_t textureID, float desiredImageWidth, float desiredImageHeight, float panelWidth, float panelHeight)
    {
        static float zoomFactor = 1.0f;
        static ImVec2 imagePos(0, 0);
        static ImVec2 imageOffset(0, 0); // To store drag offset

        // Calculate aspect ratio
        float aspectRatio = desiredImageWidth / desiredImageHeight;

        // Image scaling
        float targetHeight = panelHeight * 0.85f;
        float scaledWidth = targetHeight * aspectRatio;

        // Ensure the image fits within the panel
        if (scaledWidth > panelWidth) {
            scaledWidth = panelWidth;
            targetHeight = scaledWidth / aspectRatio;
        }

        // Set up mouse interaction
        ImGui::SetCursorPos(imagePos);

        // Zoom with mouse wheel
        ImGuiIO& io = ImGui::GetIO();
        if (ImGui::IsWindowHovered())
        {
            if (io.MouseWheel > 0)
                zoomFactor *= 1.1f; // Zoom in
            if (io.MouseWheel < 0)
                zoomFactor /= 1.1f; // Zoom out
        }

        // Apply the zoom factor
        float zoomedWidth = scaledWidth * zoomFactor;
        float zoomedHeight = targetHeight * zoomFactor;

        // Handle dragging
        bool isDragging = false;
        if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(0))
        {
            isDragging = true;
            imageOffset.x += ImGui::GetIO().MouseDelta.x;
            imageOffset.y += ImGui::GetIO().MouseDelta.y;
        }

        if (isDragging)
            imagePos = imageOffset;

        // Render the image
        ImGui::Image((void*)textureID, ImVec2(zoomedWidth, zoomedHeight), ImVec2(0, 0), ImVec2(1, 1));
    }




    void ViewPanelDebug::draw3DViewToggle(bool& is3DView)
    {
        if (ImGui::Button(is3DView ? "Switch to 2D View" : "Switch to 3D View"))
        {
            is3DView = !is3DView;
        }
    }

}
