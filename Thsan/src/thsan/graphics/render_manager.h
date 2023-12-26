#pragma once
#include "thsan/core.h"
#include <queue>
#include <memory>
#include <stack>

namespace Thsan {
	class RenderTarget;
	class Framebuffer;

	namespace renderCommands {
		class RenderCommand;
		class PushFramebuffer;
		class PopFramebuffer;
	};

	class THSAN_API RenderManager {
	public:	
		RenderManager() = default;
		~RenderManager() = default;
		
		virtual bool init() = 0;
		virtual void close() = 0;
		virtual void clear() = 0;
		virtual void setViewport(int x, int y, int w, int h) = 0;
		virtual void setDefaultViewport(int x, int y, int w, int h) = 0;
		virtual std::shared_ptr<Framebuffer> getActiveFramebuffer() = 0;
		virtual void submit(std::unique_ptr<renderCommands::RenderCommand> rc) = 0;
		virtual void flush() = 0;
	protected:
		virtual void pushFramebuffer(std::shared_ptr<Framebuffer> framebuffer) = 0;
		virtual void popFramebuffer() = 0;

		friend class renderCommands::PushFramebuffer;
		friend class renderCommands::PopFramebuffer;
	};

	THSAN_API RenderManager* create_renderManager();

	class RenderManagerImpl: public RenderManager {
	public:
		RenderManagerImpl() = default;
		~RenderManagerImpl() = default;

		bool init() override;
		void close() override;
		void clear() override;
		void setViewport(int x, int y, int w, int h) override;
		void setDefaultViewport(int x, int y, int w, int h) override;
		std::shared_ptr<Framebuffer> getActiveFramebuffer() override;
		void submit(std::unique_ptr<renderCommands::RenderCommand> rc) override;
		void flush() override;

	private:
		int x{0}, y{ 0 }, w{ 800 }, h{ 600 };

		std::queue<std::unique_ptr<renderCommands::RenderCommand>> renderCommands;
		std::shared_ptr<RenderTarget> target;
		std::shared_ptr<Framebuffer> geometryBufferFramebuffer;
	};

}