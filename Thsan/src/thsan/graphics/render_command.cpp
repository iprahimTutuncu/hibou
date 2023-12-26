#include "pch.h"
#include "render_command.h"
#include "thsan/log.h"
#include "GL/glew.h"
#include "thsan/graphics/mesh.h"
#include "thsan/graphics/shader.h"
#include "thsan/graphics/render_target.h"
#include "thsan/graphics/render_manager.h"
#include "thsan/graphics/framebuffer.h"
#include "thsan/graphics/drawable.h"

namespace Thsan {
	namespace renderCommands {
		void RenderMesh::execute(const std::weak_ptr<RenderTarget> target, RenderManager& renderManager)
		{
			std::shared_ptr<Mesh> tmp_mesh = mesh.lock();
			std::shared_ptr<RenderStates2D> tmp_renderStates2D = renderStates2D.lock();
			std::shared_ptr<RenderTarget> tmp_target = target.lock();

			if (tmp_mesh && tmp_renderStates2D) {
				tmp_mesh->bind();
				tmp_renderStates2D->bind();
				tmp_target->draw(*tmp_mesh);
				tmp_renderStates2D->unBind();
				tmp_mesh->unbind();
			}
			else {
				TS_CORE_WARN("Warning: attempting to execute RenderMesh on invalid data, mesh or renderState2D");
			}
		}

		void RenderDrawable::execute(const std::weak_ptr<RenderTarget> target, RenderManager& renderManager)
		{
			std::shared_ptr<Drawable> tmp_drawable = drawable.lock();
			std::shared_ptr<RenderStates2D> tmp_renderStates2D = renderStates2D.lock();
			std::shared_ptr<RenderTarget> tmp_target = target.lock();

			if (tmp_drawable && tmp_renderStates2D)
				tmp_target->draw(*tmp_drawable, *tmp_renderStates2D);
			else
				TS_CORE_WARN("Warning: attempting to execute RenderDrawable on invalid data");
		}

		void PushFramebuffer::execute(const std::weak_ptr<RenderTarget> target, RenderManager& renderManager)
		{
			std::shared_ptr<Framebuffer> tmp_framebuffer = framebuffer.lock();

			if (tmp_framebuffer) {
				renderManager.pushFramebuffer(tmp_framebuffer);
			}
			else {
				TS_CORE_ERROR("error: in PushFramebuffer::execute, framebuffer is not valid\n");
			}
		}

		void PopFramebuffer::execute(const std::weak_ptr<RenderTarget> target, RenderManager& renderManager)
		{
			renderManager.popFramebuffer();
		}

		THSAN_API std::unique_ptr<RenderCommand> create_renderMeshCommand(std::weak_ptr<Mesh> mesh, std::weak_ptr<RenderStates2D> renderStates2D)
		{
			return std::make_unique<RenderMesh>(mesh, renderStates2D);
		}

		THSAN_API std::unique_ptr<RenderCommand> create_renderDrawableCommand(std::weak_ptr<Drawable> drawable, std::weak_ptr<RenderStates2D> renderStates2D)
		{
			return std::make_unique<RenderDrawable>(drawable, renderStates2D);
		}

		THSAN_API std::unique_ptr<RenderCommand> create_pushFramebufferCommand(std::weak_ptr<Framebuffer> framebuffer)
		{
			return std::make_unique<PushFramebuffer>(framebuffer);
		}

		THSAN_API std::unique_ptr<RenderCommand> create_popFramebufferCommand() {

			return std::make_unique<PopFramebuffer>();
		}



	}
}

