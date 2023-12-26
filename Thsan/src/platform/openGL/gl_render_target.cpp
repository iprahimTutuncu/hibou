#include "pch.h"
#include "gl_render_target.h"
#include "gl_helper.h"
#include "thsan/graphics/mesh.h"
#include "thsan/graphics/drawable.h"
#include "thsan/log.h"

#include <GL/glew.h>

namespace Thsan {

	bool GLRenderTarget::init()
	{
		TS_CORE_INFO("OpenGL info \n Vendor:\t{}\n Renderer:\t{}\n Version:\t{}",
			(const char*)glGetString(GL_VENDOR),
			(const char*)glGetString(GL_RENDERER),
			(const char*)glGetString(GL_VERSION)
		);

#ifdef TS_CONFIG_DEBUG
		GL_CHECK(glEnable(GL_DEBUG_OUTPUT));
#endif // TS_CONFIG_DEBUG

		GL_CHECK(glEnable(GL_DEPTH_TEST));
		GL_CHECK(glDepthFunc(GL_LEQUAL));

		GL_CHECK(glEnable(GL_BLEND));
		GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		glClearColor(0.2f, 0.2f, 0.4f, 1.0f);
		return true;
	}

	void GLRenderTarget::clear()
	{
		GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	void GLRenderTarget::setViewport(int x, int y, int w, int h)
	{
		GL_CHECK(glViewport(x, y, w, h));
	}

	void GLRenderTarget::setClearColor(float r, float g, float b, float a)
	{
		GL_CHECK(glClearColor(r, g, b, a));
	}

	void GLRenderTarget::draw(const Mesh& mesh) const
	{
		GL_CHECK(glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.getIndiceCount()), GL_UNSIGNED_INT, 0));
	}

}