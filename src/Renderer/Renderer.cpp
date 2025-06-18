#include "Nova/Renderer/Renderer.hpp"
#include "Nova/Renderer/VertexArray.hpp"
#include "Nova/Renderer/Shader.hpp"
#include "Nova/Renderer/Texture.hpp"

#include "Nova/Misc/Logger.hpp"
#include "Nova/Misc/Assert.hpp"
#include "Nova/Core/Window.hpp"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Nova::Renderer
{
    static VertexArray s_QuadVA;
    static Shader s_DefaultShader;
    static Texture s_DefaultTexture; // just a white 1x1 texture

    // clang-format off
    static constexpr const char* s_VertexShaderSource =
        "#version 330 core\n"
        "layout (location = 0) in vec2 aPos;\n"
        "layout (location = 1) in vec2 aTex;\n"
        "out vec2 TexCoords;\n"
        "uniform mat4 uProjection;\n"
        "uniform mat4 uTransform;\n"
        "void main()\n"
        "{\n"
        "    TexCoords = aTex;\n"
        "    gl_Position = uProjection * uTransform * vec4(aPos, 0.0, 1.0);\n"
        "}\n";

    static constexpr const char* s_FragmentShaderSource = 
        "#version 330 core\n"
        "in vec2 TexCoords;\n"
        "out vec4 FragColor;\n"
        "uniform vec4 uColor;\n"
        "uniform sampler2D uTexture;\n"
        "void main()\n"
        "{\n"
        "    FragColor = texture(uTexture, TexCoords) * uColor;\n"
        "}\n";
	// clang-format on

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
        glm::mat4 projection = glm::ortho(0.0f, (float) width, (float) height, 0.0f, -1.0f, 1.0f);

        s_DefaultShader.Bind();
        s_DefaultShader.SetUniformMat4("uProjection", projection);
        s_DefaultShader.Unbind();

        glViewport(0, 0, width, height);

        NOVA_ASSERT(CheckOpenGLError(), "Failed to update projection!");
    }

    void Init()
    {
        if (s_Initialized)
            return;

        Logger::Info("Initializing Renderer...");

        // clang-format off
        float quadVertexData[] = {
            // pos         // tex
            -0.5f, -0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.0f, 1.0f
        };

		uint32_t quadIndices[] = {
			0, 1, 2,
			2, 3, 0
		};
        // clang-format on

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        s_QuadVA.Init();
        s_QuadVA.Bind();

        s_QuadVA.SetVertexBuffer(quadVertexData, sizeof(quadVertexData),
                                 {{ShaderDataType::Float2, false}, {ShaderDataType::Float2, false}});
        s_QuadVA.SetIndexBuffer(quadIndices, sizeof(quadIndices));

        Logger::Info("Initializing default shader...");
        s_DefaultShader.Init(s_VertexShaderSource, s_FragmentShaderSource);
        Logger::Info("Default shader initialized successfully!");

        Logger::Info("Initializing default texture...");
        s_DefaultTexture.Init(1, 1, &Nova::White);
        Logger::Info("Default texture initialized successfully!");

        s_QuadVA.Unbind();

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
        s_DefaultShader.Shutdown();
        s_DefaultTexture.Shutdown();

        NOVA_ASSERT(CheckOpenGLError(), "Failed to shutdown renderer!");

        s_Initialized = false;
        Logger::Info("Renderer shut down successfully!");
    }

    void BeginFrame() {}

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
        DrawQuad(s_DefaultTexture, position, size, color, rotation, origin);
    }

    void DrawQuad(const Texture& texture, const glm::vec2& position, float rotation, const glm::vec2& origin)
    {
        DrawQuad(texture, position, {texture.GetWidth(), texture.GetHeight()}, White, rotation, origin);
    }

    void DrawQuad(const Texture& texture, const glm::vec2& position, const glm::vec2& size, const Color& color,
                  float rotation, const glm::vec2& origin)
    {
        s_QuadVA.Bind();
        s_DefaultShader.Bind();
        texture.Bind();

        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(position, 0.0f));
        transform = glm::translate(transform, glm::vec3(origin, 0.0f));
        transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::scale(transform, glm::vec3(size.x, size.y, 1.0f));
        transform = glm::translate(transform, glm::vec3(-origin, 0.0f));

        s_DefaultShader.SetUniformInt("uTexture", 0);
        s_DefaultShader.SetUniformMat4("uTransform", transform);
        s_DefaultShader.SetUniformFloat4("uColor",
                                         {color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f});

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        texture.Unbind();
        s_DefaultShader.Unbind();
        s_QuadVA.Unbind();
    }
} // namespace Nova::Renderer
