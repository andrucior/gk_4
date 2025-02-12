#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Shader.h"

enum LightType { POINT_LIGHT, SPOT_LIGHT, DIRECTIONAL_LIGHT };

class LightSource
{
public:
    LightType type;
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 direction; // used for spotlight and directional light
    float cutOff;       // for spotlight (inner cutoff, cosine)
    float outerCutOff;  // for spotlight (outer cutoff, cosine)

    LightSource() {}
    LightSource(LightType t, const glm::vec3& pos, const glm::vec3& col)
        : type(t), position(pos), color(col),
        direction(glm::vec3(1.0f, 1.0f, 1.0f)),
        cutOff(glm::cos(glm::radians(12.5f))),
        outerCutOff(glm::cos(glm::radians(17.5f)))
    { }

    // Set uniforms into the shader 
    void setUniforms(Shader& shader, const std::string& namePrefix) {
        // Set common properties.
        glUniform3f(glGetUniformLocation(shader.ID, (namePrefix + ".position").c_str()),
            position.x, position.y, position.z);
        glUniform3f(glGetUniformLocation(shader.ID, (namePrefix + ".color").c_str()),
            color.x, color.y, color.z);

        // SpotLight specific uniforms.
        if (type == SPOT_LIGHT) {
            glm::vec3 normDirection = glm::normalize(direction); // Ensure direction is normalized.
            glUniform3f(glGetUniformLocation(shader.ID, (namePrefix + ".direction").c_str()),
                normDirection.x, normDirection.y, normDirection.z);
            // Send the already computed cosine values directly.
            glUniform1f(glGetUniformLocation(shader.ID, (namePrefix + ".cutOff").c_str()),
                cutOff);
            glUniform1f(glGetUniformLocation(shader.ID, (namePrefix + ".outerCutOff").c_str()),
                outerCutOff);
        }

        // Directional Light specific uniforms.
        if (type == DIRECTIONAL_LIGHT) {
            glm::vec3 normDirection = glm::normalize(direction);
            glUniform3f(glGetUniformLocation(shader.ID, (namePrefix + ".direction").c_str()),
                normDirection.x, normDirection.y, normDirection.z);
        }
    }
};


