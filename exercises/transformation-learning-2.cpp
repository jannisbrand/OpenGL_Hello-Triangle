#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "../textures/stb_image.h"
#include "../shaders/shaderclass.h"
#include "../libraries/glm/glm.hpp"
#include "../libraries/glm/gtc/matrix_transform.hpp"
#include "../libraries/glm/gtc/type_ptr.hpp"

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_VERSION_MINOR, 3);

    GLFWwindow *window = glfwCreateWindow(800, 600, "Chapter 7 - Exercise 1 - Rotate texture in shader", 0, NULL);

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {   
        std::cout << "No context could be found!";
        glfwTerminate();
        return 1;
    }

    float vertices[] = {
        // positions        // colors         // texture coords [2]
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f // top left
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int ebo, vao, vbo, texture1, texture2;
    int result, texWidth1, texWidth2, texHeight1, texHeight2, nrColorChannels1, nrColorChannels2;
    unsigned char *texData;

    glGenTextures(1, &texture1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    texData = stbi_load("../textures/container.jpg", &texWidth1, &texHeight1, &nrColorChannels1, 0);
    if (texData)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth1, texHeight1, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(texData);
    }
    else
    {
        std::cout << "TEXTURE 1 - ERORR";
    }

    glGenTextures(1, &texture2);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    unsigned char *texData2 = stbi_load("../textures/awesomeface.png", &texWidth2, &texHeight2, &nrColorChannels2, 0);
    if (texData)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth2, texHeight2, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData2);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(texData2);
    }
    else
    {
        std::cout << "TEXTURE 2 - ERORR";
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &ebo);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    Shader textureShader("../shaders/vertex_transformation-learning-2.vert", "../shaders/fragment_texture_exercise-1.frag");
    textureShader.use();
    textureShader.setInt("textureSampler1", 0);
    textureShader.setInt("textureSampler2", 1);

    glm::mat4 translationMat = glm::mat4(1.0f);
    translationMat = glm::translate(translationMat, glm::vec3(0.5f, -0.5f, 0.0f));  // REMEBER THAT THESE ARE THE DISPLAY COORDS NOT THE TEXTUREs!!!! :)
    translationMat = glm::rotate(translationMat, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // Combines a rotation matrix with the identity matrix. The unit vector tells axis to rotate around. 
    // translationMat = glm::scale(translationMat, glm::vec3(0.5f, 0.5f, 0.5f));   // Combines the scaling of 0.5 to the translation matrix

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.4f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        float rot = sin(glfwGetTime()) * 360;
        std::cout << rot / 100000 << std::endl;
        translationMat = glm::rotate(translationMat, glm::degrees(rot / 5000000), glm::vec3(0.0f, 0.0f, 1.0f));
        textureShader.use();
        glUniformMatrix4fv(glGetUniformLocation(textureShader.ID, "transform"), 1, GL_FALSE, glm::value_ptr(translationMat));
        glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}