#define DEBUG

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <utility>
#include <string>
#include <sstream>

#ifdef DEBUG
	#define ASSERT(x) 	if(!(x)) {\
						glfwTerminate();\
						return -1;}

	#define GL_CALL(x)	clearErrors();\
						x;\
						ASSERT(logCall(#x, __FILE__, __LINE__))
#else
	#define GL_CALL(x) x
#endif

/*	How to compile:	

time clang++ -std=gnu++2a \
-D GL_SILENCE_DEPRECATION -D GLEW_STATIC \
-lglfw.3.3 -lglew.2.2.0 -framework OpenGL \
-Wall -Wextra -Wpedantic \
-g -Og src/main.cpp -o myApp

*/

static void clearErrors()
{
	while (glGetError());
}

static bool logCall(const char *func, const char *file, const uint64_t line)
{
	uint32_t error;
	bool isError = false;
	while ((error = glGetError()))
	{
		std::cout << "OpenGL Error has occured\nError code: 0x" << std::hex << error << 
		std::dec << "\nFile: " << file << "\nLine: " << line << "\nFunction: " << func << '\n';
		isError = true;
	}
	if (isError)
		return false;
	return true;
}

static std::pair<std::string, std::string> parseShader(const std::string &filepath)
{
	enum class Mode
	{
		null = -1, vertex = 0, fragment = 1
	};

	std::ifstream file(filepath);
	std::string line;
	std::stringstream ss[2];
	Mode mode = Mode::null;

	while (getline(file, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				mode = Mode::vertex;
			else if (line.find("fragment") != std::string::npos)
				mode = Mode::fragment;
			continue;
		}
		ss[static_cast<int16_t>(mode)] << line << '\n';
	}
	return {ss[0].str(), ss[1].str()};
}

static uint32_t compileShader(const uint32_t type, const std::string &source)
{
	GL_CALL(const uint32_t id = glCreateShader(type));
	const char *src = source.c_str();
	GL_CALL(glShaderSource(id, 1, &src, nullptr));
	GL_CALL(glCompileShader(id));

	int32_t debug_result;
	GL_CALL(glGetShaderiv(id, GL_COMPILE_STATUS, &debug_result));
	if (!debug_result)
	{
		int32_t length;
		GL_CALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char *error_msg = static_cast<char *>(alloca(length * sizeof(char)));
		GL_CALL(glGetShaderInfoLog(id, length, &length, error_msg));

		std::cout << "Shader Error: " << error_msg << '\n';
		GL_CALL(glDeleteShader(id));
		return 0;
	}

	return id;
}

static uint32_t createShader(const std::string &vertex_shader, const std::string &fragment_shader)
{
	GL_CALL(uint32_t program = glCreateProgram());
	const uint32_t vs = compileShader(GL_VERTEX_SHADER, vertex_shader);
	const uint32_t fs = compileShader(GL_FRAGMENT_SHADER, fragment_shader);

	GL_CALL(glAttachShader(program, vs));
	GL_CALL(glAttachShader(program, fs));
	GL_CALL(glLinkProgram(program));
	GL_CALL(glValidateProgram(program));

	GL_CALL(glDeleteShader(vs));
	GL_CALL(glDeleteShader(fs));
	return program;
}

int main()
{	
	GLFWwindow* window;

    if (!glfwInit())
        return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(640, 480, "McDaWindow", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit())
		return -1;

	GL_CALL(std::cout << "GPU Version: " << glGetString(GL_VERSION) << "\nGLSL version: " 
	<< reinterpret_cast<char *>(const_cast<GLubyte *>(glGetString(GL_SHADING_LANGUAGE_VERSION))) << '\n');

	const float positions[] = {
		-0.5f, -0.5f, // Vertex 0
 		 0.5f, -0.5f, // Vertex 1
	 	 0.5f,  0.5f, // Vertex 2
		-0.5f,  0.5f, // Vertex 3
	};
	const uint32_t indices[] = {
		0, 1, 2, 
		2, 3, 0
	};

	uint32_t bufferID;
	GL_CALL(glGenBuffers(1, &bufferID));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, bufferID));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW));

	uint32_t ibo;
	GL_CALL(glGenBuffers(1, &ibo));
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

	GL_CALL(glEnableVertexAttribArray(0));
	GL_CALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));

	auto[vertex_shader, fragment_shader] = parseShader("../res/shaders/basic.shader");

	const uint32_t shader = createShader(vertex_shader, fragment_shader);
	GL_CALL(glUseProgram(shader));
 
    while (!glfwWindowShouldClose(window))
    {
        GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

		GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

	GL_CALL(glDeleteProgram(shader));
    glfwTerminate();
}
