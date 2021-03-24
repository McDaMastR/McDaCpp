#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

#include "include/renderer.hpp"
#include "include/vertexArray.hpp"
#include "include/indexBuffer.hpp"
#include "include/shader.hpp"
#include "include/vertexBuffer.hpp"
#include "include/vertexBufferLayout.hpp"
#include "include/texture.hpp"
#include "include/blender.hpp"
#include "include/events.hpp"
#include "include/window.hpp"

#ifdef DEBUG
#include "include/dearImGui.hpp"
#endif

#include "include/board.hpp"
#include "include/piece.hpp"

/*

	Board Indexes:

	6 	7 	8
	3 	4 	5
	0 	1 	2

*/

int main()
{
	Window::initGlfw();
	Window window(640, 640);
	Window::initGL();
	#ifdef DEBUG
	DearImGui::init(window, ImGuiColor::dark);
	#endif
	{
		const std::array<float, 16> board_verticies = {
			0.0f, 	0.0f, 	0.0f, 0.0f,
			640.0f, 0.0f, 	1.0f, 0.0f,
			640.0f, 640.0f, 1.0f, 1.0f,
			0.0f,	640.0f, 0.0f, 1.0f	
		}; const std::array<unsigned int, 6> board_indices = {
			0, 1, 2,
			2, 3, 0
		};
		const Board board("res/textures/board.png", board_verticies, board_indices);

		const Blender blender(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		const Renderer renderer;

		const Shader shader("res/shaders/basic.vert", "res/shaders/basic.frag");
		shader.bind();

		const glm::mat4 proj (glm::ortho(0.0f, 640.0f, 0.0f, 640.0f, -1.0f, 1.0f));
		const glm::mat4 view (glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)));
		const glm::mat4 model(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)));
		const glm::mat4 mvp = proj * view * model;

		shader.setUniformMatf("u_MVP", mvp);
		shader.setUniformVeci("u_Texture", 0);

		MouseEvent mouse_events(window.window());

		while (!window.shouldClose())
		{
			renderer.newFrame();

			board.render(renderer, shader);

			#ifdef DEBUG
			DearImGui::onUpdate(mouse_events);
			#endif

			window.swapBuffers();
			window.pollEvents();
		}
	}
	window.terminate();
}
