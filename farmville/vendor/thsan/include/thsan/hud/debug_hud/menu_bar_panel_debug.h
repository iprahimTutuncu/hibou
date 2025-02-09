#pragma once

#include "panel_debug.h"
#include <thsan/core.h>

namespace Thsan {
    struct Options;

    class THSAN_API MenuBarPanelDebug : public PanelDebug {
    public:
        MenuBarPanelDebug(Options* pOptions);
        virtual ~MenuBarPanelDebug() = default;

        void drawContent() override;

    private:
        Options* pOptions;
    };

}
