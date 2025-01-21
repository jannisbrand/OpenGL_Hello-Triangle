#define GLM_ENABLE_EXPERIMENTAL // For glm string cast
#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <../libraries/glm/glm.hpp>
#include <../libraries/glm/gtc/matrix_transform.hpp>
#include <../libraries/glm/gtx/string_cast.hpp>
#include "../textures/stb_image.h"
#include "../shaders/shaderclass.h"

unsigned int windowWidth = 720;
unsigned int windowHeight = 480;
glm::vec3 cameraPosition;
glm::vec3 cameraFront;  // Points in the direction of the cameras view
glm::vec3 cameraRight;
float cameraPitch = 0.0f;
float cameraYaw = -90.0f;
float cameraRoll = 0.0f;
float cameraSpeed = 0.5f; 
float mouseSensitivity = 0.4;
glm::vec3 worldOrigin = glm::vec3(0.0f, 0.0f, 0.0f);

float scrollSensitivity = 1.0f;
float fov = 45.0f;

float lastFrame = 0.0f;
float deltaTime = 0.0f;

float lastPosX = 0.0f;
float lastPosY = 0.0f;

// Should be a rectangle :)
float vertecis[] = {
    0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.0f, 0.0f, 1.0f
};

// Indexes in groups of 3's to tell which vetecis form triangles :)
unsigned int verteciIndex[] = {
    0, 1, 3,
    1, 2, 3
};

unsigned int VAO, VBO, EBO, texture1, texture2;
int texWidth1, texWidth2, texHeight1, texHeight2, nrColorChannels1, nrColorChannels2;

void processMouse(GLFWwindow *window, double xPos, double yPos)
{
    float xPosOffset = xPos - lastPosX;
    float yPosOffset = yPos - lastPosY;
    lastPosX = xPos;
    lastPosY = yPos;

    xPosOffset *= mouseSensitivity;
    yPosOffset *= mouseSensitivity;

    cameraYaw += xPosOffset;
    cameraPitch -= yPosOffset;

    if (cameraPitch >= 89.9999f)
    {
        cameraPitch = 89.9999f;
    }
    if (cameraPitch <= -89.9999f)
    {
        cameraPitch = -89.9999f;
    }
}

void processKeyboard(GLFWwindow *window)
{
    float deltaSpeed = cameraSpeed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        glm::vec3 tempRight = glm::normalize(glm::cross(cameraFront, glm::vec3(0.0f, 1.0f, 0.0f))); 
        cameraPosition -= deltaSpeed * (glm::cross(tempRight, glm::vec3(0.0f, 1.0f, 0.0f)));
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        glm::vec3 tempRight = glm::normalize(glm::cross(cameraFront, glm::vec3(0.0f, 1.0f, 0.0f))); 
        cameraPosition += deltaSpeed * (glm::cross(tempRight, glm::vec3(0.0f, 1.0f, 0.0f)));
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        cameraPosition -= deltaSpeed * glm::cross(cameraFront, glm::vec3(0.0f, 1.0f, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraPosition += deltaSpeed * glm::cross(cameraFront, glm::vec3(0.0f, 1.0f, 0.0f));
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        glm::vec3 tempRight = glm::normalize(deltaSpeed * glm::cross(cameraFront, glm::vec3(0.0f, 1.0f, 0.0f))); 
        cameraPosition -= deltaSpeed * glm::cross(cameraFront, tempRight);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        glm::vec3 tempRight = glm::normalize(deltaSpeed * glm::cross(cameraFront, glm::vec3(0.0f, 1.0f, 0.0f)));
        std::cout << glm::to_string(tempRight) << std::endl;
        cameraPosition += deltaSpeed * glm::cross(cameraFront, tempRight);
    }

}

void processScrollWheel(GLFWwindow *window, double xOffset, double yOffset)
{
    fov += yOffset * scrollSensitivity;

    if (fov > 139.9999f)
    {
        fov = 140;
    }
    if (fov < 25.0000f)
    {
        fov = 25;
    }

    std::cout << fov << std::endl;
}

int main()
{
    glfwInit();

    glfwWindowHint(GLFW_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_VERSION_MINOR, 3);

    GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight, "Chapter 10 - Camera shenanigans", 0, NULL);
    if (!window)
    {
        std::cout << "Window could not be created!" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "No context could be found!";
        window = nullptr;
    }

    glViewport(0, 0, windowWidth, windowHeight);
    glfwSetCursorPosCallback(window, processMouse);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetScrollCallback(window, processScrollWheel);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertecis), vertecis, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(verteciIndex), verteciIndex, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
    // Pinter "1" is used for color data in the my shaders, soooo...
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    std::cout << "Verteci data loaded.. " << std::endl;

    unsigned char *textureData;
    
    textureData = stbi_load("../textures/container.jpg", &texWidth1, &texHeight1, &nrColorChannels1, 0);
    if (textureData)
    {
        glActiveTexture(GL_TEXTURE0);
        glGenTextures(1, &texture1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth1, texHeight1, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
        glGenerateTextureMipmap(texture1);
        stbi_image_free(textureData);
    }
    else
    {
        std::cout << "Peniss!";
    }

    textureData = stbi_load("../textures/awesomeface.png", &texWidth2, &texHeight2, &nrColorChannels2, 0);
    if (textureData)
    {
        glActiveTexture(GL_TEXTURE1);
        glGenTextures(1, &texture2);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth2, texHeight2, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
        glGenerateTextureMipmap(texture2);
        stbi_image_free(textureData);
    }
    else
    {
        std::cout << "Peniss!";
    }

    std::cout << "Textures laoded.." << std::endl;

    Shader shader("../shaders/vertex_3d-scene.vert", "../shaders/fragment_3d-scene.frag");
    shader.setInt("textureSampler0", 0);
    shader.setInt("textureSampler1", 1);

    // Matrices and camera
    glm::mat4 model = glm::mat4(1.0f);  // To transform local coordinates (Verteci data) world space
    glm::mat4 view = glm::mat4(1.0f);   // To transform world coordinates to screenspace coordinates
    glm::mat4 projection = glm::mat4(1.0f); // Performs perspective divison on the view space. 
    
    cameraPosition = glm::vec3(0.0f, 0.0f, -0.2f);
    cameraYaw = 90.0f;

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f, 0.4f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        deltaTime = glfwGetTime() - lastFrame;
        lastFrame = glfwGetTime();

        // processInput();
        processKeyboard(window);

        // float cameraYaw = sin(glfwGetTime() / 4) * 180;
        // Calculate camera direction
        glm::vec3 cameraDirection;
        cameraDirection.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
        cameraDirection.y = sin(glm::radians(cameraPitch));
        cameraDirection.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
        cameraFront = glm::normalize(cameraDirection); // Its just necessary know the direction. Thats why normalizing 
        
        view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, glm::vec3(0.0f, 1.0f, 0.0f));

        projection = glm::perspective(glm::radians(fov), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);

        shader.use();
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}