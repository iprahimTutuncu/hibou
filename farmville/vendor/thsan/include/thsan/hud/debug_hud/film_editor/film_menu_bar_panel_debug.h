#pragma once

#include "thsan/hud/debug_hud/panel_debug.h"
#include <thsan/core.h>
#include <map>
#include <vector>
#include <memory>
#include <filesystem>

namespace fs = std::filesystem;

namespace Thsan 
{
    struct Options;
    class Frame;

    using FramePtr = std::shared_ptr<Thsan::Frame>;
    using KeyframeMap = std::map<int, FramePtr>;
    using FrameList = std::vector<FramePtr>;

    class THSAN_API FilmMenuBarPanelDebug : public PanelDebug
    {
    public:
        FilmMenuBarPanelDebug(Options* pOptions, KeyframeMap* keyframes, FrameList* frames, std::shared_ptr<Frame>* pActiveFrame);
        virtual ~FilmMenuBarPanelDebug() = default;

        void drawContent() override;

        bool importLayer(fs::path frame_folder);
        bool createFrame(int index);
        bool exportFrame(std::string frame_path);


    private:
        Options* pOptions;
        KeyframeMap* keyframes;
        FrameList* frames;

        int* currentFrame{ nullptr };
        int* totalFrames{ nullptr };
    };
}
