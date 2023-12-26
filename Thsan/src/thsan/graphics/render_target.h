#pragma once
#include "thsan/core.h"
#include "thsan/non_copyable.h"

namespace Thsan {
	class Mesh;
	class Drawable;
	class RenderStates2D;

	class THSAN_API RenderTarget: public NonCopyable {
	public:
		RenderTarget() = default;
		~RenderTarget() = default;
		virtual bool init() = 0;
		virtual void clear() = 0;
		virtual void setViewport(int x, int y, int w, int h) = 0;
		virtual void setClearColor(float r, float g, float b, float a) = 0;
		virtual void draw(const Mesh& mesh) const = 0;
		void draw(const Mesh& mesh, RenderStates2D& states) const;
		void draw(const Drawable& drawable, RenderStates2D& states) const;
	};

	THSAN_API std::shared_ptr<RenderTarget> create_renderTarget();
}