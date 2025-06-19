#include "Nova/Misc/GLError.hpp"
#include "Nova/Misc/Assert.hpp"

#include <glad/glad.h>

namespace Nova
{
    void CheckOpenGLErrors()
    {
        GLenum error;
        int errCount = 0;

        while ((error = glGetError()) != GL_NO_ERROR)
        {
            Logger::Error("OpenGL error: {}", error);
            ++errCount;
        }

        NOVA_ASSERT(errCount == 0, "{} OpenGL errors detected", errCount);
    }
} // namespace Nova
