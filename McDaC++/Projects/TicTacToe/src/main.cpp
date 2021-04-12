#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "include/GLclasses.hpp"

#include "include/texture.hpp"
#include "include/square.hpp"
#include "include/blender.hpp"
#include "include/events.hpp"
#include "include/window.hpp"

#ifdef DEBUG
#include "include/dearImGui.hpp"
#endif

#include "include/board.hpp"
#include "include/piece.hpp"

#ifdef DEBUG
#define DEBUG_LOG(x) std::cout << x << '\n'
#else
#define DEBUG_LOG(x) 
#endif

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

/*

X Y Plane:
	6 7 8
	3 4 5
	0 1 2

*/

int main()
{
	Window::initGlfw();
	Window window(640, 640); // Aspect ratio: 1 : 1
	Window::initGL();
	#ifdef DEBUG
	DearImGui::init(window, ImGuiColor::dark);
	#endif
	{
		const std::array<float, 16> board_verticies = {
			-1.0f, -1.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, 1.0f, 0.0f,
			 1.0f,  1.0f, 1.0f, 1.0f,
			-1.0f,  1.0f, 0.0f, 1.0f	
		}; 
		const std::array<std::uint32_t, 6> square_indices = {
			0, 1, 2,
			2, 3, 0
		};
		const Board board("res/textures/board.png", board_verticies, square_indices);
		Square blue_square;

		const Blender blender(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		Renderer renderer;

		const VertexArray vao;
		vao.bind();
		const Shader board_shader("res/shaders/board");
		const Shader background_shader("res/shaders/shadeBlue");
		vao.unBind();

		board_shader.bind();
		board_shader.setUniformVeci("u_Texture", 0);

		MouseEvent mouse_events(window.window());
		PieceType turn = PieceType::X;

		while (!window.shouldClose())
		{
			renderer.newFrame();

			board_shader.bind();
			board.render(renderer);
			Piece::renderAll(renderer);

			background_shader.bind();
			blue_square.changeVertices(mouse_events.indexOfMousePos());
			blue_square.render(renderer);

			if (mouse_events.isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
				// If cursor is over an X & O index and index is not already filled
				if (true) {
					Piece::addPiece(turn, {0, 1, 2, 2, 3, 0});
				}
				swapPieceType(turn);
			}

			if (Piece::check())
				break;

			#ifdef DEBUG
			DearImGui::onUpdate(mouse_events);
			#endif

			window.swapBuffers();
			window.pollEvents();
		}
		Piece::deletePieces();
	}
	window.terminate();
}
