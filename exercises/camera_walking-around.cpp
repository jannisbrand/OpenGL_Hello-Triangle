#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "../textures/stb_image.h"
#include "../shaders/shaderclass.h"
#include "../libraries/glm/glm.hpp"
#include "../libraries/glm/gtc/matrix_transform.hpp"
#include "../libraries/glm/gtc/type_ptr.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "../libraries/glm/gtx/string_cast.hpp"

glm::vec3 worldOrigin = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 arbutiaryUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 cameraDirection;
float cameraYaw = -90.0f;
float cameraPitch = 0.0f;
float cameraRoll = 0.0f;

float cameraSpeed = 0.002;

float currentFrame = 0.0f;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

double lastMousePosX = 400.0f;
double lastMousePosY = 300.0f;
float sensitivity = 0.1f;
bool firstMouseMovement = true;

void mouse_callback(GLFWwindow *window, double xPos, double yPos)
{
    if (firstMouseMovement)
    {
        lastMousePosX = xPos;
        lastMousePosY = yPos;
        firstMouseMovement = false;
    }
    float xPosDelta = (xPos - lastMousePosX);
    float yPosDelta = (lastMousePosY - yPos);
    lastMousePosX = xPos;
    lastMousePosY = yPos;

    xPosDelta *= sensitivity;
    yPosDelta *= sensitivity;
    cameraYaw += xPosDelta;
    cameraPitch += yPosDelta;

    // Contraints to prevent flipping
    if (cameraPitch > 89.0f)
    {
        cameraPitch = 89.0f;
    }
    else if (cameraPitch < -89.0f)
    {
        cameraPitch = -89.0f;
    }    

    // Camera direction (Without roll);
    glm::vec3 cameraDirection;
    cameraDirection.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    cameraDirection.y = sin(glm::radians(cameraPitch));
    cameraDirection.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    cameraFront = glm::normalize(cameraDirection);  // Sets the front facing vector to the new calculated one!
}

