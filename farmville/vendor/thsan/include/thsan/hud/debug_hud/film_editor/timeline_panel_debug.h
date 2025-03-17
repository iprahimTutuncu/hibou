#pragma once
#include "thsan/hud/debug_hud/panel_debug.h"
#include <thsan/core.h>

namespace Thsan 
{
    struct Options;

    class THSAN_API TimelinePanelDebug : public PanelDebug
    {
    public:
        TimelinePanelDebug(Options* pOptions);
        virtual ~TimelinePanelDebug() = default;

        void drawContent() override;

    private:
        Options* pOptions;
    };

}
