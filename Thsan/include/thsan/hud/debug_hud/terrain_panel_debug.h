#pragma once

#include "panel_debug.h"
#include <thsan/core.h>

namespace Thsan {
    struct TerrainOptions;

    class THSAN_API TerrainPanelDebug : public PanelDebug {
    public:
        TerrainPanelDebug(TerrainOptions* pTerrainOptions);
        virtual ~TerrainPanelDebug() = default;

        void drawContent() override;

    private:
        TerrainOptions* terrainOptions;
    };

}
