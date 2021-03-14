#include "include/test.hpp"

#include <iostream>
#include "../vendor/imGui/imgui.h"

namespace test
{
    Menu::Menu(Test *& test_ptr)
        : current_test(test_ptr)
    {

    }

    void Menu::onImGuiRender()
    {
        for (auto &test : tests)
        {
            if (ImGui::Button(test.first.c_str()))
                current_test = test.second();
        }
    }
}
