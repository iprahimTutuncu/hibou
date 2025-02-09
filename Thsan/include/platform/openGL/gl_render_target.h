#pragma once
#include "thsan/graphics/render_target.h"
#include "thsan/graphics/mesh.h"
#include "GL/glew.h"

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
		const std::unordered_map<PrimitiveType, GLenum> primitiveTypeToGL = {
			{ PrimitiveType::Point, GL_POINTS },
			{ PrimitiveType::Lines, GL_LINES },
			{ PrimitiveType::Triangles, GL_TRIANGLES },
			{ PrimitiveType::CustomIndices, GL_NONE },

		};

	};


}