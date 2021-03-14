#include "include/clearColor.hpp"
#include "../include/renderer.hpp"
#include "../vendor/imGui/imgui.h"

#include <GL/glew.h>

namespace test
{
    void ClearColor::onUpdate(const float delta_time)
    {

    }

    void ClearColor::onRender()
    {
        GL_CALL(glClearColor(color[0], color[1], color[2], color[3]));
        GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
    }

    void ClearColor::onImGuiRender()
    {
        ImGui::ColorEdit4("Clear color", color);
    }
}