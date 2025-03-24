#define M_PI 3.14159265358979323846

#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <cmath>
#include <cstddef>
#include <list>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Texture.h"
#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Camera.h"
#include "LightSource.h"
#include "Object.h"
#include "Program.h"

// Function prototypes
void Cleanup(Object& pyramid, Object& cube, Object& floor, Object& sphere, Object& lightCube,
    Texture& brickTex, Texture& sphereTex, Texture& floorTex,
    Shader& shaderProgram, Shader& lightShader, Shader& mirrorShader, GLFWwindow* window);

void generateSphere(float radius, unsigned int sectorCount, unsigned int stackCount,
    std::vector<float>& vertices, std::vector<unsigned int>& indices);
void HandleSpotlightChange(GLFWwindow* window, LightSource& spotLight);

std::tuple<Texture, Texture, Texture, Texture> SetupTextures(Shader& shaderProgram);

std::tuple<LightSource, LightSource, LightSource> SetupLightSources(Shader& shaderProgram);

void SetFogUniforms(Shader& shaderProgram, float time, Camera camera, LightSource spotLight);

template <size_t VSize, size_t ISize>
std::tuple<Object, VAO> SetupObject(GLfloat(&vertices)[VSize], GLuint(&indices)[ISize]);

template <size_t VSize>
std::tuple<Object, VAO> SetupObject(GLfloat(&vertices)[VSize]);

std::tuple<Object, VAO> SetupSphere(std::vector<float>& sphereVerts, std::vector<unsigned int>& sphereInds);

std::tuple<GLuint, GLuint> SetupMirrorTexture();

void generateTorus(float innerRadius, float outerRadius, unsigned int nsides, unsigned int nrings,
    std::vector<float>& vertices, std::vector<unsigned int>& indices);

std::tuple<Object, VAO> SetupTorus(std::vector<float>& torusVerts, std::vector<unsigned int>& torusInds);

// --- Geometry Data ---
// 
// Pyramid: 11 floats per vertex (position, color, texCoord, normal)
GLfloat pyramidVertices[] = {
    // Positions           Colors             TexCoords    Normals
    -0.5f, 0.0f,  0.5f,    0.83f,0.70f,0.44f,  0.0f, 0.0f,   0.0f, -1.0f, 0.0f,
    -0.5f, 0.0f, -0.5f,    0.83f,0.70f,0.44f,  0.0f, 5.0f,   0.0f, -1.0f, 0.0f,
     0.5f, 0.0f, -0.5f,    0.83f,0.70f,0.44f,  5.0f, 5.0f,   0.0f, -1.0f, 0.0f,
     0.5f, 0.0f,  0.5f,    0.83f,0.70f,0.44f,  5.0f, 0.0f,   0.0f, -1.0f, 0.0f,

    -0.5f, 0.0f,  0.5f,    0.83f,0.70f,0.44f,  0.0f, 0.0f,  -0.8f, 0.5f,  0.0f,
    -0.5f, 0.0f, -0.5f,    0.83f,0.70f,0.44f,  5.0f, 0.0f,  -0.8f, 0.5f,  0.0f,
     0.0f, 0.8f,  0.0f,    0.92f,0.86f,0.76f,  2.5f, 5.0f,  -0.8f, 0.5f,  0.0f,

    -0.5f, 0.0f, -0.5f,    0.83f,0.70f,0.44f,  5.0f, 0.0f,   0.0f, 0.5f, -0.8f,
     0.5f, 0.0f, -0.5f,    0.83f,0.70f,0.44f,  0.0f, 0.0f,   0.0f, 0.5f, -0.8f,
     0.0f, 0.8f,  0.0f,    0.92f,0.86f,0.76f,  2.5f, 5.0f,   0.0f, 0.5f, -0.8f,

     0.5f, 0.0f, -0.5f,    0.83f,0.70f,0.44f,  0.0f, 0.0f,   0.8f, 0.5f,  0.0f,
     0.5f, 0.0f,  0.5f,    0.83f,0.70f,0.44f,  5.0f, 0.0f,   0.8f, 0.5f,  0.0f,
     0.0f, 0.8f,  0.0f,    0.92f,0.86f,0.76f,  2.5f, 5.0f,   0.8f, 0.5f,  0.0f,

     0.5f, 0.0f,  0.5f,    0.83f,0.70f,0.44f,  5.0f, 0.0f,   0.0f, 0.5f,  0.8f,
    -0.5f, 0.0f,  0.5f,    0.83f,0.70f,0.44f,  0.0f, 0.0f,   0.0f, 0.5f,  0.8f,
     0.0f, 0.8f,  0.0f,    0.92f,0.86f,0.76f,  2.5f, 5.0f,   0.0f, 0.5f,  0.8f
};
GLuint pyramidIndices[] = {
    0, 1, 2,   0, 2, 3,
    4, 6, 5,
    7, 9, 8,
    10, 12, 11,
    13, 15, 14
};

