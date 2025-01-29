#version 330 core
in vec3 vertexColor;
in vec2 textureCoordinates;

out vec4 FragColor;

uniform vec3 objectColor;

void main()
{
    FragColor = vec4(objectColor, 1.0);
}