#include "include/test.hpp"

#include <iostream>
#include "../vendor/imGui/imgui.h"

namespace test
{
    Menu::Menu(Test *& test_ptr)
        : current_test(test_ptr)
    {

    }

	void Menu::changeCurrentTest(const std::string &name)
	{
		for (auto &test : tests)
		{
			if (test.first == name)
			{
				current_test = test.second();
				break;
			}
		}
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
