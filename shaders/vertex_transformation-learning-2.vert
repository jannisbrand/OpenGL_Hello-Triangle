 #version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
out vec3 vertexColor;
out vec2 texCoord;
uniform mat4 transform;
void main()
{
    gl_Position = transform * vec4(aPos, 1.0);  // The Matrixs[3][X] and Vectors[3] homogenious value is 1 !here!
    vertexColor = aColor;
    texCoord = aTexCoord;
}