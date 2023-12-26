#include "pch.h"
#include "gl_helper.h"
#include "thsan/log.h"
#include <GL/glew.h>

namespace Thsan {
    void CheckOpenGLError(const char* stmt, const char* fname, int line)
    {
        {
            GLenum err;
            while ((err = glGetError()) != GL_NO_ERROR) {
                TS_CORE_ERROR("OpenGL error {}, at {}:{} - for {}", err, fname, line, stmt);
            }

            TS_CORE_ASSERT(err == GL_NO_ERROR, "openGL error");
        }
    }
}
