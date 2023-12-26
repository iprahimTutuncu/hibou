#pragma once
#include "thsan/graphics/render_target.h"

namespace Thsan {

	class GLRenderTarget: public RenderTarget {
	public:
		GLRenderTarget() = default;
		~GLRenderTarget() = default;

		bool init() override;
		void clear() override;
		void setViewport(int x, int y, int w, int h) override;
		void setClearColor(float r, float g, float b, float a) override;
		void draw(const Mesh& mesh) const override;
	private:

	};
}