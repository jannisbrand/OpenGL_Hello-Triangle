#include <iostream>
#include "../libraries/glm/glm.hpp"
#include "../libraries/glm/gtc/matrix_transform.hpp"
#include "../libraries/glm/gtc/type_ptr.hpp"

int main()
{
    glm::vec4 originVector(1.0f, 0.0f, 0.0f, 1.0f); // Just a vector
    glm::mat4 translationMatx = glm::mat4(1.0f);    // Translation matrix initialized as identity matrix
    translationMatx = glm::translate(translationMatx, glm::vec3(1.0f, 1.0f, 0.0f)); // Combines the identity matrix with the translation vector
    originVector = translationMatx * originVector; // Matrix multiplication results in translated originVector!

    std::cout << originVector.x << originVector.y << originVector.z << std::endl;

    return 0;
}