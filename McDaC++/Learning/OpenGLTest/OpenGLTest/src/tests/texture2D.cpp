#include "include/texture2D.hpp"

#include "../include/renderer.hpp"
#include "../vendor/imGui/imgui.h"

#include <GL/glew.h>

namespace test
{
    Texture2D::Texture2D()
        : proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
        view(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
        translationA(200, 200, 0), translationB(400, 200, 0)
    {
        float positions[] = {
            -50.0f, -50.0f, 0.0f, 0.0f,
             50.0f, -50.0f, 1.0f, 0.0f,
             50.0f,  50.0f, 1.0f, 1.0f,
            -50.0f,  50.0f, 0.0f, 1.0f,
        };
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        GL_CALL(glEnable(GL_BLEND));
        GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        
        VAO = new VertexArray();

        VBO = new VertexBuffer(positions, sizeof(positions));
        VertexBufferLayout layout;
        layout.push<float>(2);
        layout.push<float>(2);
        VAO->addBuffer(*VBO, layout);

        IBO = new IndexBuffer(indices, 6);

        shader = new Shader("res/shaders/basic.shader");
        shader->bind();
        shader->setUniformf("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

        texture = new Texture("res/textures/grassBlock.jpeg");
        shader->setUniformi("u_Texture", 0);
    }

    Texture2D::~Texture2D()
    {
        delete VAO;
        delete VBO;
        delete IBO;
        delete shader;
        delete texture;
    }

    void Texture2D::onUpdate(const float delta_time)
    {

    }

    void Texture2D::onRender()
    {
        GL_CALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;
        texture->bind();

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
            glm::mat4 mvp = proj * view * model;
            shader->bind();
            shader->setUniformMatf("u_MVP", mvp);
            renderer.draw(*VAO, *IBO, *shader);
        }
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
            glm::mat4 mvp = proj * view * model;
            shader->bind();
            shader->setUniformMatf("u_MVP", mvp);
            renderer.draw(*VAO, *IBO, *shader);
        }
    }

    void Texture2D::onImGuiRender()
    {
        ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 960.0f);
        ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 960.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
}
