#pragma once

#include "thsan/hud/debug_hud/panel_debug.h"
#include <thsan/core.h>
#include <thsan/graphics/texture.h>
#include <thsan/graphics/frame.h>

namespace Thsan {
    struct Options;

    class THSAN_API ViewPanelDebug : public PanelDebug
    {
    public:
        ViewPanelDebug(Options* pOptions, std::shared_ptr<Frame>* pActiveFrame);
        virtual ~ViewPanelDebug() = default;

        void drawContent() override;


    private:
        void drawTabs(int& selectedTab);
        uint32_t getSelectedTexture(int selectedTab, std::shared_ptr<Frame> frame);
        void drawZoomableImage(uint32_t textureID, float desiredImageWidth, float desiredImageHeight, float panelWidth, float panelHeight);
        void draw3DViewToggle(bool& is3DView);

        Options* pOptions;
        std::shared_ptr<Frame>* pFrame;
        
    };

}