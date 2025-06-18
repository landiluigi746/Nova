#include "Nova/Renderer/Renderer.hpp"
#include "Nova/Renderer/VertexArray.hpp"
#include "Nova/Misc/Logger.hpp"
#include "Nova/Misc/Assert.hpp"
#include "Nova/Core/Window.hpp"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Nova::Renderer
{
    VertexArray s_QuadVA;
    uint32_t s_ShaderProgram = 0;

    static constexpr const char* s_VertexShaderSource =
        "#version 330 core\n"
        "layout (location = 0) in vec2 aPos;\n"
        "uniform mat4 uProjection;\n"
        "uniform mat4 uTransform;\n"
        "uniform vec4 uColor;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = uProjection * uTransform * vec4(aPos, 0.0, 1.0);\n"
        "}\n";

    static constexpr const char* s_FragmentShaderSource = "#version 330 core\n"
                                                          "out vec4 FragColor;\n"
                                                          "uniform vec4 uColor;\n"
                                                          "void main()\n"
                                                          "{\n"
                                                          "    FragColor = uColor;\n"
                                                          "}\n";

    static bool s_Initialized = false;

    static bool CheckOpenGLError()
    {
        GLenum error;
        int count = 0;

        while ((error = glGetError()) != GL_NO_ERROR)
        {
            Logger::Error("OpenGL error: {}", error);
            ++count;
        }

        return count == 0;
    }

    void UpdateProjection(int width, int height)
    {
        glUseProgram(s_ShaderProgram);
        glm::mat4 projection = glm::ortho(0.0f, (float) width, (float) height, 0.0f, -1.0f, 1.0f);
        glUniformMatrix4fv(glGetUniformLocation(s_ShaderProgram, "uProjection"), 1, GL_FALSE,
                           glm::value_ptr(projection));
        glUseProgram(0);

        glViewport(0, 0, width, height);

		NOVA_ASSERT(CheckOpenGLError(), "Failed to update projection!");
    }

    void Init()
    {
        if (s_Initialized)
            return;

        Logger::Info("Initializing Renderer...");

        // clang-format off
        float quadVertices[] = {
            -0.5f, -0.5f,
			 0.5f, -0.5f,
			 0.5f,  0.5f,
			-0.5f,  0.5f
        };

		uint32_t quadIndices[] = {
			0, 1, 2,
			2, 3, 0
		};
        // clang-format on

        s_QuadVA.Init();
        s_QuadVA.Bind();

        s_QuadVA.SetVertexBuffer(quadVertices, sizeof(quadVertices), {
			{ShaderDataType::Float2, false}
        });
		s_QuadVA.SetIndexBuffer(quadIndices, sizeof(quadIndices));

        uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
        uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(vertexShader, 1, &s_VertexShaderSource, nullptr);
        glShaderSource(fragmentShader, 1, &s_FragmentShaderSource, nullptr);

        glCompileShader(vertexShader);
        glCompileShader(fragmentShader);

        s_ShaderProgram = glCreateProgram();
        glAttachShader(s_ShaderProgram, vertexShader);
        glAttachShader(s_ShaderProgram, fragmentShader);
        glLinkProgram(s_ShaderProgram);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        UpdateProjection(Window::Get().GetWidth(), Window::Get().GetHeight());

        NOVA_ASSERT(CheckOpenGLError(), "Failed to initialize renderer!");

        s_Initialized = true;
        Logger::Info("Renderer initialized successfully!");
    }

    void Shutdown()
    {
        if (!s_Initialized)
            return;

        Logger::Info("Shutting down Renderer...");

        EndFrame();

		s_QuadVA.Shutdown();
        glDeleteProgram(s_ShaderProgram);

        NOVA_ASSERT(CheckOpenGLError(), "Failed to shutdown renderer!");

        s_Initialized = false;
        Logger::Info("Renderer shut down successfully!");
    }

    void BeginFrame()
    {}

    void EndFrame()
    {
        NOVA_ASSERT(CheckOpenGLError(), "Failed to end frame!");
    }

    void ClearScreen(const Color& color)
    {
        glClearColor((color.r / 255.0f), (color.g / 255.0f), (color.b / 255.0f), (color.a / 255.0f));
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Color& color, float rotation,
                  const glm::vec2& origin)
    {
		s_QuadVA.Bind();
		glUseProgram(s_ShaderProgram);

        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(position, 0.0f));
        transform = glm::translate(transform, glm::vec3(origin, 0.0f));
        transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::scale(transform, glm::vec3(size.x, size.y, 1.0f));
		transform = glm::translate(transform, glm::vec3(-origin, 0.0f));

        glUniformMatrix4fv(glGetUniformLocation(s_ShaderProgram, "uTransform"), 1, GL_FALSE, glm::value_ptr(transform));
        glUniform4f(glGetUniformLocation(s_ShaderProgram, "uColor"), color.r / 255.0f, color.g / 255.0f,
                    color.b / 255.0f, color.a / 255.0f);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		glUseProgram(0);
        s_QuadVA.Unbind();
    }
} // namespace Nova::Renderer
