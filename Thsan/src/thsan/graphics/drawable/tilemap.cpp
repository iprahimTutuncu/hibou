#include "pch.h"
#include "thsan/graphics/drawable/tilemap.h"
#include "thsan/graphics/drawable/tilemap_impl.h"

std::shared_ptr<Thsan::Tilemap> Thsan::Tilemap::create()
{
    return std::make_shared<Thsan::TilemapImpl>();
}