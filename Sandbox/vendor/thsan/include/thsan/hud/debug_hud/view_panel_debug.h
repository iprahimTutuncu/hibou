#pragma once

#include "panel_debug.h"
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
        Options* pOptions;
        std::shared_ptr<Frame>* pFrame;
    };

}