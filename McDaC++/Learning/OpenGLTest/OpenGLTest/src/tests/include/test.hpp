#pragma once

#include "../../vendor/imGui/imgui.h"

#include <iostream>
#include <string>
#include <vector>
#include <functional>

namespace test
{
    class Test
    {
    public:
        Test() = default;
        virtual ~Test() = default;

        virtual void onUpdate(const float delta_time) {}
        virtual void onRender() {}
        virtual void onImGuiRender() {ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);}
    };

    class Menu : public Test
    {
    private:
        Test *& current_test;
        std::vector<std::pair<std::string, std::function<Test *()>>> tests;

    public:
        Menu(Test *& test_ptr);
        ~Menu() = default;
    
        void onImGuiRender() override;

		void changeCurrentTest(const std::string &name);

        template <class T>
        void registerTest(const std::string &name)
        {
            std::cout << name << " test has been registered\n";
            tests.push_back(std::make_pair(name, []{return new T;}));
        }
    };
}
