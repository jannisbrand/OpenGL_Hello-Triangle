#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Temporary
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    " gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_VERSION_MINOR, 3);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Exercise 2 - 2 Triangles with different VAO & VBO & seperate data", 0, NULL);
    if (!window)
    {
        std::cout << "Failed to create window!";
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to get address of context!" << std::endl;
        glfwTerminate();
    }

    // The data! 🤓☝️
    float vertices_triangle_one[] = {
        -0.5f,  0.5f, 0.0f,
         0.0f,  0.0f, 0.0f,
        -0.5f, -0.0f, 0.0f
    };

    float vertices_triangle_two[] = {
        0.0f,  0.0f, 0.0f,
        0.5f, -0.0f, 0.0f,
        0.5f, -0.5f, 0.0f
    };

    unsigned int vertex, fragment, program;
    int result;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexShaderSource, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        std::cout << "Vertex shader? More like Vertex shader'nt";
        glfwSetWindowShouldClose(window, 1);
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragment);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        std::cout << "Fragment shader? More like fragment shader'nt";
        glfwSetWindowShouldClose(window, 1);
    }

    std::cout << "Programm";

    program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (!result)
    {
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    std::cout << "First Triangle";

    unsigned int vbo_one, vao_one, vbo_two, vao_two;

    // First triangle
    glGenVertexArrays(1, &vao_one);
    glGenBuffers(1, &vbo_one);

    glBindVertexArray(vao_one);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_one);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_triangle_one), vertices_triangle_one, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    std::cout << "Secound Triangle";

    // Secound triangle
    glGenVertexArrays(1, &vao_two);
    glGenBuffers(1, &vbo_two);

    glBindVertexArray(vao_two);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_two);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_triangle_two), vertices_triangle_two, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    std::cout << "Loop";

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.5f, 0.2f, 0.4f, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glBindVertexArray(vao_one);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        glBindVertexArray(vao_two);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}