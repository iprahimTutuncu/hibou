#include "pch.h"
#include "thsan/graphics/mesh.h"
#include "thsan/log.h"
#include "thsan/graphics/graphic_api.h"
#include "platform/openGL/gl_mesh.h"

namespace Thsan {

	std::shared_ptr<Mesh> Mesh::create(uint32_t vertexCount)
	{
		switch (get_graphic_API())
		{
		case GraphicAPI::openGL: return std::make_shared<GLMesh>(vertexCount);
		case GraphicAPI::vulkan: return nullptr;
		};

		TS_CORE_ERROR("error: RenderTarget not supported for the current graphic API");
		return nullptr;
	}

}
