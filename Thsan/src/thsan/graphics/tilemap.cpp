#include "pch.h"
#include "tilemap.h"
#include "tilemap_impl.h"

namespace Thsan {

	THSAN_API std::shared_ptr<Tilemap> create_tilemap()
	{
		return std::make_shared<TilemapImpl>();
	}
}