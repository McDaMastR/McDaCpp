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
	bool m_mousePressedState;

public:
	explicit MouseEvent(GLFWwindow * const window);
	~MouseEvent() = default;

	bool isMouseButtonPressed(const std::int32_t key);
	std::pair<double, double> getMousePos() const;
	std::uint8_t indexOfMousePos() const;

	static void cursorPositionCallback(GLFWwindow * const /* window */, double /* x */, double /* y */);
	static void cursorEnterCallback	  (GLFWwindow * const /* window */, std::int32_t /* entered */);
	static void mouseButtonCallback	  (GLFWwindow * const /* window */, std::int32_t /* button */, std::int32_t /* action */, std::int32_t /* mods */);
	static void mouseScrollCallback	  (GLFWwindow * const /* window */, double /* x_offset */, double /* y_offset */);

};
