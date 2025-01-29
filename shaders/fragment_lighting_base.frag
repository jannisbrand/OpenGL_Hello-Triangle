#version 330 core
in vec3 vertexColor;
in vec2 textureCoordinates;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform sampler2D textureSampler;

void main()
{
    FragColor = texture(textureSampler, textureCoordinates) * vec4(lightColor * objectColor, 1.0);
    //FragColor = texture(textureSampler, textureCoordinates);
}