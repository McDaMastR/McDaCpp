#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <utility>

class Window
{
public:
	Window(const int32_t width, const int32_t height) noexcept;
	~Window() noexcept = default;

	GLFWwindow *window() const noexcept;
	std::pair<int32_t, int32_t> dimensions() const;
	bool shouldClose() const;

	void swapBuffers() const;
	void pollEvents() const;
	void terminate() const;

	static void printStats() noexcept;
	static void initGlfw();
	static void initGL();

private:
	GLFWwindow *m_window;
	const int32_t m_width, m_height;
};