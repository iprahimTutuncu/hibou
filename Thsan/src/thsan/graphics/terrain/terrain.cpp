#include "pch.h"
#include "thsan/graphics/terrain/terrain.h"
#include "thsan/graphics/terrain/terrain_impl.h"

std::shared_ptr<Thsan::Terrain> Thsan::Terrain::create()
{
    return std::make_shared<TerrainImpl>();
}
