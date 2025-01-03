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

/** Initializes a window and passes it to an OpenGL context */
void initialization(GLFWwindow *window)
{
    
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_VERSION_MINOR, 3);
    
    GLFWwindow *window = glfwCreateWindow(800, 600, "Exercise 1 - 2 Triangles", 0, NULL);
    if (!window)
    {
        std::cout << "Failed to create a window!";
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);

    // Even if glfwGetProcAddress returns a string, the type cast "GLADloadproc" has to be there.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "failed to get the address of the context!";
        glfwTerminate();
    }


    float vertecis[] = {
        0.25f, 0.25f, 0.0f,
        0.25f, -0.25f, 0.0f,
        -0.25f, -0.25f, 0.0f, // Left Triangle

        0.25f, 0.30f, 0.0f,
        -0.25f, 0.30f, 0.0f,
        -0.25f, -0.20f, 0.0f    // Right Trinagle
    };

    unsigned int VAO, VBO, EBO;
    unsigned int shaderProgram, vertexShader, fragmentShader;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertecis), vertecis, GL_STATIC_DRAW);

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Error check .. :)

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Error check .. :)

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);


    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.5f, 0.2f, 0.4f, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}