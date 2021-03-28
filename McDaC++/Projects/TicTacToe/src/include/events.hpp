#pragma once

#include <GLFW/glfw3.h>

#include <utility>

class Event
{
public:
	explicit Event(GLFWwindow * const window);
	virtual ~Event() = default;

protected:
	GLFWwindow *m_window;
};

class MouseEvent : public Event
{
private:
	bool mouse_pressed_state;

public:
	explicit MouseEvent(GLFWwindow * const window);
	~MouseEvent() = default;

	bool isMouseButtonPressed(const int32_t key);
	std::pair<double, double> getMousePos() const;
	uint16_t indexOfMousePos();

	static void cursorPositionCallback(GLFWwindow *window, double x, double y);
	static void cursorEnterCallback(GLFWwindow *window, int32_t entered);
	static void mouseButtonCallback(GLFWwindow *window, int32_t button, int32_t action, int32_t mods);
	static void mouseScrollCallback(GLFWwindow *window, double x_offset, double y_offset);

};
