#include "include/events.hpp"
#include <iostream>

Event::Event(GLFWwindow * const window)
	: m_window(window) {}


MouseEvent::MouseEvent(GLFWwindow * const window)
	: Event(window), m_mousePressedState(false)
{
	glfwSetCursorPosCallback(m_window, cursorPositionCallback);
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetCursorEnterCallback(m_window, cursorEnterCallback);
	glfwSetMouseButtonCallback(m_window, mouseButtonCallback);
	glfwSetInputMode(m_window, GLFW_STICKY_MOUSE_BUTTONS, GL_TRUE);
	glfwSetScrollCallback(m_window, mouseScrollCallback);
}

bool MouseEvent::isMouseButtonPressed(const std::int32_t key)
{
	std::uint16_t state = glfwGetMouseButton(m_window, key);
	if (m_mousePressedState) {
		if (!(state == GLFW_PRESS))
			m_mousePressedState = false;
		return false;
	} if (state == GLFW_PRESS) {
		m_mousePressedState = true;
		return true;
	}
	return false;
}

std::pair<double, double> MouseEvent::getMousePos() const
{
	double x, y;
	glfwGetCursorPos(m_window, &x, &y);
	return {x, y};
}

std::uint8_t MouseEvent::indexOfMousePos() const
{
	auto[pos_x, pos_y] = getMousePos();
	float index_y = pos_y / (640.0f / 3.0f);
	return 3 * (index_y == 1 ? index_y : (index_y == 0 ? 2 : 0)) + pos_x / (640.0f / 3.0f);
}

void MouseEvent::cursorPositionCallback(GLFWwindow * const /* window */, double /* x */, double /* y */)
{
}

void MouseEvent::cursorEnterCallback(GLFWwindow * const /* window */, std::int32_t /* entered */)
{
}

void MouseEvent::mouseButtonCallback(GLFWwindow * const /* window */, std::int32_t /* button */, std::int32_t /* action */, std::int32_t /* mods */)
{
}

void MouseEvent::mouseScrollCallback(GLFWwindow * const /* window */, double /* x_offset */, double /* y_offset */)
{
}
