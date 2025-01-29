#version 330 core
in vec3 vertexColor;
in vec2 textureCoordinates;
in vec3 fragPosition;
in vec3 normal;

out vec4 FragColor;

// Object material definition. Describes the color of the following methods
struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
uniform Material material;
uniform vec3 objectColor;
uniform vec3 lightPosition;
uniform vec3 viewPosition;
uniform sampler2D textureSampler;

void main()
{
    vec3 ambientLight = light.ambient * material.ambient; // To always give the object some lighting. (Also called "Spielerei"))

    vec3 normalVector = normalize(normal);
    vec3 lightDirection = normalize(light.position - fragPosition);
    float diffuseAngleDifference = max(dot(normalVector, lightDirection), 0.0);    // max() assures that the result wont get in the negatives
    vec3 finalDiffuse = light.diffuse * (diffuseAngleDifference * material.diffuse);  // The lights strenght get altered by the difference in angle between the direction of light and the direction of the normal.

    float specularStrenght = 0.5;
    vec3 viewDirection = normalize(viewPosition - fragPosition);
    vec3 reflectionDirection = reflect(-lightDirection, normalVector);
    float specularAngleDifference = max(dot(viewDirection, reflectionDirection), 0.0);  // Same as in the normal calculation
    float specularShininess = pow(specularAngleDifference, material.shininess); // The exponent defines how concentratet the reflection is.
    vec3 finalSpecular = light.specular * (specularShininess * material.specular);    // Specular shininess seasoned with the specular color (-material) :)

    vec3 finalColor = (ambientLight + finalDiffuse + finalSpecular);
    FragColor = vec4(finalColor, 1.0);
    // FragColor = vec4(normalVector * 0.5 + 0.5, 1.0);

    // FragColor = vec4(finalLight, 1.0);
    // FragColor = vec4(lightColor * ambientLight, 1.0);
    // FragColor = texture(textureSampler, textureCoordinates) * vec4(finalLight, 1.0);
    // FragColor = texture(textureSampler, textureCoordinates);
}