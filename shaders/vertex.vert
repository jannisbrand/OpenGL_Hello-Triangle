#version 330 core   // Specifies the version of OGL (In this case 3.4).
layout (location = 0) in vec3 aPos;     // Defines aPos as an input variabel at location 0.

void main()
{
    glPosition = vec4(aPos.x, aPos.y, aPos.z, 1.0);     // Represents the output of the vertex shader (Data as a type: vec4).
}