// Cube vertices: 36 vertices with same layout as pyramid.
GLfloat cubeVertices[] = {
    // Back face
    1.0f, -0.5f, -0.5f,  1,0,0,   0.0f, 0.0f,    0,0,-1,
    2.0f,  0.5f, -0.5f,  1,0,0,   1.0f, 1.0f,    0,0,-1,
    2.0f, -0.5f, -0.5f,  1,0,0,   1.0f, 0.0f,    0,0,-1,
    2.0f,  0.5f, -0.5f,  1,0,0,   1.0f, 1.0f,    0,0,-1,
    1.0f, -0.5f, -0.5f,  1,0,0,   0.0f, 0.0f,    0,0,-1,
    1.0f,  0.5f, -0.5f,  1,0,0,   0.0f, 1.0f,    0,0,-1,
    // Front face
    1.0f, -0.5f,  0.5f,  0,1,0,   0.0f, 0.0f,    0,0,1,
    2.0f, -0.5f,  0.5f,  0,1,0,   1.0f, 0.0f,    0,0,1,
    2.0f,  0.5f,  0.5f,  0,1,0,   1.0f, 1.0f,    0,0,1,
    2.0f,  0.5f,  0.5f,  0,1,0,   1.0f, 1.0f,    0,0,1,
    1.0f,  0.5f,  0.5f,  0,1,0,   0.0f, 1.0f,    0,0,1,
    1.0f, -0.5f,  0.5f,  0,1,0,   0.0f, 0.0f,    0,0,1,
    // Left face
    1.0f,  0.5f,  0.5f,  0,0,1,   1.0f, 0.0f,   -1,0,0,
    1.0f,  0.5f, -0.5f,  0,0,1,   1.0f, 1.0f,   -1,0,0,
    1.0f, -0.5f, -0.5f,  0,0,1,   0.0f, 1.0f,   -1,0,0,
    1.0f, -0.5f, -0.5f,  0,0,1,   0.0f, 1.0f,   -1,0,0,
    1.0f, -0.5f,  0.5f,  0,0,1,   0.0f, 0.0f,   -1,0,0,
    1.0f,  0.5f,  0.5f,  0,0,1,   1.0f, 0.0f,   -1,0,0,
    // Right face
    2.0f,  0.5f,  0.5f,  1,1,0,   1.0f, 0.0f,    1,0,0,
    2.0f, -0.5f, -0.5f,  1,1,0,   0.0f, 1.0f,    1,0,0,
    2.0f,  0.5f, -0.5f,  1,1,0,   1.0f, 1.0f,    1,0,0,
    2.0f, -0.5f, -0.5f,  1,1,0,   0.0f, 1.0f,    1,0,0,
    2.0f,  0.5f,  0.5f,  1,1,0,   1.0f, 0.0f,    1,0,0,
    2.0f, -0.5f,  0.5f,  1,1,0,   0.0f, 0.0f,    1,0,0,
    // Bottom face
    1.0f, -0.5f, -0.5f,  1,0,1,   0.0f, 1.0f,    0,-1,0,
    2.0f, -0.5f, -0.5f,  1,0,1,   1.0f, 1.0f,    0,-1,0,
    2.0f, -0.5f,  0.5f,  1,0,1,   1.0f, 0.0f,    0,-1,0,
    2.0f, -0.5f,  0.5f,  1,0,1,   1.0f, 0.0f,    0,-1,0,
    1.0f, -0.5f,  0.5f,  1,0,1,   0.0f, 0.0f,    0,-1,0,
    1.0f, -0.5f, -0.5f,  1,0,1,   0.0f, 1.0f,    0,-1,0,
    // Top face
    1.0f,  0.5f, -0.5f,  0,1,1,   0.0f, 1.0f,    0,1,0,
    2.0f,  0.5f,  0.5f,  0,1,1,   1.0f, 0.0f,    0,1,0,
    2.0f,  0.5f, -0.5f,  0,1,1,   1.0f, 1.0f,    0,1,0,
    2.0f,  0.5f,  0.5f,  0,1,1,   1.0f, 0.0f,    0,1,0,
    1.0f,  0.5f, -0.5f,  0,1,1,   0.0f, 1.0f,    0,1,0,
    1.0f,  0.5f,  0.5f,  0,1,1,   0.0f, 0.0f,    0,1,0,
};

