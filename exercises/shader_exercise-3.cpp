// Like my thesis, because the 2 negative values of the point arent reaching into the positives.
// Additionally the "z"-value is 0, so no valid color can be rendered. 

#include "../shaders/shaderclass.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main()
{
    // X Y Z & R G B
    float vertecis[] = {
        0.0f, 0.25f, 0.0f, 1.0f, 0.2f, 0.4f,
        -0.5f, -0.25f, 0.0f, 0.2f, 0.4f, 0.2f,
        0.5f, -0.25f, 0.0f, 0.4f, 0.1f, 1.0f,
    };

    glfwInit();
    glfwWindowHint(GLFW_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_VERSION_MINOR, 3);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Shader exercise 2 - Positional offset", NULL, 0);
    if (!window)
    {
        std::cout << "Window could not be created!";
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Es wurde kein Kontext gefunden!";
        glfwTerminate();
        return 1;
    }

    unsigned int vao, vbo;
    int result;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertecis), vertecis, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);

    Shader myShaders("../shaders/vertex_exercise-3.vert", "../shaders/fragment_exercise-3.frag");

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.4f, 0.2f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        myShaders.use();
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}