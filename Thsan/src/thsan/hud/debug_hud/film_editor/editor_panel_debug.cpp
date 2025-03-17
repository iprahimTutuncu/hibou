#include "pch.h"
#include "thsan/hud/debug_hud/film_editor/editor_panel_debug.h"
#include "thsan/hud/debug_hud/film_editor/selection_panel_debug.h"
#include "thsan/graphics/frame.h"
#include "thsan/ressource_manager/layer_manager.h"
#include "thsan/ressource_manager/texture2D_manager.h"
#include "imgui.h"
#include <glm/gtc/type_ptr.hpp>
#include <thsan/graphics/compute_shader.h>

std::shared_ptr<Thsan::ComputeShader> copyR16CS{ nullptr };
std::shared_ptr<Thsan::ComputeShader> occludeR16WithMaskCS{ nullptr };
std::shared_ptr<Thsan::ComputeShader> depthClampCS{ nullptr };
std::shared_ptr<Thsan::ComputeShader> depthPrecisionCS{ nullptr };
std::shared_ptr<Thsan::ComputeShader> depthToPositionCS{ nullptr };
std::shared_ptr<Thsan::ComputeShader> depthBlurCS{ nullptr };
std::shared_ptr<Thsan::Texture2D> bluredDepthTexture{ nullptr };
std::shared_ptr<Thsan::Texture2D> unChangedModifiedDepthTexture{ nullptr };

std::vector<float> generateGaussianKernel(int kernelSize, float sigma)
{
    std::vector<float> kernel(kernelSize);
    float sum = 0.0f;
    int halfSize = kernelSize / 2;

    for (int i = 0; i < kernelSize; ++i) 
    {
        int x = i - halfSize;
        kernel[i] = std::exp(-0.5f * (x * x) / (sigma * sigma));
        sum += kernel[i];
    }

    for (int i = 0; i < kernelSize; ++i) 
    {
        kernel[i] /= sum;
    }

    return kernel;
}


namespace Thsan
{
    EditorPanelDebug::EditorPanelDebug(Options* pOptions, Selected* selection) :
        pOptions(pOptions), selection(selection)
    {
        copyR16CS = Thsan::ComputeShader::create("media/compute_shader/copyR16.cs");
        occludeR16WithMaskCS = Thsan::ComputeShader::create("media/compute_shader/occludeR16WithMask.cs");
        depthClampCS = Thsan::ComputeShader::create("media/compute_shader/depthClamp.cs");
        depthPrecisionCS = Thsan::ComputeShader::create("media/compute_shader/depthPrecision.cs");
        depthToPositionCS = Thsan::ComputeShader::create("media/compute_shader/depthToPosition.cs");
        depthBlurCS = Thsan::ComputeShader::create("media/compute_shader/depthBlur.cs");

        RessourceManager::TextureDesc desc;
        desc.data = nullptr;
        desc.format = TextureFormat::R16;
        desc.size = glm::uvec2(64, 64); // this will be changed when layer is created
        bluredDepthTexture = RessourceManager::Texture2DManager::create("depthBlurredTexture", desc);
        unChangedModifiedDepthTexture = RessourceManager::Texture2DManager::create("unChangedModifiedDepthTexture", desc);
    }

