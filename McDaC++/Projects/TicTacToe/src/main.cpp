#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

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

#define DEBUG_LOG(x) std::cout << x++ << '\n'

/*

Vertex buffer:
	Pure data about graphical info (void *)

Index buffer:
	Placement of indices of triangle

Vertex array:
	Array of vertex buffer IDs

Layout:
	The structure of data in the vertex buffer

Shader:
	Program specifically for the GPU

Ok to have multiple objects of all above types

*/

int main()
{
	Window::initGlfw();
	Window window(640, 640); // Aspect ratio: 1 : 1
	Window::initGL();
	#ifdef DEBUG
	DearImGui::init(window, ImGuiColor::dark);
	uint16_t debug_num = 0;
	#endif
	{
		const std::array<float, 16> board_verticies = {
			-1.0f, -1.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, 1.0f, 0.0f,
			 1.0f,  1.0f, 1.0f, 1.0f,
			-1.0f,  1.0f, 0.0f, 1.0f	
		}; const std::array<uint32_t, 6> board_indices = {
			0, 1, 2,
			2, 3, 0
		};
		const Board board("res/textures/board.png", board_verticies, board_indices);

		const Blender blender(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		Renderer renderer;

		const Shader shader("res/shaders/basic");
		shader.bind();

		shader.setUniformVeci("u_Texture", 0);

		MouseEvent mouse_events(window.window());
		PieceType turn = PieceType::X;

		while (!window.shouldClose())
		{
			renderer.newFrame();

			board.render(renderer, shader);
			Piece::renderAll(renderer,shader);

			#ifdef DEBUG
			DearImGui::onUpdate(mouse_events);
			#endif

			switch (mouse_events.indexOfMousePos())
			{
			case 0:
				break;
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
			case 5:
				break;
			case 6:
				break;
			case 7:
				break;
			default:
				break;
			}

			if (mouse_events.isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
				// If cursor is over an X & O index and index is not already filled
				if (true) {
					Piece::addPiece(turn, {0, 1, 2, 2, 3, 0});
				}
				swapPiecetype(turn);
			}

			if (Piece::check())
				break;

			window.swapBuffers();
			window.pollEvents();
		}
		Piece::deletePieces();
	}
	window.terminate();
}
