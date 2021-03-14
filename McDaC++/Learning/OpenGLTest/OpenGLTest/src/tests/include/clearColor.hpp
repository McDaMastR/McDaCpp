#pragma once

#include "test.hpp"

namespace test
{
    class ClearColor : public Test
    {
    public:
        ClearColor() = default;
        ~ClearColor() = default;

        void onUpdate(const float delta_time) override;
        void onRender() override;
        void onImGuiRender() override;
    
    private:
        float color[4] = {0.2f, 0.3f, 0.8f, 1.0f};
    };
}