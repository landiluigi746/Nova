#include "Nova/Renderer/Renderer.hpp"
#include "Nova/Renderer/VertexArray.hpp"
#include "Nova/Renderer/Shader.hpp"
#include "Nova/Renderer/Texture.hpp"
#include "Nova/Renderer/GLError.hpp"

#include "Nova/Misc/Logger.hpp"
#include "Nova/Misc/Assert.hpp"
#include "Nova/Misc/Metrics.hpp"

#include "Nova/Core/Window.hpp"

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <array>
#include <algorithm>

namespace Nova::Renderer
{
    static constexpr size_t MAX_QUADS = 16000;
    static constexpr size_t MAX_VERTICES = MAX_QUADS * 4;
    static constexpr size_t MAX_INDICES = MAX_QUADS * 6;
    static constexpr size_t MAX_TEXTURE_SLOTS = 16;

    struct VertexData
    {
        glm::vec2 Position;
        glm::vec2 TexCoords;
        glm::vec4 Color;
        float TexIndex;
    };

    struct RendererData
    {
        uint32_t QuadIndicesCount = 0;
        std::shared_ptr<Texture> QuadTexture = std::make_shared<Texture>(); // just a white 1x1 texture
        std::vector<VertexData> QuadVertices;
        std::vector<std::shared_ptr<Texture>> QuadTextures;
        VertexArray QuadVA;
        Shader QuadShader;
    };

    static RendererData s_Data;

    // clang-format off
    static constexpr const char* s_VertexShaderSource =
        "#version 330 core\n"
        "layout (location = 0) in vec2 aPos;\n"
        "layout (location = 1) in vec2 aTexCoords;\n"
        "layout (location = 2) in vec4 aColor;\n"
        "layout (location = 3) in float aTexIndex;\n"
        "out vec2 TexCoords;\n"
        "out vec4 Color;\n"
        "out float TexIndex;\n"
        "uniform mat4 uProjection;\n"
        "void main()\n"
        "{\n"
        "    TexCoords = aTexCoords;\n"
        "    Color = aColor;\n"
        "    TexIndex = aTexIndex;\n"
        "    gl_Position = uProjection * vec4(aPos, 0.0, 1.0);\n"
        "}\n";

    static constexpr const char* s_FragmentShaderSource = 
        "#version 330 core\n"
        "in vec2 TexCoords;\n"
        "in vec4 Color;\n"
        "in float TexIndex;\n"
        "out vec4 FragColor;\n"
        "uniform sampler2D uTextures[16];\n"
        "void main()\n"
        "{\n"
        "    int texIdx = int(TexIndex);\n"
        "    vec4 color = Color;\n"
        "    switch (texIdx)\n"
        "    {\n"
        "        case 0: color *= texture(uTextures[0], TexCoords); break;\n"
        "        case 1: color *= texture(uTextures[1], TexCoords); break;\n"
        "        case 2: color *= texture(uTextures[2], TexCoords); break;\n"
        "        case 3: color *= texture(uTextures[3], TexCoords); break;\n"
        "        case 4: color *= texture(uTextures[4], TexCoords); break;\n"
        "        case 5: color *= texture(uTextures[5], TexCoords); break;\n"
        "        case 6: color *= texture(uTextures[6], TexCoords); break;\n"
        "        case 7: color *= texture(uTextures[7], TexCoords); break;\n"
        "        case 8: color *= texture(uTextures[8], TexCoords); break;\n"
        "        case 9: color *= texture(uTextures[9], TexCoords); break;\n"
        "        case 10: color *= texture(uTextures[10], TexCoords); break;\n"
        "        case 11: color *= texture(uTextures[11], TexCoords); break;\n"
        "        case 12: color *= texture(uTextures[12], TexCoords); break;\n"
		"        case 13: color *= texture(uTextures[13], TexCoords); break;\n"
		"        case 14: color *= texture(uTextures[14], TexCoords); break;\n"
		"        case 15: color *= texture(uTextures[15], TexCoords); break;\n"
		"    }\n"
        "    if (color.a == 0.0) discard;\n"
        "    FragColor = color;\n"
        "}\n";

    static constexpr glm::vec2 s_QuadVertexPos[4] = {
        { -0.5f, -0.5f },
        {  0.5f, -0.5f },
        {  0.5f,  0.5f },
        { -0.5f,  0.5f },
    };

    static constexpr glm::vec2 s_QuadTexCoords[4] = {
        { 0.0f, 0.0f },
        { 1.0f, 0.0f },
        { 1.0f, 1.0f },
        { 0.0f, 1.0f },
    };
    // clang-format on

