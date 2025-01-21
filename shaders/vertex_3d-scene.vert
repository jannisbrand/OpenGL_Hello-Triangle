#version 330 core
layout (location = 0) in vec3 aPositon;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTextureCoordinates;

out vec3 vertexColor;
out vec2 textureCoordinates;

uniform mat4 model; // World space
uniform mat4 view;  // View space (Camera)
uniform mat4 projection; // Perspective devision -> To clip space

void main()
{
    gl_Position = projection * view * model * vec4(aPositon, 1.0f);
    vertexColor = aColor;  
    textureCoordinates = aTextureCoordinates;
}

