#include <GL/glew.h>

#include "include/dearImGui.hpp"
#include <iostream>

void DearImGui::init(Window &window, const ImGuiColor color)
{
	m_window = &window;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	if (color == ImGuiColor::dark)
		ImGui::StyleColorsDark();
	else if (color == ImGuiColor::light)
		ImGui::StyleColorsLight();
	else
		ImGui::StyleColorsClassic();

	ImGui_ImplGlfw_InitForOpenGL(m_window->window(), true);
	ImGui_ImplOpenGL3_Init(reinterpret_cast<char *>(const_cast<unsigned char *>(glGetString(330))));
}

void DearImGui::terminate()
{
	ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void DearImGui::onUpdate(const MouseEvent &mouse_events)
{
	ImGuiIO &io = ImGui::GetIO();

	auto[width, height] = m_window->dimensions();
	io.DisplaySize = ImVec2(width / 3, height / 3);

	float time = static_cast<float>(glfwGetTime());
	io.DeltaTime = m_time > 0.0f ? (time - m_time) : (1.0f / 60.0f);
	m_time = time;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	begin("Debugging");

	// if (mouse_events.isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
	// 	outputText("Left mouse button pressed!");
	// if (mouse_events.isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
	// 	outputText("Right mouse button pressed!");

	mousePos(mouse_events); // TODO Say the x & y coords of the mouse
	mouseIndex(mouse_events);
	FPS();

	end();

	begin("Versions");
	stats();
	end();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());	
}

void DearImGui::begin(const char * const name)
{
	ImGui::Begin(name);
}

void DearImGui::end()
{
	ImGui::End();
}

void DearImGui::stats()
{
	ImGui::Text("OpenGL: %s\nGLSL: %s\nGPU: %s\nRenderer: %s", 
				glGetString(GL_VERSION), 
				reinterpret_cast<char *>(const_cast<GLubyte *>(glGetString(GL_SHADING_LANGUAGE_VERSION))),
				glGetString(GL_VENDOR),
				glGetString(GL_RENDERER));
}

void DearImGui::mousePos(const MouseEvent &mouse_events)
{
	auto[x, y] = mouse_events.getMousePos();
	outputText("X: %.3lf, Y: %.3lf", x, y);	
}

void DearImGui::mouseIndex(const MouseEvent &mouse_events)
{
	ImGui::Text("Index of mouse: %d", mouse_events.indexOfMousePos());
}

void DearImGui::FPS()
{
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}

bool DearImGui::button(const char * const description)
{
	return ImGui::Button(description);
}

bool DearImGui::checkbox(const char * const description)
{
	bool value;
	ImGui::Checkbox(description, &value);
	return value;
}
