#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTextureCoordinates;
layout (location = 3) in vec3 aNormal;

out vec2 textureCoordinates;
out vec3 vertexColor;
out vec3 normal;
out vec3 fragPosition;  // Position of the fragment in "World space!"

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vertexColor = vec3(1.0, 1.0, 1.0);
    textureCoordinates = aTextureCoordinates;
    fragPosition = vec3(model * vec4(aPosition, 1.0));  // Transforms the vertex position to world space! This calculation is done seperatley so that the world space coordinates are available in the fragment shader. Otherwise the fragment shader would end up with the coordinates of clip space.. All for lighting!
    normal = mat3(transpose(inverse(model))) * aNormal; // Complicated math to avoid light disstortions if uniform scales are performed on an Object at the conversion to worldspace
    gl_Position = projection * view * vec4(fragPosition, 1.0);
}