#pragma once

#include "window.hpp"
#include "renderer.hpp"
#include "events.hpp"

#include "../vendor/imGui/imgui.h"
#include "../vendor/imGui/imgui_impl_glfw.h"
#include "../vendor/imGui/imgui_impl_opengl3.h"

enum class ImGuiColor
{
	light, dark, classic
};

namespace DearImGui
{
	inline Window *m_window;
	inline double m_time;

	void init(Window &window, const ImGuiColor color);
	void terminate();

	void onUpdate(const MouseEvent &mouse_events);

	void begin(const char * const name);
	void end();

	template <class ...T>
	inline void outputText(const char * const text, const T ... args)
	{
		ImGui::Text(text, args...);
	}

	void stats();
	void mousePos(const MouseEvent &mouse_events);
	void mouseIndex(const MouseEvent &mouse_events);
	void FPS();
	bool button(const char * const description);
	bool checkbox(const char * const description);
}