// Floor: a large quad (11 floats per vertex)
GLfloat floorVertices[] = {
    // Positions             Colors    TexCoords   Normals
    -10.0f, 0.0f,  10.0f,     1,1,1,    0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
     10.0f, 0.0f,  10.0f,     1,1,1,    1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
     10.0f, 0.0f, -10.0f,     1,1,1,    1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
    -10.0f, 0.0f, -10.0f,     1,1,1,    0.0f, 1.0f,  0.0f, 1.0f, 0.0f,
};
GLuint floorIndices[] = { 0, 1, 2, 0, 2, 3 };

// Light cube: a small cube.
GLfloat lightVertices[] = {
    -0.1f, -0.1f,  0.1f,
    -0.1f, -0.1f, -0.1f,
     0.1f, -0.1f, -0.1f,
     0.1f, -0.1f,  0.1f,
    -0.1f,  0.1f,  0.1f,
    -0.1f,  0.1f, -0.1f,
     0.1f,  0.1f, -0.1f,
     0.1f,  0.1f,  0.1f
};
GLuint lightIndices[] = {
    0, 2, 1,  0, 3, 2,  // Bottom face
    0, 7, 4,  0, 3, 7,  // Front face
    3, 6, 7,  3, 2, 6,  // Right face
    2, 5, 6,  2, 1, 5,  // Back face
    1, 4, 5,  1, 0, 4,  // Left face
    4, 6, 5,  4, 7, 6   // Top face
};


// Mirror
GLfloat mirrorVertices[] = {
    // Positions             Colors   TexCoords    Normals
    -1.0f, 0.0f, -3.0f,       1,1,1,   0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
     1.0f, 0.0f, -3.0f,       1,1,1,   1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
     1.0f, 2.0f, -3.0f,       1,1,1,   1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
    -1.0f, 2.0f, -3.0f,       1,1,1,   0.0f, 1.0f,  0.0f, 0.0f, 1.0f
};
GLuint mirrorIndices[] = { 0, 1, 2, 0, 2, 3 };

const unsigned int width = 800, height = 800;