    static bool s_Initialized = false;

    void UpdateProjection(int width, int height)
    {
        glm::mat4 projection = glm::ortho(0.0f, (float) width, (float) height, 0.0f, -1.0f, 1.0f);

        s_Data.QuadShader.Bind();
        s_Data.QuadShader.SetUniformMat4("uProjection", projection);
        s_Data.QuadShader.Unbind();

        glViewport(0, 0, width, height);

        CheckOpenGLErrors();
    }

    void EnableMultisampling()
    {
        glEnable(GL_MULTISAMPLE);
    }

    void DisableMultisampling()
    {
        glDisable(GL_MULTISAMPLE);
    }

    void Init(int width, int height)
    {
        if (s_Initialized)
            return;

        Logger::Info("Initializing Renderer...");

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        s_Data.QuadVertices.reserve(MAX_VERTICES);
        s_Data.QuadTextures.reserve(MAX_TEXTURE_SLOTS);

        s_Data.QuadVA.Init();
        s_Data.QuadVA.Bind();

        s_Data.QuadVA.InitVertexBuffer(nullptr, s_Data.QuadVertices.capacity() * sizeof(VertexData),
                                       {{ShaderDataType::Float2, false},
                                        {ShaderDataType::Float2, false},
                                        {ShaderDataType::Float4, false},
                                        {ShaderDataType::Float, false}});

        std::vector<uint32_t> quadIndices(MAX_INDICES);

        uint32_t offset = 0;
        for (uint32_t i = 0; i < MAX_INDICES; i += 6, offset += 4)
        {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;
            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;
        }

        s_Data.QuadVA.InitIndexBuffer(quadIndices.data(), MAX_INDICES);

        Logger::Info("Initializing default shader...");
        s_Data.QuadShader.Init(s_VertexShaderSource, s_FragmentShaderSource);

        std::array<int32_t, MAX_TEXTURE_SLOTS> texSlots;
        for (uint32_t i = 0; i < MAX_TEXTURE_SLOTS; ++i)
            texSlots[i] = i;

        s_Data.QuadShader.Bind();
        s_Data.QuadShader.SetUniformIntV("uTextures", texSlots.data(), MAX_TEXTURE_SLOTS);
        s_Data.QuadShader.Unbind();
        Logger::Info("Default shader initialized successfully!");

        Logger::Info("Initializing default texture...");
        s_Data.QuadTexture->Init(1, 1, &Nova::White);
        Logger::Info("Default texture initialized successfully!");

        s_Data.QuadTextures.emplace_back(s_Data.QuadTexture);
        s_Data.QuadVA.Unbind();

        UpdateProjection(width, height);

        CheckOpenGLErrors();

        s_Initialized = true;
        Logger::Info("Renderer initialized successfully!");
    }

    void Shutdown()
    {
        if (!s_Initialized)
            return;

        Logger::Info("Shutting down Renderer...");

        EndFrame();

        s_Data.QuadVA.Shutdown();
        s_Data.QuadShader.Shutdown();
        s_Data.QuadTexture->Shutdown();
        s_Data.QuadVertices.clear();
        s_Data.QuadTextures.clear();

        CheckOpenGLErrors();

        s_Initialized = false;
        Logger::Info("Renderer shut down successfully!");
    }

    static void ClearQuadBatch()
    {
        s_Data.QuadVertices.clear();
        s_Data.QuadTextures.resize(1);
        s_Data.QuadIndicesCount = 0;
    }

    static void SendQuadBatch()
    {
        if (s_Data.QuadIndicesCount == 0)
            return;

        s_Data.QuadVA.Bind();
        s_Data.QuadShader.Bind();

        for (size_t i = 0; i < s_Data.QuadTextures.size(); ++i)
            s_Data.QuadTextures[i]->Bind(i);

        s_Data.QuadVA.SetVertexBufferData(s_Data.QuadVertices.data(), s_Data.QuadVertices.size() * sizeof(VertexData));
        glDrawElements(GL_TRIANGLES, s_Data.QuadIndicesCount, GL_UNSIGNED_INT, nullptr);

        s_Data.QuadShader.Unbind();
        s_Data.QuadVA.Unbind();

        CheckOpenGLErrors();

        Metrics::IncrementDrawnObjects(s_Data.QuadIndicesCount / 6);
        Metrics::IncrementDrawCalls();

        ClearQuadBatch();
    }

    void BeginFrame() {}

    void EndFrame()
    {
        SendQuadBatch();
        CheckOpenGLErrors();
    }

