#pragma once

#include <string>
#include <thsan/core.h>

namespace Thsan {

    class THSAN_API PanelDebug {
    public:
        PanelDebug() = default;
        virtual ~PanelDebug() = default;

        virtual void drawContent() = 0;

    };

}