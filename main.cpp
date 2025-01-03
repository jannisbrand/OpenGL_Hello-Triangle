#include <iostream>
#include <glad/glad.h>  // Make OpenGL function pointers available..
#include <GLFW/glfw3.h> // GLFW3 is an multi-platform library for OpenGL

#define WIDTH 800
#define HEIGHT 600

float vertices[] = {    // vertices -> Eckpunkte (The coordinates are specified in NDC (Normalized Device Coordinates))
        -0.5f, -0.5f, 0.0f,     // Vertex 1: X=-0.5, Y=-0.5, Z=0.0
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f 
    };

float verticesRect[] = {    // vertices -> Eckpunkte (The coordinates are specified in NDC (Normalized Device Coordinates))
         0.5f,  0.5f, 0.0f,     // Top Right
         0.5f, -0.5f, 0.0f,      // Bottom Right
        -0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };

unsigned int indicesRect[] = {  // Contains indices that tell the EBO wich of the triangles vertices should be used to form a rectangle
    0, 1, 3,    // 1st Triangle
    1, 2, 3     // 2nd Triangle
    };

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

    //Creation and binding of vertex array object. (VAO) After Binding VBO configs are stored in VAO
    unsigned int VAO;
    unsigned int VBO;   // Vertex Buffer Object. Is used to send vertex data to the GPU. (id)
    unsigned int EBO;
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);

    // Creation and binding of vertex buffer object (VBO)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);     // Binds the Buffer type to the OGL context. OGL can only bind to 1 buffer of one type.
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesRect), verticesRect, GL_STATIC_DRAW);  // Writes data (float:vertices) to the GPU memory

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesRect), indicesRect, GL_STATIC_DRAW);

    int result;
    
    // Creation and binding of shader object (vertex shader)
    unsigned int vertexShader;  // Vertex shader object (id)
    vertexShader = glCreateShader(GL_VERTEX_SHADER);    // Creates a shader object of type "vertex shader".
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);     // References the shader source code to the shader object "vertexShader".
    glCompileShader(vertexShader);  // Tells OGL to compile the shader at runtime
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);    // Writes the status code to "result"
    if (!result)
    {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);   // Copies the shader error message to "infoLog"
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
                    infoLog << std::endl;
    }

    // Creation and binding of a fragment shader object (FSO). The same as the vertex shader.
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
                    infoLog << std::endl;
    }

    // Creation of a shader program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    // Attachment of the previously created shader objects to the program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);   // Links all the shader objects together. (Can create linker errors)
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
    if (!result)
    {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" <<
                    infoLog << std::endl;
    }
    glDeleteShader(vertexShader);   // Removal of the shader because they arent needed anymore.
    glDeleteShader(fragmentShader); // Removal of the shader because they arent needed anymore.

    // Telling OGL how to process/interpret the data in "vertices"
    /**
     * Parameter 1: The location of the vertex attribute
     * Parameter 2: The size of the vertex attribute (x,y,z -> 3, so 3 :))
     * Parameter 3: The data type of the vertex attribute (the vec type is always float)
     * Parameter 4: Normalizing of the values
     * Parameter 5: The space between consecutive vertex attributes. (float = 4 bytes! and there are 3 of them) So every 12 a new vertex attribute starts.
     * Parameter 6: The offset to where the vertex attributes starts in the buffer. (Maybe there is a header or smth.)
     */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);   // Enables the vertey attribute

    glBindBuffer(GL_ARRAY_BUFFER, 0);   // glVertexAttribPointer registered the VBO as the vertex attribute bound to the vertex buffer object. So it can unbind safely.
    glBindVertexArray(0); // Debinds the VAO after VBOs and vertecis configs are done
    // It is important to unbind Buffer after the VAo is unbound. The VAO stores both bind and unbinds!

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // Activates wireframe mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);   // Deactivates wireframe mode

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear buffers
        glClearColor(.3f, .2f, .3f, 1.0f);  // Sets Color the buffer buffer should be cleared
        glClear(GL_COLOR_BUFFER_BIT);   // Clear color bits only


        // Input
        processInput(window);


        // Rendering commands
        // TODO: Chapter 5; Page 38; Find out why the Element Buffer Object does not do what it supposed to do!
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // VAO dont have to be unbind every time.
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);   // Draws primitive shapes (here a triangle)
        
        // Check and call events and swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
