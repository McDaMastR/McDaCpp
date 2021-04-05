#include "include/window.hpp"

#include "include/renderer.hpp"

#include <iostream>

Window::Window(const std::int32_t width, const std::int32_t height) noexcept
	: m_width(width), m_height(height)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	m_window = glfwCreateWindow(m_width, m_height, "TicTacToe", nullptr, nullptr);
    if (!m_window) {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
		assert(false);
    }

	glfwMakeContextCurrent(m_window);
}

GLFWwindow *Window::window() const noexcept
{
	return m_window;
}

std::pair<std::int32_t, std::int32_t> Window::dimensions() const
{
	return {m_width, m_height};
}

bool Window::shouldClose() const
{
	return glfwWindowShouldClose(m_window);
}

void Window::swapBuffers() const
{
	glfwSwapBuffers(m_window);
}

void Window::pollEvents() const
{
	glfwPollEvents();
}

void Window::terminate() const
{
	glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Window::printStats() noexcept
{
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) 
	<< "\nGLSL Version: " << reinterpret_cast<char *>(const_cast<GLubyte *>(glGetString(GL_SHADING_LANGUAGE_VERSION)))
	<< "\nGPU Version: " << glGetString(GL_VENDOR)
	<< "\nRenderer Version: " << glGetString(GL_RENDERER) << '\n';
}

void Window::initGlfw()
{
	#ifdef DEBUG
	if (!glfwInit()) {
		std::cerr << "Failed to initilize glfw\n";
		assert(false);
	}
	#else
	glfwInit();
	#endif
}

void Window::initGL()
{
	glewExperimental = GL_TRUE;
	#ifdef DEBUG
	if (glewInit()) {
		std::cerr << "Failed to initilize glew\n";
		assert(false);
	}
	#else
	glewInit();
	#endif
}
