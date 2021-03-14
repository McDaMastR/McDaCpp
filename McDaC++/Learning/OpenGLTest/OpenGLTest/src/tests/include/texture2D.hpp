#pragma once

#include "test.hpp"

#include "../../include/vertexBuffer.hpp"
#include "../../include/vertexBufferLayout.hpp"
#include "../../include/texture.hpp"
#include "../../include/shader.hpp"

#include "../../vendor/glm/glm.hpp"
#include "../../vendor/glm/gtc/matrix_transform.hpp"

namespace test
{
    class Texture2D : public Test
    {
    public:
        Texture2D();
        ~Texture2D();

        void onUpdate(const float delta_time) override;
        void onRender() override;
        void onImGuiRender() override;

    private:
        VertexArray *VAO;
        VertexBuffer *VBO;
        IndexBuffer *IBO;
        Shader *shader;
        Texture *texture;

        glm::mat4 proj, view;
        glm::vec3 translationA, translationB;
    };
}
