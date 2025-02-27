#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int main() {
    std::cout << "GLM test started!" << std::endl;

    glm::vec3 position(1.0f, 2.0f, 3.0f);
    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);

    std::cout << "Transform Matrix:\n";
    const float* matrix = glm::value_ptr(model);
    for (int i = 0; i < 16; i++) {
        std::cout << matrix[i] << " ";
        if ((i + 1) % 4 == 0) std::cout << std::endl;
    }

    return 0;
}
