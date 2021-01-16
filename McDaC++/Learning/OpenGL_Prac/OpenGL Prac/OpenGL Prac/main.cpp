#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

int main()
{
    // Width and height of window  -  (variables not needed but recommended for demo)
    const GLint WIDTH = 800, HEIGHT = 600;
    
    // Initialize glfw library
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW library" << std::endl;
        return -1;
    }
    
    // Set properties for window  -  !!! REQUIRED FOR MAC/CROSS PLATFORM !!!
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    // Create the window
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "My Window", nullptr, nullptr);
    if(!window)
    {
        glfwTerminate();
        std::cout << "Failed to create GLFW window" << std::endl;
        return -1;
    }
    
    // Get window size relevant to screen  -  !!! REQUIRED FOR MAC/CROSS PLATFORM !!!
    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    
    // Make the window's context current
    glfwMakeContextCurrent(window);
    
    // Enable access to modern OpenGL features
    glewExperimental = GL_TRUE;
    if(GLEW_OK != glewInit())
    {
        std::cout << "Failed initialization of glew" << std::endl;
        return -1;
    }
    
    // Adjusts window for high density screen  -  !!! REQUIRED FOR MAC/CROSS PLATFORM !!!
    glViewport(0, 0, screenWidth, screenHeight);
    
    // Loop until user closes the window
    while(!glfwWindowShouldClose(window))
    {
        // Poll for and process events
        glfwPollEvents();
        
        // Render window
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Swap front and back buffers
        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    
    return 0;
}
