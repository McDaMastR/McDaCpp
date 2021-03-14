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

#include "include/maze.hpp"

int main()
{
    GLFWwindow* window;

    if (!glfwInit()) {
        std::cout << "Failed to initilize glfw\n";
        return -1;
    }

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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
        std::cout << "Failed to initilize GLEW\n";
        return -1;
    }

    std::cout << "OPENGL Version: " << glGetString(GL_VERSION) << "\nGLSL Version: "
    << reinterpret_cast<char *>(const_cast<GLubyte *>(glGetString(GL_SHADING_LANGUAGE_VERSION))) << '\n';

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
}