    void EditorPanelDebug::drawContent()
    {
        float screenHeight = pOptions->windowOptions.screenHeight;
        float screenWidth = pOptions->windowOptions.screenWidth;
        float panelWidth = 250.f;  // Fixed width for editing
        float panelHeight = screenHeight * 0.7f; // 70% of screen height
        float marginTop = 20.f - 1.f;

        // Position the panel on the right side
        ImGui::SetNextWindowPos(ImVec2(screenWidth - panelWidth, marginTop));
        ImGui::SetNextWindowSizeConstraints(ImVec2(panelWidth, panelHeight), ImVec2(screenWidth * 0.2f, panelHeight));

        // Window flags to prevent hiding, resizing, or moving
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoTitleBar;

        ImGui::Begin("Editor Panel", nullptr, windowFlags);

        // Tab bar for "Light Editor" and "Depth Editor"
        if (ImGui::BeginTabBar("Editor Tabs"))
        {
            if (ImGui::BeginTabItem("Light Editor"))
            {
                drawLightEditor(); // Calls the function for the light editor content
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Depth Editor"))
            {
                drawDepthEditor(); // Calls the function for the depth editor content
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImGui::End();
    }

    void EditorPanelDebug::drawLightEditor()
    {
        if (selection && selection->isLightSelected && selection->pLight)
        {
            LightProperties* light = selection->pLight;

            ImGui::Checkbox("Enabled", &light->isEnabled);

            bool isDirectional = light->isDirectional;
            bool isPoint = light->isPoint;
            bool isSpot = light->isSpot;

            if (ImGui::RadioButton("Directional", isDirectional)) 
            {
                light->isDirectional = true;
                light->isPoint = false;
                light->isSpot = false;
            }
            if (ImGui::RadioButton("Point", isPoint)) 
            {
                light->isDirectional = false;
                light->isPoint = true;
                light->isSpot = false;
            }
            if (ImGui::RadioButton("Spot", isSpot)) 
            {
                light->isDirectional = false;
                light->isPoint = false;
                light->isSpot = true;
            }

            if (!light->isDirectional) 
            { 
                ImGui::DragFloat3("Position", glm::value_ptr(light->position), 0.1f);
            }

            if (light->isDirectional || light->isSpot)
            { 
                ImGui::DragFloat3("Direction", glm::value_ptr(light->direction), 0.1f);
            }

            ImGui::SliderFloat("Intensity", &light->intensity, 0.0f, 100.0f);

            float color[3] = { light->radiance.getRed() / 255.0f,
                               light->radiance.getGreen() / 255.0f,
                               light->radiance.getBlue() / 255.0f };

            if (ImGui::ColorEdit3("Radiance", color)) {
                light->radiance.setRGB(static_cast<uint8_t>(color[0] * 255),
                    static_cast<uint8_t>(color[1] * 255),
                    static_cast<uint8_t>(color[2] * 255));
            }

            if (light->isPoint || light->isSpot)
            {
                ImGui::SliderFloat("Radius", &light->radius, 0.1f, 10.0f);
                ImGui::SliderFloat("Falloff", &light->falloff, 0.1f, 10.0f);
            }

            if (light->isSpot) 
            {
                ImGui::SliderFloat("Cutoff Angle", &light->cutoffAngle, 0.0f, 90.0f);
                ImGui::SliderFloat("Outer Cutoff Angle", &light->outerCutoffAngle, 0.0f, 90.0f);
            }
        }
        else
        {
            ImGui::Text("No light selected.");
        }
    }

    void EditorPanelDebug::drawDepthEditor()
    {
        if (selection)
        {
            if (!selection->layerName)
                return;

            auto layer = RessourceManager::LayerManager::get(std::string(selection->layerName));

            if (!layer)
                return;

            // Focal length & distortion coefficients
            depthChanged |= ImGui::DragFloat("Fx", &layer->fx, 1.0f, 0.0001f, 100.0f);
            depthChanged |= ImGui::DragFloat("Fy", &layer->fy, 1.0f, 0.0001f, 100.0f);
            depthChanged |= ImGui::DragFloat("Cx", &layer->cx, 1.0f, 0.001f, 10000.0f);
            depthChanged |= ImGui::DragFloat("Cy", &layer->cy, 1.0f, 0.001f, 10000.0f);
            depthChanged |= ImGui::DragFloat("k1", &layer->k1, 1.0f, -1.0f, 1.0f, "%.3f");
            depthChanged |= ImGui::DragFloat("k2", &layer->k2, 1.0f, -1.0f, 1.0f, "%.3f");

            //write a depth in big for categorization of those, and remove the word depth fromm each
            // 
            // Depth threshold & normalization controls
            ImGui::Text("Depth");
            ImGui::Separator();

            depthChanged |= ImGui::DragFloat("clamp Low", &layer->depthClampLowerBound, 0.01f, 0.0f, 1.0f, "%.4f");
            depthChanged |= ImGui::DragFloat("clamp Up ", &layer->depthClampUpperBound, 0.01f, 0.0f, 1.0f, "%.4f");
            depthChanged |= ImGui::DragFloat("Precision", &layer->depthPrecision,   0.01f, 0.0f, 1.0f, "%.4f");

            depthChanged |= ImGui::DragInt("blur Kernel", &layer->depthBlurKernelSize, 1, 1, 100, "%d");
            depthChanged |= ImGui::DragFloat("blur sigma", &layer->depthblurSigma, 1.0f, 0.01f, 500.0f, "%.3f");
            depthChanged |= ImGui::DragFloat("blur limit", &layer->depthBlurableLimitDistance, 0.01f, 0.00f, 1.f, "%.3f");

            if (true)
            {
                /*
                
                    TODO:
                        - prend toujours la frame 0, potentiellement changer ca pour autre frame
                        - la couleur de mask est à (0, 1.0, 0, 1.0) avoir cela dans le .json
                        - être en mesure d'entrer la couleur de mask à travers les setting de projets
                */

                // toujours passer le mask pour retirer les zones inutiles
                auto depthTexture = layer->getTexture(LayerTextureType::Depth, 0);
                auto modifiedDepthTexture = layer->getTexture(LayerTextureType::ModifiedDepth, 0);
                auto mask = layer->getTexture(LayerTextureType::Mask, 0);

                {
                    occludeR16WithMaskCS->use();

                    occludeR16WithMaskCS->setImage2D(mask, 0);  // maskTexture
                    occludeR16WithMaskCS->setImage2D(depthTexture, 1);  // r16Texture
                    occludeR16WithMaskCS->setRWImage2D(modifiedDepthTexture, 2);  // modifiedR16Texture

                    occludeR16WithMaskCS->setVec4("occlusionColor", glm::vec4(0.f, 1.f, 0.f, 1.f));

                    unsigned int dispatchX = (modifiedDepthTexture->getWidth() + 15) / 16;
                    unsigned int dispatchY = (modifiedDepthTexture->getHeight() + 15) / 16;
                    unsigned int dispatchZ = 1;

                    occludeR16WithMaskCS->dispatch(dispatchX, dispatchY, dispatchZ);
                    occludeR16WithMaskCS->wait();
                }

                // compute pour Precision
                {
                    depthPrecisionCS->use();

                    depthPrecisionCS->setRWImage2D(modifiedDepthTexture, 0);
                    depthPrecisionCS->setFloat("depthPrecision", layer->depthPrecision);

                    unsigned int dispatchX = (modifiedDepthTexture->getWidth() + 15) / 16;
                    unsigned int dispatchY = (modifiedDepthTexture->getHeight() + 15) / 16;
                    unsigned int dispatchZ = 1;

                    depthPrecisionCS->dispatch(dispatchX, dispatchY, dispatchZ);

                    depthPrecisionCS->wait();
                }

                {
                    depthClampCS->use();

                    depthClampCS->setRWImage2D(modifiedDepthTexture, 0);
                    depthClampCS->setFloat("lowerBound", layer->depthClampLowerBound);
                    depthClampCS->setFloat("upperBound", layer->depthClampUpperBound);

                    unsigned int dispatchX = (modifiedDepthTexture->getWidth() + 15) / 16;
                    unsigned int dispatchY = (modifiedDepthTexture->getHeight() + 15) / 16;
                    unsigned int dispatchZ = 1;

                    depthClampCS->dispatch(dispatchX, dispatchY, dispatchZ);

                    depthClampCS->wait();
                }

                {
                    int w = RessourceManager::LayerManager::width;
                    int h = RessourceManager::LayerManager::height;

                    if (!bluredDepthTexture || bluredDepthTexture->getWidth() != w || bluredDepthTexture->getHeight() != h)
                    {
                        RessourceManager::TextureDesc desc;
                        desc.data = nullptr;
                        desc.format = TextureFormat::R16;
                        desc.size = glm::uvec2(w, h);

                        bluredDepthTexture = RessourceManager::Texture2DManager::create("depthBlurredTexture", desc);
                    }
                    
                    if (!unChangedModifiedDepthTexture || unChangedModifiedDepthTexture->getWidth() != w || unChangedModifiedDepthTexture->getHeight() != h)
                    {
                        RessourceManager::TextureDesc desc;
                        desc.data = nullptr;
                        desc.format = TextureFormat::R16;
                        desc.size = glm::uvec2(w, h);

                        unChangedModifiedDepthTexture = RessourceManager::Texture2DManager::create("depthBlurredTexture", desc);
                    }
                    
                    copyR16CS->use();

                    copyR16CS->setImage2D(modifiedDepthTexture, 0);
                    copyR16CS->setRWImage2D(unChangedModifiedDepthTexture, 1);

                    unsigned int dispatchX = (modifiedDepthTexture->getWidth() + 15) / 16;
                    unsigned int dispatchY = (modifiedDepthTexture->getHeight() + 15) / 16;
                    unsigned int dispatchZ = 1;

                    copyR16CS->dispatch(dispatchX, dispatchY, dispatchZ);
                    copyR16CS->wait();

                    //horizontale
                    std::vector<float> kernel = generateGaussianKernel(layer->depthBlurKernelSize, layer->depthblurSigma);

                    depthBlurCS->use();

                    depthBlurCS->setRWImage2D(modifiedDepthTexture, 0);
                    depthBlurCS->setRWImage2D(bluredDepthTexture, 1);
                    depthBlurCS->setRWImage2D(unChangedModifiedDepthTexture, 2);

                    depthBlurCS->setVec2i("iDirection", glm::uvec2(1, 0));
                    depthBlurCS->setInt("kernelSize", layer->depthBlurKernelSize);
                    depthBlurCS->setFloat("blurableLimitDistance", layer->depthBlurableLimitDistance);
                    depthBlurCS->setFloatArray("coeffs", kernel.data(), kernel.size());

                    depthBlurCS->dispatch(dispatchX, dispatchY, dispatchZ);

                    depthBlurCS->wait();


                    //verticale

                    depthBlurCS->use();

                    depthBlurCS->setRWImage2D(bluredDepthTexture, 0);
                    depthBlurCS->setRWImage2D(modifiedDepthTexture, 1);
                    depthBlurCS->setRWImage2D(unChangedModifiedDepthTexture, 2);

                    depthBlurCS->setVec2i("iDirection", glm::uvec2(0, 1));
                    depthBlurCS->setInt("kernelSize", layer->depthBlurKernelSize);
                    depthBlurCS->setFloat("blurableLimitDistance", layer->depthBlurableLimitDistance);

                    depthBlurCS->dispatch(dispatchX, dispatchY, dispatchZ);

                    depthBlurCS->wait();
                }

                // compute la position à partir de de focal et centre

                // compute les normales à partir des position


                depthChanged = false;
            }

        }
    }
}
