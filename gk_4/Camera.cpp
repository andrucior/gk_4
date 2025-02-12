#include "Camera.h"

Camera::Camera(int width, int height, glm::vec3 position)
{
    this->width = width;
    this->height = height;
    Position = position;
    mode = FIRST_PERSON;
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
    // Compute the view matrix from the current position and orientation.
    viewMatrix = glm::lookAt(Position, Position + Orientation, Up);

    // Compute the projection matrix.
    projectionMatrix = glm::perspective(glm::radians(FOVdeg), static_cast<float>(width) / height, nearPlane, farPlane);

    // Optionally, compute a combined camera matrix.
    cameraMatrix = projectionMatrix * viewMatrix;
}

void Camera::Matrix(Shader& shader, const char* uniform)
{
    // This sends the combined cameraMatrix to the shader.
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::Inputs(GLFWwindow* window)
{
    if (mode == FIRST_PERSON)
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            Position += speed * Orientation;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            Position += speed * -glm::normalize(glm::cross(Orientation, Up));
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            Position += speed * -Orientation;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            Position += speed * glm::normalize(glm::cross(Orientation, Up));
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            Position += speed * Up;
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            Position += speed * -Up;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            speed = 0.4f;
        else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
            speed = 0.1f;
    }
    else if (mode == THIRD_PERSON)
    {
        if (attachedObject)
            target = *attachedObject; // Follow the attached object.
        Position = target + thirdPersonOffset;
        Orientation = glm::normalize(target - Position);
    }
    else if (mode == ORBITAL)
    {
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            if (firstClick)
            {
                glfwSetCursorPos(window, width / 2, height / 2);
                firstClick = false;
            }
            double mouseX, mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);
            float rotX = sensitivity * static_cast<float>(mouseY - (height / 2)) / height;
            float rotY = sensitivity * static_cast<float>(mouseX - (width / 2)) / width;
            orbitElevation += glm::radians(-rotX);
            orbitAzimuth += glm::radians(-rotY);
            float minEl = glm::radians(5.0f);
            float maxEl = glm::radians(175.0f);
            if (orbitElevation < minEl) orbitElevation = minEl;
            if (orbitElevation > maxEl) orbitElevation = maxEl;
            glfwSetCursorPos(window, width / 2, height / 2);
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            firstClick = true;
        }
        // Update position based on orbital parameters.
        Position.x = target.x + orbitRadius * sin(orbitElevation) * cos(orbitAzimuth);
        Position.y = target.y + orbitRadius * cos(orbitElevation);
        Position.z = target.z + orbitRadius * sin(orbitElevation) * sin(orbitAzimuth);
        Orientation = glm::normalize(target - Position);
    }
}
