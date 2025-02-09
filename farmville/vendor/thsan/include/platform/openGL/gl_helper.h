#pragma once

namespace Thsan {
    void CheckOpenGLError(const char* stmt, const char* fname, int line);
}

#ifdef TS_CONFIG_DEBUG
    #define GL_CHECK(stmt) stmt; CheckOpenGLError(#stmt, __FILE__, __LINE__);
#else
    #define GL_CHECK(stmt) stmt
#endif