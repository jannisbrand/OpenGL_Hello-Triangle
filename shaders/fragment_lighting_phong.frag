#version 330 core
in vec3 vertexColor;
in vec2 textureCoordinates;
in vec3 fragPosition;
in vec3 normal;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 viewPosition;
uniform sampler2D textureSampler;

void main()
{
    float ambientStrength = 0.1;
    vec3 ambientLight = ambientStrength * lightColor; // To always give the object some lighting. (Also called "Spielerei"))


    vec3 normalVector = normalize(normal);
    vec3 lightDirection = normalize(lightPosition - fragPosition);
    float diffuseAngleDifference = max(dot(normalVector, lightDirection), 0.0);    // max() assures that the result wont get in the negatives
    vec3 finalDiffuse = diffuseAngleDifference * lightColor;  // The lights strenght get altered by the difference in angle between the direction of light and the direction of the normal.
    // (Not needed any more! vec3 finalDiffuse = (ambientLight + diffusedColor) * objectColor; // All lighting calculations done and added up!

    float specularStrenght = 0.5;
    vec3 viewDirection = normalize(viewPosition - fragPosition);
    vec3 reflectionDirection = reflect(-lightDirection, normalVector);
    float specularAngleDifference = max(dot(viewDirection, reflectionDirection), 0.0);  // Same as in the normal calculation
    float specularShininess = pow(specularAngleDifference, 256); // The exponent defines how concentratet the reflection is.
    vec3 finalSpecular = specularStrenght * specularShininess * lightColor;

    vec3 finalColor = (ambientLight + finalDiffuse + finalSpecular) * objectColor;
    FragColor = vec4(finalColor, 1.0);
    // FragColor = vec4(normalVector * 0.5 + 0.5, 1.0);

    // FragColor = vec4(finalLight, 1.0);
    // FragColor = vec4(lightColor * ambientLight, 1.0);
    // FragColor = texture(textureSampler, textureCoordinates) * vec4(finalLight, 1.0);
    // FragColor = texture(textureSampler, textureCoordinates);
}