    void ClearScreen(const Color& color)
    {
        glClearColor((color.r / 255.0f), (color.g / 255.0f), (color.b / 255.0f), (color.a / 255.0f));
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Color& color, float rotation,
                  const glm::vec2& origin)
    {
        DrawQuad(s_Data.QuadTexture, position, size, color, rotation, origin,
                 {0.0f, 0.0f, s_Data.QuadTexture->GetWidth(), s_Data.QuadTexture->GetHeight()});
    }

    void DrawQuad(std::shared_ptr<Texture> texture, const glm::vec2& position, float rotation, const glm::vec2& origin)
    {
        DrawQuad(texture, position, {texture->GetWidth(), texture->GetHeight()}, White, rotation, origin,
                 {0.0f, 0.0f, texture->GetWidth(), texture->GetHeight()});
    }

    void DrawQuad(std::shared_ptr<Texture> texture, const glm::vec2& position, float rotation, const glm::vec2& origin,
                  const glm::vec4& sourceRect)
    {
        glm::vec4 src = sourceRect;

        if (src.z < 0.0f)
            src.z *= -1.0f;

        if (src.w < 0.0f)
            src.w *= -1.0f;

        DrawQuad(texture, position, {src.z, src.w}, White, rotation, origin, src);
    }

    void DrawSprite(const Sprite& sprite, const glm::vec2& position, float rotation,
                    const glm::vec2& origin)
    {
        DrawSprite(sprite, position, sprite.GetFrameSize(), rotation, origin);
    }

    void DrawSprite(const Sprite& sprite, const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec2& origin)
    {
        DrawQuad(sprite.GetTexture(), position, size, White, rotation, origin,
                 glm::vec4(sprite.GetFramePosition(), sprite.GetFrameSize()));
    }

    void DrawQuad(std::shared_ptr<Texture> texture, const glm::vec2& position, const glm::vec2& size,
                  const Color& color, float rotation, const glm::vec2& origin, const glm::vec4& sourceRect)
    {
        if (!texture)
            texture = s_Data.QuadTexture;

        if (s_Data.QuadVertices.size() > MAX_VERTICES - 4)
            SendQuadBatch();

        float texIndex = 0.0f;
        if (texture != s_Data.QuadTexture)
        {
            auto it =
                std::find_if(std::begin(s_Data.QuadTextures), std::end(s_Data.QuadTextures), [&](const auto& tex) {
                    return tex->GetID() == texture->GetID();
                });

            if (it != s_Data.QuadTextures.end())
                texIndex = static_cast<float>(std::distance(s_Data.QuadTextures.begin(), it));
            else
            {
                if (s_Data.QuadTextures.size() >= MAX_TEXTURE_SLOTS)
                    SendQuadBatch();

                s_Data.QuadTextures.emplace_back(texture);
                texIndex = static_cast<float>(s_Data.QuadTextures.size() - 1);
            }
        }

        glm::vec2 texSize = {(float) texture->GetWidth(), (float) texture->GetHeight()};
        glm::vec2 uvMin = {0.0f, 0.0f};
        glm::vec2 uvMax = {1.0f, 1.0f};

        if (sourceRect.z > 0 && sourceRect.w > 0)
        {
            uvMin = {sourceRect.x / texSize.x, sourceRect.y / texSize.y};
            uvMax = {(sourceRect.x + sourceRect.z) / texSize.x, (sourceRect.y + sourceRect.w) / texSize.y};
        }

        glm::vec2 texCoords[4] = {
            {uvMin.x, uvMin.y},
            {uvMax.x, uvMin.y},
            {uvMax.x, uvMax.y},
            {uvMin.x, uvMax.y},
        };

        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(position, 0.0f));
        transform = glm::translate(transform, glm::vec3(origin, 0.0f));
        transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::scale(transform, glm::vec3(size, 1.0f));
        transform = glm::translate(transform, glm::vec3(-origin, 0.0f));

        glm::vec4 normalizedColor = glm::vec4(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f);

        for (int i = 0; i < 4; ++i)
        {
            glm::vec4 pos = transform * glm::vec4(s_QuadVertexPos[i], 0.0f, 1.0f);

            s_Data.QuadVertices.emplace_back(VertexData{.Position = glm::vec2{pos.x, pos.y},
                                                        .TexCoords = texCoords[i],
                                                        .Color = normalizedColor,
                                                        .TexIndex = texIndex});
        }

        s_Data.QuadIndicesCount += 6;
    }
} // namespace Nova::Renderer
