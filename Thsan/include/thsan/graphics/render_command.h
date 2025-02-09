#pragma once
#include "thsan/core.h"
#include <memory>

namespace Thsan {

	class Mesh;
	class Shader;
	class RenderTarget; 
	class RenderStates;
	class Framebuffer;
	class Renderer2D;
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
			RenderMesh(std::weak_ptr<Mesh> mesh, std::weak_ptr<RenderStates> RenderStates):
				mesh(mesh),
				RenderStates(RenderStates)
			{
			}

			~RenderMesh() = default;
			void execute(const std::weak_ptr<RenderTarget> target) override;
		private:
			std::weak_ptr<Mesh> mesh;
			std::weak_ptr<RenderStates> RenderStates;

		};

		class RenderDrawable : public Command {
		public:
			RenderDrawable(std::weak_ptr<Drawable> drawable, std::weak_ptr<RenderStates> RenderStates) :
				drawable(drawable),
				RenderStates(RenderStates)
			{
			}

			~RenderDrawable() = default;
			void execute(const std::weak_ptr<RenderTarget> target) override;
		private:
			std::weak_ptr<Drawable> drawable;
			std::weak_ptr<RenderStates> RenderStates;

		};

		inline THSAN_API std::unique_ptr<Command> createRenderMeshCommand(std::weak_ptr<Mesh> mesh, std::weak_ptr<RenderStates> renderState);
		inline THSAN_API std::unique_ptr<Command> createRenderDrawableCommand(std::weak_ptr<Drawable> drawable, std::weak_ptr<RenderStates> RenderStates);

	}
}