int main()
{
    // Toggle for specular model
    static bool useBlinn = false;

    // Initialize GLFW and create window.
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(width, height, "YoutubeOpenGL", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    gladLoadGL();
    glViewport(0, 0, width, height);

    // Build shaders.
    Shader shaderProgram("default.vert", "default.frag");
    Shader lightShader("light.vert", "light.frag");
    Shader mirrorShader("mirror.vert", "mirror.frag");

    // --- Set Up Geometry Objects ---
    // Pyramid
    auto [pyramid, pyramidVAO] = SetupObject(pyramidVertices, pyramidIndices);

    // Moving Cube
    auto [cube, cubeVAO] = SetupObject(cubeVertices);

    // Floor
    auto [floor, floorVAO] = SetupObject(floorVertices, floorIndices);

    // Rotating Sphere
    std::vector<float> sphereVerts;
    std::vector<unsigned int> sphereInds;
    auto [sphere, sphereVAO] = SetupSphere(sphereVerts, sphereInds);

    // Light Cube
    auto [lightCube, lightVAO] = SetupObject(lightVertices, lightIndices);

    // Mirror
    auto [mirror, mirrorVAO] = SetupObject(mirrorVertices, mirrorIndices);

    // Set Up Textures
    auto [brickTex, floorTex, sphereTex, torrusTex] = SetupTextures(shaderProgram);

    // Set Up Lights 
    auto [fixedLight, spotLight, dirLight] = SetupLightSources(shaderProgram);

    // Create a texture and framebuffer for mirror reflection.
    auto [reflectionTexture, reflectionFBO] = SetupMirrorTexture();

    // Rotating Torus
    std::vector<float> torusVerts;
    std::vector<unsigned int> torusInds;
    auto [torus, torusVAO] = SetupTorus(torusVerts, torusInds);
    
    // Reflection matrix for mirror plane (plane z = -3).
    glm::mat4 reflectionMatrix = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -6)), glm::vec3(1, 1, -1));

    // Camera Setup 
    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));
    camera.target = glm::vec3(0.0f);

    glEnable(GL_DEPTH_TEST);

    // Main Render Loop 
    while (!glfwWindowShouldClose(window))
    {
        float time = glfwGetTime();

        // Toggle specular model
        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
        {
            useBlinn = !useBlinn;
        }

        // --- Update Camera & Input ---
        camera.HandleModes(window);
        camera.Inputs(window);
        camera.updateMatrix(45.0f, 0.1f, 100.0f);

        // Clear buffers.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // Update cube position and spotlight.
        glm::vec3 cubePos(sinf(time) * 3.0f, 0.5f, 1.0f);
        camera.attachedObject = &cubePos;
        dirLight.color = glm::vec3(1.0f, 1.0f, std::max(sin(time / 10), 0.0f));
        dirLight.setUniforms(shaderProgram, "dirLight");
        fixedLight.setUniforms(shaderProgram, "fixedLight");
        shaderProgram.setVec3("spotLight.position", spotLight.position);
        HandleSpotlightChange(window, spotLight);

        // --- Render Main Scene ---
        shaderProgram.Activate();
        shaderProgram.setBool("useBlinn", useBlinn);
        SetFogUniforms(shaderProgram, time, camera, spotLight);

        // Draw Pyramid.
        glm::mat4 pyramidModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));
        pyramid.SetTexture(brickTex);
        pyramid.Draw(glm::vec3(0.0f), shaderProgram, sizeof(pyramidIndices) / sizeof(GLuint));

        // Draw Cube.
        glm::mat4 cubeModel = glm::translate(glm::mat4(1.0f), cubePos);
        cube.Draw(cubePos, shaderProgram, 0, 36);

        // Update spotlight attached to cube.
        glm::vec3 localOffset(0.2f, 0.2f, 0.0f);
        glm::vec3 spotlightWorldPos = glm::vec3(cubeModel * glm::vec4(localOffset, 1.0f));
        spotLight.position = spotlightWorldPos;
        spotLight.setUniforms(shaderProgram, "spotLight");
        shaderProgram.setVec3("spotLight.position", spotLight.position);
        shaderProgram.setVec3("spotLight.direction", spotLight.direction);

        // Draw Rotating Sphere.
        glm::mat4 sphereModel = glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 1.0f, 0.0f)),
            time, glm::vec3(0.0f, 1.0f, 0.0f));
        shaderProgram.setMatrix4("model", sphereModel);
        sphereTex.Bind();
        sphereVAO.Bind();
        glDrawElements(GL_TRIANGLES, sphereInds.size(), GL_UNSIGNED_INT, 0);

        // Draw Floor.
        glm::mat4 floorModel = glm::mat4(1.0f);
        floor.SetTexture(floorTex);
        floor.Draw(glm::vec3(0.0f), shaderProgram, 6);

        // Render Light Cube
        glm::vec3 lightCubePos(3.5f, 1.5f, 5.5f);
        glm::mat4 lightModel = glm::translate(glm::mat4(1.0f), lightCubePos);
        shaderProgram.setMatrix4("model", lightModel);
        shaderProgram.setMatrix4("camMatrix", camera.cameraMatrix);
        shaderProgram.setVec4("lightColor", glm::vec4(1.0f));
        lightVAO.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

        // Render torus
        glm::mat4 torusModel = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.5f, -2.0f));
        torusModel = glm::rotate(torusModel, time, glm::vec3(0.0f, 1.0f, 0.0f));
        shaderProgram.setMatrix4("model", torusModel);
        torusVAO.Bind();
        torrusTex.Bind();
        glDrawElements(GL_TRIANGLES, torusInds.size(), GL_UNSIGNED_INT, 0);

        // Render Mirror Reflection
        // Reflection texture
        glBindFramebuffer(GL_FRAMEBUFFER, reflectionFBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 reflectionViewMatrix = camera.viewMatrix * reflectionMatrix;
        shaderProgram.setMatrix4("view", reflectionViewMatrix);
        shaderProgram.setMatrix4("projection", camera.projectionMatrix);
        brickTex.Bind();
        pyramidVAO.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(pyramidIndices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
        cubeVAO.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Mark mirror area in stencil buffer.
        glEnable(GL_STENCIL_TEST);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glDepthMask(GL_FALSE);
        shaderProgram.Activate();
        glm::mat4 mirrorModel = glm::mat4(1.0f);
        shaderProgram.setMatrix4("model", mirrorModel);
        mirrorVAO.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glDepthMask(GL_TRUE);

        // Render reflection only in mirror region
        glStencilFunc(GL_EQUAL, 1, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        shaderProgram.Activate();
        shaderProgram.setMatrix4("view", camera.viewMatrix * reflectionMatrix);
        glm::vec3 reflectedCameraPos(camera.Position.x, camera.Position.y, -camera.Position.z - 6);
        shaderProgram.setVec3("cameraPos", reflectedCameraPos);

        // Render reflected Pyramid
        pyramidModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));
        pyramidModel = reflectionMatrix * pyramidModel;
        mirrorShader.setMatrix4("model", pyramidModel);
        brickTex.Bind();
        pyramidVAO.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(pyramidIndices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

        // Render reflected Cube
        cube.Draw(cubePos, shaderProgram, 0, 36);

        // Render reflected Sphere
        sphereModel = glm::rotate(glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 1.0f, 0.0f)),
            time, glm::vec3(0.0f, 1.0f, 0.0f));
        shaderProgram.setMatrix4("model", sphereModel);
        sphereVAO.Bind();
        sphereTex.Bind();
        glDrawElements(GL_TRIANGLES, sphereInds.size(), GL_UNSIGNED_INT, 0);

        // Render reflected Torus
        torusModel = glm::rotate(
            glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.5f, -2.0f)),
            time, glm::vec3(0.0f, 1.0f, 0.0f)
        );
        shaderProgram.setMatrix4("model", torusModel);
        torusVAO.Bind();
        torrusTex.Bind();
        glDrawElements(GL_TRIANGLES, torusInds.size(), GL_UNSIGNED_INT, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_STENCIL_TEST);

        // Draw the mirror surface with reflection texture.
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        mirrorShader.Activate();
        mirrorShader.setMatrix4("view", camera.viewMatrix);
        mirrorShader.setMatrix4("projection", camera.projectionMatrix);
        mirrorShader.setMatrix4("model", mirrorModel);
        mirrorShader.setVec4("mirrorColor", glm::vec4(1.0f, 1.0f, 1.0f, 0.3f));
        mirrorShader.setInt("reflectionTexture", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, reflectionTexture);
        mirrorVAO.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDisable(GL_BLEND);

        // Swap buffers and poll events.
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    Cleanup(pyramid, cube, floor, sphere, lightCube,
        brickTex, sphereTex, floorTex,
        shaderProgram, lightShader, mirrorShader, window);
    return 0;
}

