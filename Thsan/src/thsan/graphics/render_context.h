#pragma once

#include <memory>
#include <queue>
#include <glm/ext/vector_float4.hpp>

namespace Thsan {

	namespace RenderCmd {
		class Command;
	}
	class RenderTarget;
	class Framebuffer;
	class RenderStates;

	class RenderContext {
	public:
		RenderContext() = default;
		~RenderContext() = default;

		bool init();
		void close();
		void clear();
		void setClearColor(glm::vec4 color);
		void setViewport(int x, int y, int w, int h);
		void setDefaultViewport(int x, int y, int w, int h);
		void setState(std::shared_ptr<RenderStates> state);
		void beginPass(std::shared_ptr<Framebuffer> fb = nullptr);
		void submit(std::unique_ptr<RenderCmd::Command> command);
		void endPass();
	private:
		int x{ 0 }, y{ 0 }, w{ 800 }, h{ 600 };

		void flush();

		std::queue<std::unique_ptr<RenderCmd::Command>> commandList;
		std::weak_ptr<Framebuffer> curr_fb;
		std::weak_ptr<RenderStates> curr_state;
		std::shared_ptr<RenderTarget> target;
	};
}