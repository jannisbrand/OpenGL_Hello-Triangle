 #version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
out vec3 vertexColor;
out vec2 texCoord;
uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
    gl_Position = projection * view * model * transform * vec4(aPos, 1.0);  // The Matrixs[3][X] and Vectors[3] homogenious value is 1 !here!
    vertexColor = vec3(1.0, 1.0, 1.0);
    texCoord = aTexCoord;
}