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
		RenderContext() = delete;

		static bool init();
		static void close();
		static void clear();
		static void setClearColor(glm::vec4 color);
		static void setViewport(int x, int y, int w, int h);
		static void setDefaultViewport(int x, int y, int w, int h);
		static void setState(std::shared_ptr<RenderStates> state);
		static void beginPass(std::shared_ptr<Framebuffer> fb = nullptr);
		static void submit(std::unique_ptr<RenderCmd::Command> command);
		static void endPass();

	private:
		static int x, y, w, h;

		static void flush();

		static std::queue<std::unique_ptr<RenderCmd::Command>> commandList;
		static std::weak_ptr<Framebuffer> curr_fb;
		static std::weak_ptr<RenderStates> curr_state;
		static std::shared_ptr<RenderTarget> target;
	};
}
