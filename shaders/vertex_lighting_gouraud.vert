#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTextureCoordinates;
layout (location = 3) in vec3 aNormal;

out vec2 textureCoordinates;
out vec3 vertexColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 objectColor;
uniform vec3 viewPosition;

void main()
{
    vec3 modPosition = vec3(model * vec4(aPosition, 1.0));  // Transforms the vertex position to world space! This calculation is done seperatley so that the world space coordinates are available in the fragment shader. Otherwise the fragment shader would end up with the coordinates of clip space.. All for lighting!
    vec3 normal = mat3(transpose(inverse(model))) * aNormal; // Complicated math to avoid light disstortions if uniform scales are performed on an Object at the conversion to worldspace
    
    float ambientStrength = 0.1;
    vec3 ambientLight = ambientStrength * lightColor;

    vec3 lightDirection = normalize(lightPosition - modPosition);
    vec3 normalDirection = normalize(normal);
    float diffuseAngleDifference = (dot(normalDirection, lightDirection));
    vec3 finalDiffuse = diffuseAngleDifference * lightColor;    // The angle difference acts like the modifier for the light! :)

    float specularStrenght = 0.3;
    vec3 viewDirection = normalize(viewPosition - modPosition); // Because we want the angle to the light affected object. Where else would a shiny reflection be??
    vec3 reflectionDirection = reflect(-lightDirection, viewDirection);
    float specularAngleDifference = (dot(viewDirection, reflectionDirection));
    float specularShininess = pow(specularAngleDifference, 32);
    vec3 finalSpecular = specularStrenght * specularShininess * lightColor;
 
    vec3 finalColor = (ambientLight + finalDiffuse + finalSpecular) * objectColor;
    
    vertexColor = finalColor;
    textureCoordinates = aTextureCoordinates;
    gl_Position = projection * view * model * vec4(aPosition, 1.0);
}