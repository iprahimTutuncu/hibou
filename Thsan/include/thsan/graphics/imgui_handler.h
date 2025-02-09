#pragma once
#include "thsan/core.h"
#include <memory>

namespace Thsan {

    class ImGuiHandler {
    public:
        virtual ~ImGuiHandler() = default;

        virtual bool init() = 0;
        virtual void shutdown() = 0;
        virtual void newFrame() = 0;
        virtual void render() = 0;

        static std::unique_ptr<ImGuiHandler> create();
    };

}
