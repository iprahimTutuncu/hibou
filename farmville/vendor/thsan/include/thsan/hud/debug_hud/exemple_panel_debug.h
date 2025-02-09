#pragma once

#include "panel_debug.h"
#include <thsan/core.h>

namespace Thsan {

    class THSAN_API ExamplePanelDebug : public PanelDebug {
    public:
        ExamplePanelDebug();
        virtual ~ExamplePanelDebug() = default;

        void drawContent() override;

    private:
        bool showDemoWindow = true;
        bool showAnotherWindow = false;
        float floatValue = 0.0f;
        int intValue = 0;
    };

}
