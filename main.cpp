#include <iostream>
#include <glad/glad.h>  // Make OpenGL function pointers available..
#include <GLFW/glfw3.h> // GLFW3 is an multi-platform library for OpenGL

#define WIDTH 800
#define HEIGHT 600

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

int main(int, char**){
    std::cout << "Hello, from ogl-hello-triangle!\n";

    glfwInit(); // Initializes the GLFW-Context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  // Specifies that Version '3'.3 is used
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  // Specifies that Version 3.'3' is used
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // Specifies what profile is used

    GLFWwindow *window = glfwCreateWindow(800, 600, "Hello!", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create a GLFW window!";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // Gives the context the created window

    /* Glad is used to give access to OGL functions.
        glad has to be initialized before any use of OGL functions!
        */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD!";
        return -1;
    }

    glViewport(0, 0, 800, 600); // Creates a OGL viewport :) It is a area where the programm can draw sick graphics ;)
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  // Registers function as callback to set new glViewport size after a resize event occurs

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear buffers
        glClearColor(.3f, .2f, .3f, 1.0f);  // Sets Color the buffer buffer should be cleared
        glClear(GL_COLOR_BUFFER_BIT);   // Clear color bits only


        // Input
        processInput(window);


        // Rendering commands
        // TODO: Hello Triangle; Chapter 5; Page 26

        
        // Check and call events and swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
