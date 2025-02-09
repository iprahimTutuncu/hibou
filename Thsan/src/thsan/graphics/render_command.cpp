#include "pch.h"
#include "thsan/log.h"
#include "GL/glew.h"
#include "thsan/graphics/render_command.h"
#include "thsan/graphics/mesh.h"
#include "thsan/graphics/shader.h"
#include "thsan/graphics/render_target.h"
#include "thsan/graphics/renderer/renderer_2D.h"
#include "thsan/graphics/framebuffer.h"
#include "thsan/graphics/drawable.h"

namespace Thsan {
	namespace RenderCmd {
		void RenderMesh::execute(const std::weak_ptr<RenderTarget> target)
		{ 
			std::shared_ptr<Mesh> tmp_mesh = mesh.lock();
			std::shared_ptr<Thsan::RenderStates> tmp_RenderStates = RenderStates.lock();
			std::shared_ptr<RenderTarget> tmp_target = target.lock();

			if (tmp_mesh && tmp_RenderStates) {
				tmp_mesh->bind();
				tmp_RenderStates->bind();
				tmp_target->draw(*tmp_mesh);
				tmp_RenderStates->unBind();
				tmp_mesh->unbind();
			}
			else {
				TS_CORE_WARN("Warning: attempting to execute RenderMesh on invalid data, mesh or renderState2D");
			}
		}

		void RenderDrawable::execute(const std::weak_ptr<RenderTarget> target)
		{
			std::shared_ptr<Drawable> tmp_drawable = drawable.lock();
			std::shared_ptr<Thsan::RenderStates> tmp_RenderStates = RenderStates.lock();
			std::shared_ptr<RenderTarget> tmp_target = target.lock();

			if (tmp_drawable && tmp_RenderStates)
				tmp_target->draw(*tmp_drawable, *tmp_RenderStates);
			else
				TS_CORE_WARN("Warning: attempting to execute RenderDrawable on invalid data");
		}

		THSAN_API std::unique_ptr<Command> createRenderMeshCommand(std::weak_ptr<Mesh> mesh, std::weak_ptr<RenderStates> RenderStates)
		{
			return std::make_unique<RenderMesh>(mesh, RenderStates);
		}

		THSAN_API std::unique_ptr<Command> createRenderDrawableCommand(std::weak_ptr<Drawable> drawable, std::weak_ptr<RenderStates> RenderStates)
		{
			return std::make_unique<RenderDrawable>(drawable, RenderStates);
		}

	}
}

