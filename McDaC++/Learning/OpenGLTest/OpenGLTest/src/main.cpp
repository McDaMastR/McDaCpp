#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <utility>
#include <string>
#include <sstream>

#include "include/renderer.hpp"
#include "include/vertexBuffer.hpp"
#include "include/vertexBufferLayout.hpp"
#include "include/indexBuffer.hpp"
#include "include/vertexArray.hpp"
#include "include/shader.hpp"
#include "include/texture.hpp"

#include "tests/include/test.hpp"
#include "tests/include/clearColor.hpp"
#include "tests/include/texture2D.hpp"

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/imGui/imgui.h"
#include "vendor/imGui/imgui_impl_glfw.h"
#include "vendor/imGui/imgui_impl_opengl3.h"

/*	How to compile:	

time clang++ -std=gnu++2a -g -Og -D DEBUG \
-D GL_SILENCE_DEPRECATION -D GLEW_STATIC -lglfw.3.3 -lglew.2.2.0 -framework OpenGL \
-Wall -Wextra -Wpedantic -Wshadow -Wno-deprecated-volatile \
src/vendor/stb_image/stb_image.cpp src/vendor/imGui/imgui_demo.cpp src/vendor/imGui/imgui_draw.cpp \
src/vendor/imGui/imgui_tables.cpp src/vendor/imGui/imgui_widgets.cpp src/vendor/imGui/imgui.cpp \
src/vendor/imGui/imgui_impl_glfw.cpp src/vendor/imGui/imgui_impl_opengl3.cpp \
src/main.cpp src/renderer.cpp src/vertexBuffer.cpp src/indexBuffer.cpp src/vertexArray.cpp \
src/vertexBufferElement.cpp src/vertexBufferLayout.cpp src/shader.cpp src/texture.cpp \
src/tests/test.cpp src/tests/clearColor.cpp src/tests/texture2D.cpp \
-o myApp

*/

int main(const int argc, const char **argv)
{	
	GLFWwindow* window;

    if (!glfwInit())
	{
		std::cout << "Failed to initialize glfw\n";
        return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(960, 540, "McDaWindow", NULL, NULL); // 16:9 ratio
    if (!window)
    {
		std::cout << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	glewExperimental = GL_TRUE;
	if (glewInit())
	{
		std::cout << "Failed to initialize glew\n";
		return -1;
	}

	GL_CALL(std::cout << "GPU Version: " << glGetString(GL_VERSION) << "\nGLSL version: " 
	<< reinterpret_cast<char *>(const_cast<GLubyte *>(glGetString(GL_SHADING_LANGUAGE_VERSION))) << '\n');

	{
		GL_CALL(glEnable(GL_BLEND));
		GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); // ALPHA == r g b _A_

		const Renderer renderer;

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init((reinterpret_cast<char *>(const_cast<unsigned char *>(glGetString(330)))));

		test::Test *current_test;
		test::Menu *test_menu = new test::Menu(current_test);
		current_test = test_menu;

		test_menu->registerTest<test::ClearColor>("Clear Color");
		test_menu->registerTest<test::Texture2D>("Texture 2D");

		while (!glfwWindowShouldClose(window))
		{
			renderer.setClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			renderer.clear();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			if (current_test)
			{
				current_test->onUpdate(0.0f);
				current_test->onRender();

				ImGui::Begin("Debugging");
				if (current_test != test_menu && ImGui::Button("Back to Menu"))
				{
					delete current_test;
					current_test = test_menu;
				}
				current_test->onImGuiRender();
				ImGui::End();
			}
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
		if (current_test != test_menu)
			delete current_test;
		delete test_menu;
	}

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
}
