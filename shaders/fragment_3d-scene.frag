#version 330 core
in vec3 vertexColor;
in vec2 textureCoordinates;

out vec4 FragColor;

uniform sampler2D textureSampler0;
uniform sampler2D textureSampler1;
uniform float textureMixRatio;

void main()
{
    FragColor = mix(
        texture(textureSampler0, textureCoordinates),
        texture(textureSampler1, textureCoordinates),
        textureMixRatio
    );
}