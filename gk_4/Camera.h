#pragma once

#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

enum CameraMode { FIRST_PERSON, THIRD_PERSON, ORBITAL };

class Camera
{
public:
    glm::vec3 Position;
    glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 cameraMatrix = glm::mat4(1.0f);
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    int width;
    int height;
    float speed = 0.1f;
    float sensitivity = 100.0f;
    bool firstClick = true;

    CameraMode mode = FIRST_PERSON;
    glm::vec3 target = glm::vec3(0.0f);
    glm::vec3 thirdPersonOffset = glm::vec3(0.0f, 2.0f, 8.0f);
    float orbitRadius = 10.0f;
    float orbitAzimuth = glm::radians(45.0f);
    float orbitElevation = glm::radians(45.0f);

    glm::vec3* attachedObject = nullptr; // Pointer to an object to follow in THIRD_PERSON mode

    Camera(int width, int height, glm::vec3 position);
    void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
    void Matrix(Shader& shader, const char* uniform);
    void Inputs(GLFWwindow* window);
    void HandleModes(GLFWwindow* window);
};