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

	namespace RenderCmd {

		class THSAN_API Command {
		public:
			Command() = default;
			Command(Command&) = default;
			virtual ~Command() = default;

			virtual void execute(const std::weak_ptr<RenderTarget> target) = 0;
		protected:
		};

		class RenderMesh: public Command {
		public:
			RenderMesh(std::weak_ptr<Mesh> mesh, std::weak_ptr<RenderStates2D> renderStates2D):
				mesh(mesh),
				renderStates2D(renderStates2D)
			{
			}

			~RenderMesh() = default;
			void execute(const std::weak_ptr<RenderTarget> target) override;
		private:
			std::weak_ptr<Mesh> mesh;
			std::weak_ptr<RenderStates2D> renderStates2D;

		};

		class RenderDrawable : public Command {
		public:
			RenderDrawable(std::weak_ptr<Drawable> drawable, std::weak_ptr<RenderStates2D> renderStates2D) :
				drawable(drawable),
				renderStates2D(renderStates2D)
			{
			}

			~RenderDrawable() = default;
			void execute(const std::weak_ptr<RenderTarget> target) override;
		private:
			std::weak_ptr<Drawable> drawable;
			std::weak_ptr<RenderStates2D> renderStates2D;

		};

		inline THSAN_API std::unique_ptr<Command> create_renderMeshCommand(std::weak_ptr<Mesh> mesh, std::weak_ptr<RenderStates2D> renderState);
		inline THSAN_API std::unique_ptr<Command> create_renderDrawableCommand(std::weak_ptr<Drawable> drawable, std::weak_ptr<RenderStates2D> renderStates2D);
	}
}