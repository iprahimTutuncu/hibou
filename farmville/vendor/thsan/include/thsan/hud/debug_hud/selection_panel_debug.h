#pragma once

#include "panel_debug.h"
#include <thsan/core.h>
#include <thsan/options.h>

namespace Thsan
{

    struct LightProperties;
    class Frame;
    class Layer;

    struct THSAN_API Selected
    {
        LightProperties* pLight;
        Layer* pLayer;

        bool isLayerSelected{ false };
        bool isLightSelected{ false };
    };

    class THSAN_API SelectionPanelDebug : public PanelDebug
    {
    public:
        SelectionPanelDebug(Options* pOptions, std::shared_ptr<Frame>* pActiveFrame, Selected* select);
        virtual ~SelectionPanelDebug() = default;

        void drawContent() override;

    private:
        Options* pOptions;

        std::shared_ptr<Frame>* pActiveFrame;
        Selected* select;
    };

}