void Cleanup(Object& pyramid, Object& cube, Object& floor, Object& sphere, Object& lightCube,
    Texture& brickTex, Texture& sphereTex, Texture& floorTex,
    Shader& shaderProgram, Shader& lightShader, Shader& mirrorShader, GLFWwindow* window)
{
    // Delete objects and textures.
    pyramid.Delete();
    cube.Delete();
    floor.Delete();
    sphere.Delete();
    lightCube.Delete();
    brickTex.Delete();
    floorTex.Delete();
    sphereTex.Delete();
    shaderProgram.Delete();
    lightShader.Delete();
    mirrorShader.Delete();
    glfwDestroyWindow(window);
    glfwTerminate();
}

void generateSphere(float radius, unsigned int sectorCount, unsigned int stackCount,
    std::vector<float>& vertices, std::vector<unsigned int>& indices)
{
    vertices.clear();
    indices.clear();
    float lengthInv = 1.0f / radius;
    float sectorStep = 2 * glm::pi<float>() / sectorCount;
    float stackStep = glm::pi<float>() / stackCount;

    // Generate vertices.
    for (unsigned int i = 0; i <= stackCount; ++i)
    {
        float stackAngle = glm::pi<float>() / 2 - i * stackStep;
        float xy = radius * cosf(stackAngle);
        float z = radius * sinf(stackAngle);

        for (unsigned int j = 0; j <= sectorCount; ++j)
        {
            float sectorAngle = j * sectorStep;
            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);
            float s = (float)j / sectorCount;
            float t = (float)i / stackCount;
            
            // Position
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
            
            // Color (white)
            vertices.push_back(1.0f);
            vertices.push_back(1.0f);
            vertices.push_back(1.0f);

            // Texture coordinates
            vertices.push_back(s);
            vertices.push_back(t);
            
            // Normal
            vertices.push_back(x * lengthInv);
            vertices.push_back(y * lengthInv);
            vertices.push_back(z * lengthInv);
        }
    }

    // Generate indices.
    unsigned int k1, k2;
    for (unsigned int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);
        k2 = k1 + sectorCount + 1;
        for (unsigned int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            if (i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }
            if (i != (stackCount - 1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
}

void HandleSpotlightChange(GLFWwindow* window, LightSource& spotLight)
{
    float angleDelta = 0.02f;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        spotLight.direction = glm::rotateY(spotLight.direction, angleDelta);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        spotLight.direction = glm::rotateY(spotLight.direction, -angleDelta);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        glm::vec3 right = glm::normalize(glm::cross(spotLight.direction, glm::vec3(0.0f, 1.0f, 0.0f)));
        spotLight.direction = glm::rotate(spotLight.direction, angleDelta, right);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        glm::vec3 right = glm::normalize(glm::cross(spotLight.direction, glm::vec3(0.0f, 1.0f, 0.0f)));
        spotLight.direction = glm::rotate(spotLight.direction, -angleDelta, right);
    }
    spotLight.direction = glm::normalize(spotLight.direction);
}

std::tuple<Texture, Texture, Texture, Texture> SetupTextures(Shader& shaderProgram)
{
    Texture brickTex("brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    brickTex.texUnit(shaderProgram, "tex0", 0);
    Texture floorTex("wood_texture.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    floorTex.texUnit(shaderProgram, "tex0", 0);
    Texture sphereTex("brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    sphereTex.texUnit(shaderProgram, "tex0", 0);
    Texture torrusTex("torrus.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    torrusTex.texUnit(shaderProgram, "tex0", 0);

    return { brickTex, floorTex, sphereTex, torrusTex };
}

std::tuple<LightSource, LightSource, LightSource> SetupLightSources(Shader& shaderProgram)
{
    LightSource fixedLight(POINT_LIGHT, glm::vec3(0.5f, 0.5f, 6.5f), glm::vec3(1.0f, 0.3f, 0.3f));
    LightSource spotLight(SPOT_LIGHT, glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(1.0f, 1.0f, 0.8f));
    LightSource dirLight(DIRECTIONAL_LIGHT, glm::vec3(5.0f, 5.0f, 5.0f), glm::vec3(1.0f, 1.0f, 0.0f));
    dirLight.direction = glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f));

    shaderProgram.setVec3("fixedLight.position", fixedLight.position);
    shaderProgram.setVec3("fixedLight.color", fixedLight.color);
    shaderProgram.setVec3("spotLight.color", spotLight.color);
    shaderProgram.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    shaderProgram.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
    shaderProgram.setVec3("dirLight.direction", dirLight.direction);
    shaderProgram.setVec3("dirLight.color", dirLight.color);
    shaderProgram.setFloat("fixedLight.cutOff", 0.0f);
    shaderProgram.setFloat("dirLight.cutOff", 0.0f);
    return { fixedLight, spotLight, dirLight };
}

void SetFogUniforms(Shader& shaderProgram, float time, Camera camera, LightSource spotLight)
{
    shaderProgram.setVec3("fogColor", glm::vec3(
        0.5f + 0.5f * cos(time / 10),
        0.5f + 0.5f * cos(time / 10),
        0.5f + 0.5f * cos(time / 10)));
    shaderProgram.setFloat("fogStart", -4.0f);
    shaderProgram.setFloat("fogEnd", 1.0f + 20 * cos(time));
    shaderProgram.setVec3("cameraPos", camera.Position);
    shaderProgram.setVec3("spotLight.direction", spotLight.direction);
    shaderProgram.setMatrix4("view", camera.viewMatrix);
    shaderProgram.setMatrix4("projection", camera.projectionMatrix);
}

template <size_t VSize, size_t ISize>
std::tuple<Object, VAO> SetupObject(GLfloat(&vertices)[VSize], GLuint(&indices)[ISize])
{
    VAO vao;
    vao.Bind();
    VBO vbo(vertices, VSize * sizeof(GLfloat));
    EBO ebo(indices, ISize * sizeof(GLuint));
    Object obj(vao, vbo);

    obj.LinkAttributes();
    obj.SetEBO(ebo);
    obj.Unbind();

    return { obj, vao };
}


template <size_t VSize>
std::tuple<Object, VAO> SetupObject(GLfloat(&vertices)[VSize])
{
    VAO vao; 
    vao.Bind();
    VBO vbo(vertices, sizeof(vertices));
    Object obj(vao, vbo);
    obj.LinkAttributes();
    obj.Unbind();

    return { obj, vao };
}


std::tuple<Object, VAO> SetupSphere(std::vector<float>& sphereVerts, std::vector<unsigned int>& sphereInds)
{
    generateSphere(0.5f, 36, 18, sphereVerts, sphereInds);
    VAO sphereVAO; sphereVAO.Bind();
    VBO sphereVBO(sphereVerts.data(), sphereVerts.size() * sizeof(float));
    EBO sphereEBO(sphereInds.data(), sphereInds.size() * sizeof(unsigned int));
    Object sphere(sphereVAO, sphereVBO);
    sphere.LinkAttributes();
    sphere.Unbind();
    return { sphere, sphereVAO };
}

std::tuple<GLuint, GLuint> SetupMirrorTexture()
{
    GLuint reflectionTexture;
    glGenTextures(1, &reflectionTexture);
    glBindTexture(GL_TEXTURE_2D, reflectionTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Reflection framebuffer.
    GLuint reflectionFBO;
    glGenFramebuffers(1, &reflectionFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, reflectionFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, reflectionTexture, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    return { reflectionTexture, reflectionFBO };
}

std::tuple<
    std::list<std::tuple<Object, VAO>>, 
    std::list<LightSource>, 
    std::list<Texture>,
    GLuint, GLuint
> Setup(Shader& shaderProgram)
{
    std::list<std::tuple<Object,VAO>> ans = {};
    std::list<Texture> texs = {};
    std::list<LightSource> sources = {};

    // --- Set Up Geometry Objects ---
    // Pyramid
    auto [pyramid, pyramidVAO] = SetupObject(pyramidVertices, pyramidIndices);
    ans.push_back({ pyramid, pyramidVAO });

    // Moving Cube
    auto [cube, cubeVAO] = SetupObject(cubeVertices);
    ans.push_back({ cube, cubeVAO });

    // Floor
    auto [floor, floorVAO] = SetupObject(floorVertices, floorIndices);
    ans.push_back({ floor, floorVAO });

    // Rotating Sphere
    std::vector<float> sphereVerts;
    std::vector<unsigned int> sphereInds;
    auto [sphere, sphereVAO] = SetupSphere(sphereVerts, sphereInds);
    ans.push_back({ sphere, sphereVAO });

    // Light Cube
    auto [lightCube, lightVAO] = SetupObject(lightVertices, lightIndices);
    ans.push_back({ lightCube, lightVAO });

    // Mirror
    auto [mirror, mirrorVAO] = SetupObject(mirrorVertices, mirrorIndices);
    ans.push_back({ mirror, mirrorVAO });

    // --- Set Up Textures ---
    auto [brickTex, floorTex, sphereTex, torusTex] = SetupTextures(shaderProgram);
    texs.push_back(brickTex);
    texs.push_back(floorTex);
    texs.push_back(sphereTex);

    // --- Set Up Lights ---
    auto [fixedLight, spotLight, dirLight] = SetupLightSources(shaderProgram);
    sources.push_back(fixedLight);
    sources.push_back(spotLight);
    sources.push_back(dirLight);

    // Create a texture and framebuffer for mirror reflection.
    auto [reflectionTexture, reflectionFBO] = SetupMirrorTexture();
    return { ans, sources, texs, reflectionTexture, reflectionFBO };
}

void generateTorus(float innerRadius, float outerRadius, unsigned int nsides, unsigned int nrings,
    std::vector<float>& vertices, std::vector<unsigned int>& indices)
{
    vertices.clear();
    indices.clear();

    float ringFactor = 2.0f * glm::pi<float>() / nrings;
    float sideFactor = 2.0f * glm::pi<float>() / nsides;
    float offsetY = 0.5f;  

    // Generate vertices.
    for (unsigned int ring = 0; ring <= nrings; ++ring)
    {
        float u = ring * ringFactor;
        float cosU = cos(u);
        float sinU = sin(u);
        for (unsigned int side = 0; side <= nsides; ++side)
        {
            float v = side * sideFactor;
            float cosV = cos(v);
            float sinV = sin(v);

            // The distance from the center of the torus tube to the current point.
            float r = outerRadius + innerRadius * cosV;
            float x = r * cosU;
            float y = r * sinU + offsetY;  // add the offset here
            float z = innerRadius * sinV;

            // Position.
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // Color (choose a pleasant light blue color).
            vertices.push_back(0.3f);
            vertices.push_back(0.7f);
            vertices.push_back(0.9f);

            // Texture coordinates.
            vertices.push_back((float)ring / nrings);
            vertices.push_back((float)side / nsides);

            // Compute normal.
            // The center of the tube for this ring is at (outerRadius*cosU, outerRadius*sinU, 0), then add the same offset.
            float cx = outerRadius * cosU;
            float cy = outerRadius * sinU + offsetY;
            float cz = 0.0f;
            glm::vec3 normal = glm::normalize(glm::vec3(x - cx, y - cy, z - cz));
            vertices.push_back(normal.x);
            vertices.push_back(normal.y);
            vertices.push_back(normal.z);
        }
    }

    // Generate indices.
    for (unsigned int ring = 0; ring < nrings; ++ring)
    {
        unsigned int ringStart = ring * (nsides + 1);
        unsigned int nextRingStart = (ring + 1) * (nsides + 1);
        for (unsigned int side = 0; side < nsides; ++side)
        {
            unsigned int current = ringStart + side;
            unsigned int next = ringStart + side + 1;
            unsigned int currentNextRing = nextRingStart + side;
            unsigned int nextNextRing = nextRingStart + side + 1;
            // Two triangles per quad.
            indices.push_back(current);
            indices.push_back(currentNextRing);
            indices.push_back(next);

            indices.push_back(next);
            indices.push_back(currentNextRing);
            indices.push_back(nextNextRing);
        }
    }
}


std::tuple<Object, VAO> SetupTorus(std::vector<float>& torusVerts, std::vector<unsigned int>& torusInds)
{
    generateTorus(0.2f, 0.5f, 24, 24, torusVerts, torusInds);
    VAO torusVAO;
    torusVAO.Bind();
    VBO torusVBO(torusVerts.data(), torusVerts.size() * sizeof(float));
    EBO torusEBO(torusInds.data(), torusInds.size() * sizeof(unsigned int));
    Object torus(torusVAO, torusVBO);
    torus.LinkAttributes();
    torus.SetEBO(torusEBO);
    torus.Unbind();
    return { torus, torusVAO };
}

