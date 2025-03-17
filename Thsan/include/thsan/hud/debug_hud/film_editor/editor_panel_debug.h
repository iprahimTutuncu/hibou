#pragma once

#include "thsan/hud/debug_hud/panel_debug.h"
#include <thsan/core.h>
#include <thsan/options.h>

namespace Thsan {

    struct Selected;

    class THSAN_API EditorPanelDebug : public PanelDebug
    {
    public:
        EditorPanelDebug(Options* pOptions, Selected* select);
        virtual ~EditorPanelDebug() = default;

        void drawContent() override;


    private:
        void drawLightEditor();
        void drawDepthEditor();

        bool showDemoWindow = true;
        bool showAnotherWindow = false;
        bool depthChanged = true;
        float floatValue = 0.0f;
        int intValue = 0;

        Options* pOptions;
        Selected* selection;
    };

}
