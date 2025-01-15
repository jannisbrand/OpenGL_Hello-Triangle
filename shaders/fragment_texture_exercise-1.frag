#version 330 core
in vec3 vertexColor;
in vec2 texCoord;
out vec4 FragColor;
uniform sampler2D textureSampler1;
uniform sampler2D textureSampler2;
void main()
{
    FragColor = mix(
        texture(textureSampler1, texCoord),
        texture(textureSampler2, texCoord * -1),
        0.2
    );
}