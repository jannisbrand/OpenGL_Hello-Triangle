/**
 * This basic scene containes a cube to represent a light source and a cube to represent a light reciever.
 */

#define STB_IMAGE_IMPLEMENTATION
#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "../shaders/shaderclass.h"
#include "../textures/stb_image.h"
#include "../libraries/glm/glm.hpp"
#include "../libraries/glm/gtc/matrix_transform.hpp"
#include "../libraries/glm/gtx/string_cast.hpp"

int windowWidth = 800;
int windowHeight = 600;

float cameraFov = 45.0f;
float cameraSensitivity = 0.2f;
float cameraSpeed = 0.5f;
float cameraYaw = 0.0f;
float cameraPitch = 0.0f;
glm::vec3 cameraPos;
glm::vec3 cameraFront;
glm::vec3 cameraRight;
glm::vec3 cameraUp;

glm::vec3 worldOriginPos = glm::vec3(0.0f, 0.0f, 0.0f);

float deltaTime = 0.0f;
float lastFrameTime = 0.0f;
float framesPerSecound = 0.0f;

glm::vec3 lightPos(0.0f, 0.5f, -3.0f);

// CUBE
float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

struct Metal
{
    glm::vec3 ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    glm::vec3 diffuse = glm::vec3(0.05f, 0.05f, 0.05f);
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
    float shininess = 256.0f;
};

struct Wood
{
    glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 specular = glm::vec3(0.1f, 0.1f, 0.1f);
    float shininess = 1.0f;
};

void processKeyboard(GLFWwindow *window)
{
    float cameraOffset = cameraSpeed * deltaTime;

    // Shift the cameras position by the value "camera speed" in the direction of the camera front facing vector
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        cameraPos += cameraOffset * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        cameraPos -= cameraOffset * cameraFront;
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        lightPos += cameraOffset * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        lightPos -= cameraOffset * cameraFront;
    }
}

void processMouse(GLFWwindow *window, double xPos, double yPos)
{
    static float lastPosX;
    static float lastPosY;

    float xOffset = lastPosX - xPos;
    float yOffset = lastPosY - yPos;
    lastPosX = xPos;
    lastPosY = yPos;

    xOffset *= cameraSensitivity;
    yOffset *= cameraSensitivity;

    cameraYaw += xOffset;
    cameraPitch -= yOffset;
}

glm::mat4 lookAt(glm::vec3 cPos, glm::vec3 cTarget, glm::vec3 up)
{
    glm::vec3 cDirection = glm::normalize(cPos - cTarget);
    glm::vec3 cRight = glm::normalize(glm::cross(cDirection, up));
    glm::vec3 cUp    = glm::normalize(glm::cross(cDirection, cRight));

    glm::mat4 rotation = glm::mat4(1.0f);
    // Remember: COLUMN MAJOR!
    rotation[0] = glm::vec4(cRight.x, cUp.x, cDirection.x, 0.0f);
    rotation[1] = glm::vec4(cRight.y, cUp.y, cDirection.y, 0.0f);
    rotation[2] = glm::vec4(cRight.z, cUp.z, cDirection.z, 0.0f);

    glm::mat4 translation = glm::mat4(1.0f);
    translation[3] = glm::vec4(-cPos.x, -cPos.y, -cPos.z, 1);

    return rotation * translation;
}


