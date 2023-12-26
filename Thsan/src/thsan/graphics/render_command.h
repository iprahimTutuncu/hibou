#pragma once
#include "thsan/core.h"
#include <memory>

namespace Thsan {

	class Mesh;
	class Shader;
	class RenderTarget;
	class RenderStates2D;
	class Framebuffer;
	class RenderManager;
	class Drawable;

	namespace renderCommands {

		class THSAN_API RenderCommand {
		public:
			RenderCommand() = default;
			RenderCommand(RenderCommand&) = default;
			virtual ~RenderCommand() = default;

			virtual void execute(const std::weak_ptr<RenderTarget> target, RenderManager& renderManager) = 0;
		protected:
		};

		class RenderMesh: public RenderCommand {
		public:
			RenderMesh(std::weak_ptr<Mesh> mesh, std::weak_ptr<RenderStates2D> renderStates2D):
				mesh(mesh),
				renderStates2D(renderStates2D)
			{
			}

			~RenderMesh() = default;
			void execute(const std::weak_ptr<RenderTarget> target, RenderManager& renderManager) override;
		private:
			std::weak_ptr<Mesh> mesh;
			std::weak_ptr<RenderStates2D> renderStates2D;

		};

		class RenderDrawable : public RenderCommand {
		public:
			RenderDrawable(std::weak_ptr<Drawable> drawable, std::weak_ptr<RenderStates2D> renderStates2D) :
				drawable(drawable),
				renderStates2D(renderStates2D)
			{
			}

			~RenderDrawable() = default;
			void execute(const std::weak_ptr<RenderTarget> target, RenderManager& renderManager) override;
		private:
			std::weak_ptr<Drawable> drawable;
			std::weak_ptr<RenderStates2D> renderStates2D;

		};

		class PushFramebuffer : public RenderCommand {
		public:
			PushFramebuffer(std::weak_ptr<Framebuffer> framebuffer) :
				framebuffer(framebuffer)
			{
			}

			~PushFramebuffer() = default;
			void execute(const std::weak_ptr<RenderTarget> target, RenderManager& renderManager) override;
		private:
			std::weak_ptr<Framebuffer> framebuffer;

		};

		class PopFramebuffer : public RenderCommand {
		public:
			PopFramebuffer() = default;
			~PopFramebuffer() = default;
			void execute(const std::weak_ptr<RenderTarget> target, RenderManager& renderManager) override;
		};

		inline THSAN_API std::unique_ptr<RenderCommand> create_renderMeshCommand(std::weak_ptr<Mesh> mesh, std::weak_ptr<RenderStates2D> renderState);
		inline THSAN_API std::unique_ptr<RenderCommand> create_pushFramebufferCommand(std::weak_ptr<Framebuffer> framebuffer);
		inline THSAN_API std::unique_ptr<RenderCommand> create_popFramebufferCommand();
		inline THSAN_API std::unique_ptr<RenderCommand> create_renderDrawableCommand(std::weak_ptr<Drawable> drawable, std::weak_ptr<RenderStates2D> renderStates2D);
	}
}