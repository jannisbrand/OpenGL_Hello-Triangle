#ifndef SHADERCLASS_H
#define SHADERCLASS_H   

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glad/glad.h>

class Shader
{
public:
    // Shader program Id
    unsigned int ID;

    Shader(const char* vertexPath, const char* fragmentPath)
    {
        // Step 1: Secure the keys!
        std::string vertexCode, fragmentCode;
        std::ifstream vShaderFile, fShaderFile;
        // Enable exceptions
        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);   // Fail or bad file error!
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // Read file's content into the streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // Close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // Convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch(std::ifstream::failure e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
        }

        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        
        // Step 2: Ascend from darkness!
        unsigned int vertex, fragment;
        int sucess;
        char infoLog[512];

        // Vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        // Print compile errors
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &sucess);
        if (!sucess)
        {
            glGetShaderInfoLog(vertex, sizeof(infoLog), NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                        << infoLog << std::endl;
        }

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &sucess);
        if (!sucess)
        {
            glGetShaderInfoLog(fragment, sizeof(infoLog), NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                        << infoLog << std::endl;
        }

        // Step 3: Rain fire!
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        // Print linking errors
        glGetProgramiv(ID, GL_LINK_STATUS, &sucess);
        if (!sucess)
        {
            glGetProgramInfoLog(ID, sizeof(infoLog), NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                        << infoLog << std::endl;
        }
        // Delete shaders; They are already linked
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void use()
    {
        glUseProgram(ID);
    }

    void setBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
    }

    void setInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setFloat(const std::string &name, float value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
};
#endif