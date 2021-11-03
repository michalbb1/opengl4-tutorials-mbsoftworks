// STL
#include <iostream>

namespace gldebug {

inline void ClearOpenGLError()
{
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "Cleared OpenGL error " << std::hex << err << std::dec << std::endl;
    }
}

inline void CheckOpenGLError(const char* stmt, const char* fname, int line)
{
    GLenum err = glGetError();
    if (err == GL_NO_ERROR) {
        return;
    }
    std::cout << "OpenGL error " << std::hex << err << std::dec << " at " << fname << ":" << line << " - " << stmt << std::endl;
}

} // namespace gldebug

#ifdef _DEBUG
#define GL_CHECK(stmt) { \
	gldebug::ClearOpenGLError(); \
    stmt; \
    gldebug::CheckOpenGLError(#stmt, __FILE__, __LINE__); \
}
#else
#define GL_CHECK(stmt) stmt
#endif