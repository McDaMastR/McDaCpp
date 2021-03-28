#include "include/events.hpp"

Event::Event(GLFWwindow * const window)
	: m_window(window) {}


MouseEvent::MouseEvent(GLFWwindow * const window)
	: Event(window), mouse_pressed_state(false)
{
	glfwSetCursorPosCallback(m_window, cursorPositionCallback);
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetCursorEnterCallback(m_window, cursorEnterCallback);
	glfwSetMouseButtonCallback(m_window, mouseButtonCallback);
	glfwSetInputMode(m_window, GLFW_STICKY_MOUSE_BUTTONS, GL_TRUE);
	glfwSetScrollCallback(m_window, mouseScrollCallback);
}

bool MouseEvent::isMouseButtonPressed(const int key)
{
	uint16_t state = glfwGetMouseButton(m_window, key);
	if (mouse_pressed_state) {
		if (!(state == GLFW_PRESS))
			mouse_pressed_state = false;
		return false;
	} if (state == GLFW_PRESS) {
		mouse_pressed_state = true;
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

uint16_t MouseEvent::indexOfMousePos()
{

}

void MouseEvent::cursorPositionCallback(GLFWwindow *window, double x, double y)
{
}

void MouseEvent::cursorEnterCallback(GLFWwindow *window, int32_t entered)
{
}

void MouseEvent::mouseButtonCallback(GLFWwindow *window, int32_t button, int32_t action, int32_t mods)
{
}

void MouseEvent::mouseScrollCallback(GLFWwindow *window, double x_offset, double y_offset)
{
}
