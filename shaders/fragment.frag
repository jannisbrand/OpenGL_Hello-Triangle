#version 330 core   // Specifies the version of OGL
out vec4 FragColor;     // Defining of an output variable. The frament only requires 1 output variable.

void main()
{
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);   // Sets the color values (RGB) and opaquenis to 1.
}