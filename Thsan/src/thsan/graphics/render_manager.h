#pragma once
#include "thsan/core.h"
#include <queue>
#include <memory>
#include <stack>

namespace Thsan {
	class RenderTarget;
	class Framebuffer;
	class RenderContext;
	class RenderStates2D;
	class RenderStates3D;
	class Drawable;

	class THSAN_API RenderManager {
	public:	
		RenderManager() = default;
		~RenderManager() = default;
		
		virtual bool init() = 0;
		virtual void close() = 0;
		virtual void clear() = 0;
		virtual void setViewport(int x, int y, int w, int h) = 0;
		virtual void setDefaultViewport(int x, int y, int w, int h) = 0;
		virtual void setState(std::shared_ptr<RenderStates2D> states) = 0;
		virtual void setState(std::shared_ptr<RenderStates3D> states) = 0;
		virtual void display() = 0;

		virtual void add(std::shared_ptr<Drawable> drawable) = 0;
		virtual void remove(std::shared_ptr<Drawable> drawable) = 0;
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
		void setState(std::shared_ptr<RenderStates2D> states) override;
		void setState(std::shared_ptr<RenderStates3D> states) override;
		void display() override;

		void add(std::shared_ptr<Drawable> drawable) override;
		void remove(std::shared_ptr<Drawable> drawable) override;

	private:
		int x{0}, y{ 0 }, w{ 800 }, h{ 600 };

		std::vector<std::shared_ptr<Drawable>> drawables;

		std::shared_ptr<RenderContext> renderContext;
		std::shared_ptr<Framebuffer> geometryBufferFramebuffer;
		std::shared_ptr<RenderStates2D> states;
		std::shared_ptr<RenderStates3D> states3D;
	};

}