void processInput(GLFWwindow *window)
{
    cameraSpeed = 0.5 * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraPosition += cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraPosition -= cameraSpeed * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraPosition += glm::normalize(glm::cross(cameraFront, arbutiaryUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cameraPosition -= glm::normalize(glm::cross(cameraFront, arbutiaryUp)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        cameraPosition -= cameraSpeed * arbutiaryUp;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        cameraPosition += cameraSpeed * arbutiaryUp;
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, 1);
    }
}

int main()
{
    // Init process
    glfwInit();
    glfwWindowHint(GLFW_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_VERSION_MINOR, 3);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Chapter - 10: Walking on sunshine", 0, NULL);

    glfwMakeContextCurrent(window);
    if (!window)
    {
        std::cout << "Failed to create a window!" << std::endl;
        glfwTerminate();
    }

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Could not find a context!" << std::endl;
    }

    // Vertex data part (cube)
    float vertices[] = {
        // Cube face 1
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
         0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
         0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        // Face 2
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
         0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
         0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        // Face 3
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        // Face 4
         0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
         0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
         0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
         0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        // Face 5
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
         0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
         0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        // Face 6
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
         0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
         0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
         0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
    };

    // A few random positions- in world space for objects to be placed at
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f, 0.0f, 0.0f),
        glm::vec3( 2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f, 2.0f, -2.5f),
        glm::vec3( 1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
    };

    // Loading the vertecis into the app!
    unsigned int vao, vbo;

    glGenVertexArrays(1, &vao); // Vertex Array Object: Holds all information of other buffers while its binded!
    glGenBuffers(1, &vbo);  // Vertex Buffer Object: Holds pointers to vertecis data and other parameters!
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);  

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  // Self explainatory..
    // Vertex Attribute Pointer for vertex positions (x, y, z -> local space)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);  // Pointer ""Id"" = 0 / Amount of different data = 3 (x, y, z) / Float type / Not nomralized / Stride: The length from the beginning of the first verteci position, over the other data (maybe texture coords or color) to the first position of the next verteci / Where the verteci data starts (maybe there is a header or smth..)
    // Vertex Attribute Ponter for texture coordinates (x, y -> positions on verteci level)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float))); // Points to the last values (texture coords x & y) that are "behind" 3 floats (the verteci position -> local space)
    // Enabling the before defined vertex attribute pointers to be used
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);   // De-Binds the Array Buffer.
    glBindVertexArray(0); // All settings / parameters / defines / and the vbo (Vertex Buffer Object) done before before are saved in the vao (Vertex Array Object)

    // Texture loading process (manual)
    unsigned char *textureData;
    int texWidth1, texWidth2, texHeight1, texHeight2, nrColorChannels1, nrColorChannels2;
    unsigned int texture1, texture2;

    textureData = stbi_load("../textures/container.jpg", &texWidth1, &texHeight1, &nrColorChannels1, 0);
    if (textureData)
    {
        glGenTextures(1, &texture1);
        glActiveTexture(GL_TEXTURE0);   // Like a vao (Vertex Array Object) it holds the settings / parameters / etc. of textures 
        glBindTexture(GL_TEXTURE_2D, texture1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth1, texHeight1, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);    // Same as with the VertexAttribPointer that defines the correct usage of the texture data. This gets stored in the active Texture "place".
        glGenerateMipmap(GL_TEXTURE_2D);    // Generates Mip maps. Additional textures that decline in resolution for use if only a small amount of fragments would be seen on screen.
        // Texture parameter
        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);    // Defines what filtering method is applied when the scale gets smaller or higer.
        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);   // The texture will repeat if the it falls out of the texture coordinate range. (0 - 1)
        stbi_image_free(textureData);
    }
    else
    {
        std::cout << "Failed to load the container texture!" << std::endl;
    }

    stbi_set_flip_vertically_on_load(1);
    textureData = stbi_load("../textures/awesomeface.png", &texWidth2, &texHeight2, &nrColorChannels2, 0);
    if (textureData)
    {
        glGenTextures(1, &texture2);
        glActiveTexture(GL_TEXTURE1);   // Like a vao (Vertex Array Object) it holds the settings / parameters / etc. of textures 
        glBindTexture(GL_TEXTURE_2D, texture2);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth2, texHeight2, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);    // Same as with the VertexAttribPointer that defines the correct usage of the texture data. This gets stored in the active Texture "place".
        glGenerateMipmap(GL_TEXTURE_2D);    // Generates Mip maps. Additional textures that decline in resolution for use if only a small amount of fragments would be seen on screen.
        // Texture parameter
        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);    // Defines what filtering method is applied when the scale gets smaller or higer.
        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);   // The texture will repeat if the it falls out of the texture coordinate range. (0 - 1)
        stbi_image_free(textureData);
    }
    else
    {
        std::cout << "Failed to load the awesomeface texture!" << std::endl;
    }

    // Shader initialization (manual - you need a const char* :()
    Shader textureShader("../shaders/vertex_3d-scene.vert", "../shaders/fragment_3d-scene.frag");
    textureShader.use();
    textureShader.setInt("textureSampler0", 0);
    textureShader.setInt("textureSampler1", 1);

    // Defining translation matrices
    glm::mat4 translation = glm::mat4(1.0f);  
    glm::mat4 model = glm::mat4(1.0f);  // Initializes the matrix as a identity matrix!
    glm::mat4 view = glm::mat4();
    glm::mat4 projection = glm::mat4(1.0f);

    // view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));  // Translates the "camera" back with 3 / Translates the world space forward with 3 :)
    view = glm::lookAt(cameraPosition, worldOrigin, arbutiaryUp);   // Initial camera position
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);  // Creates a frustum

    // Render parameter
    glEnable(GL_DEPTH_TEST);    // Enables OpenGL to consider the w coordinate to perfrom depth testing (z-buffer)
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f, 0.2f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        model = glm::mat4(1.0f);
        view = glm::mat4();

        currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        // Create a look at matrix
        view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, arbutiaryUp);  // With the additon of the normalized camera front vec3 to the camera position vec3 its possible to let the camera follow the carrot ;)

        // ### Performance monitoring ###
        std::cout << "\033[2J\033[H" << "CAM: " << glm::to_string(cameraPosition) << std::endl <<
            "CURSOR: " << glm::to_string(cameraFront) << std::endl <<
            "DELTA T: " << deltaTime << std::endl <<
            "FPS: " << (1 / deltaTime);
        // ### Performance monitoring ###

        // ### ONE CUBE ###
        // glBindVertexArray(vao);
        
        // model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

        // textureShader.use();
        // textureShader.setMat4("translation", translation);
        // textureShader.setMat4("model", model);
        // textureShader.setMat4("view", view);
        // textureShader.setMat4("projection", projection);
        // textureShader.setFloat("textureMixRatio", 0.2f);

        // glDrawArrays(GL_TRIANGLES, 0, 36);  // Draw triangles / Starts at the firt verteci and draws 36 (cube..)
        // ### ONE CUBE ###

        // ### MORE CUBES ###
        glBindVertexArray(vao);

        for (int i = 0; i < 10; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, glm::radians((float)glfwGetTime() * 3 * i), glm::vec3(0.75f, 1.0f, 0.2f));

            textureShader.use();
            textureShader.setMat4("translation", translation);
            textureShader.setMat4("model", model);
            textureShader.setMat4("view", view);
            textureShader.setMat4("projection", projection);
            textureShader.setFloat("textureMixRatio", 0.2f);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        // ### MORE CUBES ###


        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}