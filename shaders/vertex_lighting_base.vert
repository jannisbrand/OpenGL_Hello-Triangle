#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTextureCoordinates;

out vec2 textureCoordinates;
out vec3 vertexColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPosition, 1.0);
    vertexColor = vec3(1.0, 1.0, 1.0);
    textureCoordinates = aTextureCoordinates;
}