int main()
{
    glfwInit();

    glfwWindowHint(GLFW_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_VERSION_MINOR, 3);

    GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight, "Chapter - 12: Basic Lighting -> Basic Scene", 0, NULL);

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Context could not be found!";
        glfwTerminate();
        return -1;
    }

    glViewport((GLint)0, (GLint)0, (GLsizei)windowWidth, (GLsizei)windowHeight);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, processMouse);

    // ***********************************VERTEX DATA****************************************

    unsigned int vao_base, vao_lightSource, vbo;
    glGenVertexArrays(1, &vao_base);
    glGenVertexArrays(1, &vao_lightSource);
    glGenBuffers(1, &vbo);

    // Base Cube :)
    glBindVertexArray(vao_base);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);  // Vertex position (x,y,z)
    // --> No vertex color data in the base example! (1)
    // --> No texture coordinates in this one
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3* sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // Light source cube :)
    glBindVertexArray(vao_lightSource);
    glBindBuffer(GL_ARRAY_BUFFER, vbo); 
    // No additional buffer data definition needed. The buffer "vbo" already has it defined ;)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);  // Vertex position (x,y,z)
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // END TEST
    // ***********************************END VERTEX DATA****************************************

    // ***********************************TEXTURE****************************************
    unsigned int texture;
    int textureWidth, textureHeight, nrColorChannels;

    unsigned char *textureData = stbi_load("../textures/container.jpg", &textureWidth, &textureHeight, &nrColorChannels, 0);
    if (textureData)
    {
        glActiveTexture(GL_TEXTURE0);
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(textureData);
    }
    else
    {
        std::cout << "Error while loading the texture!" << std::endl;
        glfwTerminate();
        return -1;
    }

    // ***********************************END TEXTURE****************************************

    // ***********************************SHADER****************************************
    Shader baseShader("../shaders/vertex_lighting_material_phong.vert", "../shaders/fragment_lighting_material_phong.frag");
    baseShader.use();
    baseShader.setInt("textureSampler", 0);
    Shader lightSourceShader("../shaders/vertex_lighting_light-source.vert", "../shaders/fragment_lighting_light-source.frag");

    // ***********************************END SHADER****************************************

    // ***********************************CAMERA AND MATRICES****************************************
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4();

    cameraPos = glm::vec3(0.0f, -2.1f, -4.0f);

    view = lookAt(cameraPos, worldOriginPos, glm::vec3(0.0f, 0.0f, 3.0f));
    projection = glm::perspective(glm::radians(cameraFov), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);   // Vision frustum

    // ***********************************END CAMERA AND MATRICES****************************************

    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FULL_SUPPORT);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float current = (float)glfwGetTime();
        deltaTime = current - lastFrameTime;
        lastFrameTime = current;

        processKeyboard(window);

        glm::vec3 cameraDirection;
        cameraDirection.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
        cameraDirection.y = sin(glm::radians(cameraPitch));
        cameraDirection.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
        cameraFront = cameraDirection;

        view = lookAt(cameraPos, cameraPos + cameraDirection, glm::vec3(0.0f, 1.0f, 0.0f));

        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
        baseShader.use();

        Wood wood;

        glm::vec3 brown = glm::vec3(0.7f, 0.2f, 0.2f);

        // WOOD
        // Material-Colors of the light and the object 
        baseShader.setVec3("light.position", lightPos);    // Position of the light. (Cube in this example)
        baseShader.setVec3("viewPosition", cameraPos);
        baseShader.setVec3("light.ambient", brown * glm::vec3(1.0f));
        baseShader.setVec3("light.diffuse", brown * glm::vec3(1.0f));
        baseShader.setVec3("light.specular", brown * glm::vec3(1.0f));
        baseShader.setVec3("material.ambient", wood.ambient);
        baseShader.setVec3("material.diffuse", wood.diffuse);
        baseShader.setVec3("material.specular", wood.specular);
        baseShader.setFloat("material.shininess", wood.shininess);
        baseShader.setMat4("model", model);
        baseShader.setMat4("view", view);
        baseShader.setMat4("projection", projection);
        glBindVertexArray(vao_base);
        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);


        model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
        baseShader.use();

        Metal metal;

        // METAL
        // Material-Colors of the light and the object 
        baseShader.setVec3("light.position", lightPos);    // Position of the light. (Cube in this example)
        baseShader.setVec3("viewPosition", cameraPos);
        baseShader.setVec3("light.ambient", glm::vec3(1.0f));
        baseShader.setVec3("light.diffuse", glm::vec3(1.0f));
        baseShader.setVec3("light.specular", glm::vec3(1.0f));
        baseShader.setVec3("material.ambient", metal.ambient);
        baseShader.setVec3("material.diffuse", metal.diffuse);
        baseShader.setVec3("material.specular", metal.specular);
        baseShader.setFloat("material.shininess", metal.shininess);
        baseShader.setMat4("model", model);
        baseShader.setMat4("view", view);
        baseShader.setMat4("projection", projection);
        glBindVertexArray(vao_base);
        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);


        glBindVertexArray(vao_lightSource);
        model = glm::translate(model, lightPos); // Position of the light. (Cube in this example)
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        lightSourceShader.use();
        lightSourceShader.setVec3("objectColor", glm::vec3(1.0f));
        lightSourceShader.setMat4("model", model);
        lightSourceShader.setMat4("view", view);
        lightSourceShader.setMat4("projection", projection);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}