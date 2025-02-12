#define M_PI           3.14159265358979323846
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "Texture.h"
#include "Shader.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Camera.h"
#include "LightSource.h"
#include "Object.h"
#include "Program.h"


// --- Utility: Sphere Generation Function ---
void generateSphere(float radius, unsigned int sectorCount, unsigned int stackCount,
    std::vector<float>& vertices, std::vector<unsigned int>& indices)
{
    vertices.clear();
    indices.clear();

    float x, y, z, xy;      // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius;  // normal
    float s, t;             // texture coordinate

    float sectorStep = 2 * glm::pi<float>() / sectorCount;
    float stackStep = glm::pi<float>() / stackCount;
    float sectorAngle, stackAngle;

    for (unsigned int i = 0; i <= stackCount; ++i)
    {
        stackAngle = glm::pi<float>() / 2 - i * stackStep; // from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);
        z = radius * sinf(stackAngle);

        for (unsigned int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;
            x = xy * cosf(sectorAngle);
            y = xy * sinf(sectorAngle);
            // Position
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
            // Color (white for now)
            vertices.push_back(1.0f);
            vertices.push_back(1.0f);
            vertices.push_back(1.0f);
            // Texture coordinates
            s = (float)j / sectorCount;
            t = (float)i / stackCount;
            vertices.push_back(s);
            vertices.push_back(t);
            // Normal (normalized position)
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            vertices.push_back(nx);
            vertices.push_back(ny);
            vertices.push_back(nz);
        }
    }

    // Build indices.
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

// --- Geometry Data --- //

// Pyramid vertices (11 floats per vertex: position, color, texCoord, normal)
GLfloat pyramidVertices[] =
{ //  POSITION             COLOR              TexCoord         NORMAL
    -0.5f, 0.0f,  0.5f,      0.83f,0.70f,0.44f,   0.0f, 0.0f,      0.0f, -1.0f, 0.0f,
    -0.5f, 0.0f, -0.5f,      0.83f,0.70f,0.44f,   0.0f, 5.0f,      0.0f, -1.0f, 0.0f,
     0.5f, 0.0f, -0.5f,      0.83f,0.70f,0.44f,   5.0f, 5.0f,      0.0f, -1.0f, 0.0f,
     0.5f, 0.0f,  0.5f,      0.83f,0.70f,0.44f,   5.0f, 0.0f,      0.0f, -1.0f, 0.0f,

    -0.5f, 0.0f,  0.5f,      0.83f,0.70f,0.44f,   0.0f, 0.0f,     -0.8f, 0.5f,  0.0f,
    -0.5f, 0.0f, -0.5f,      0.83f,0.70f,0.44f,   5.0f, 0.0f,     -0.8f, 0.5f,  0.0f,
     0.0f, 0.8f,  0.0f,      0.92f,0.86f,0.76f,   2.5f, 5.0f,     -0.8f, 0.5f,  0.0f,

    -0.5f, 0.0f, -0.5f,      0.83f,0.70f,0.44f,   5.0f, 0.0f,      0.0f, 0.5f, -0.8f,
     0.5f, 0.0f, -0.5f,      0.83f,0.70f,0.44f,   0.0f, 0.0f,      0.0f, 0.5f, -0.8f,
     0.0f, 0.8f,  0.0f,      0.92f,0.86f,0.76f,   2.5f, 5.0f,      0.0f, 0.5f, -0.8f,

     0.5f, 0.0f, -0.5f,      0.83f,0.70f,0.44f,   0.0f, 0.0f,      0.8f, 0.5f,  0.0f,
     0.5f, 0.0f,  0.5f,      0.83f,0.70f,0.44f,   5.0f, 0.0f,      0.8f, 0.5f,  0.0f,
     0.0f, 0.8f,  0.0f,      0.92f,0.86f,0.76f,   2.5f, 5.0f,      0.8f, 0.5f,  0.0f,

     0.5f, 0.0f,  0.5f,      0.83f,0.70f,0.44f,   5.0f, 0.0f,      0.0f, 0.5f,  0.8f,
    -0.5f, 0.0f,  0.5f,      0.83f,0.70f,0.44f,   0.0f, 0.0f,      0.0f, 0.5f,  0.8f,
     0.0f, 0.8f,  0.0f,      0.92f,0.86f,0.76f,   2.5f, 5.0f,      0.0f, 0.5f,  0.8f
};
GLuint pyramidIndices[] =
{
    0, 1, 2,  0, 2, 3,
    4, 6, 5,
    7, 9, 8,
    10, 12, 11,
    13, 15, 14
};

// Moving Cube (36 vertices; same layout as pyramid)
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

// Floor (a large quad with 11 floats per vertex)
GLfloat floorVertices[] = {
    // positions              // colors       // texCoords   // normals
    -10.0f, 0.0f,  10.0f,      1,1,1,         0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
     10.0f, 0.0f,  10.0f,      1,1,1,         1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
     10.0f, 0.0f, -10.0f,      1,1,1,         1.0f, 1.0f,   0.0f, 1.0f, 0.0f,
    -10.0f, 0.0f, -10.0f,      1,1,1,         0.0f, 1.0f,   0.0f, 1.0f, 0.0f,
};
GLuint floorIndices[] = {
    0,1,2,
    0,2,3
};

// Light cube geometry (a small cube)
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
    0,1,2,  0,2,3,
    0,4,7,  0,7,3,
    3,7,6,  3,6,2,
    2,6,5,  2,5,1,
    1,5,4,  1,4,0,
    4,5,6,  4,6,7
};

GLfloat mirrorVertices[] = {
    // positions            // colors   // texCoords  // normals
    -1.0f, 0.0f, -3.0f,      1,1,1,     0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
     1.0f, 0.0f, -3.0f,      1,1,1,     1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
     1.0f, 2.0f, -3.0f,      1,1,1,     1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
    -1.0f, 2.0f, -3.0f,      1,1,1,     0.0f, 1.0f,  0.0f, 0.0f, 1.0f
};
GLuint mirrorIndices[] = {
    0,1,2,
    0,2,3
};

Object ObjectSetup(GLfloat vertices[])
{
    VAO ObjectVAO;
    ObjectVAO.Bind();
    VBO ObjectVBO(vertices, sizeof(vertices));
    Object Obj(ObjectVAO, ObjectVBO);
    Obj.LinkAttributes();
    Obj.Unbind();
    return Obj;
}

Object ObjectSetup(GLfloat vertices[], GLuint indices[])
{
    VAO ObjectVAO;
    ObjectVAO.Bind();
    VBO ObjectVBO(vertices, sizeof(vertices));
    EBO ObjectEBO(indices, sizeof(indices));
    Object Obj(ObjectVAO, ObjectVBO);
    Obj.LinkAttributes();
    Obj.SetEBO(ObjectEBO);
    Obj.Unbind();
    return Obj;
}

const unsigned int width = 800, height = 800;

int initialize()
{
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
}
int main()
{
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

    // --- Set Up VAOs, VBOs, EBOs --- //

    // Pyramid
    VAO pyramidVAO;
    pyramidVAO.Bind();
    VBO pyramidVBO(pyramidVertices, sizeof(pyramidVertices));
    EBO pyramidEBO(pyramidIndices, sizeof(pyramidIndices));
    Object pyramid(pyramidVAO, pyramidVBO);
    pyramid.LinkAttributes();
    pyramid.SetEBO(pyramidEBO);
    pyramid.Unbind();

    // Moving Cube
    VAO cubeVAO;
    cubeVAO.Bind();
    VBO cubeVBO(cubeVertices, sizeof(cubeVertices));
    Object cube(cubeVAO, cubeVBO);
    cube.LinkAttributes();
    cube.Unbind();

    // Floor
    VAO floorVAO;
    floorVAO.Bind();
    VBO floorVBO(floorVertices, sizeof(floorVertices));
    EBO floorEBO(floorIndices, sizeof(floorIndices));
    Object floor(floorVAO, floorVBO);
    floor.LinkAttributes();
    floor.Unbind();

    // Rotating Sphere
    std::vector<float> sphereVerts;
    std::vector<unsigned int> sphereInds;
    generateSphere(0.5f, 36, 18, sphereVerts, sphereInds);
    VAO sphereVAO;
    sphereVAO.Bind();
    VBO sphereVBO(sphereVerts.data(), sphereVerts.size() * sizeof(float));
    EBO sphereEBO(sphereInds.data(), sphereInds.size() * sizeof(unsigned int));

    Object sphere(sphereVAO, sphereVBO);
    sphere.LinkAttributes();
    sphere.Unbind();

    // Light Cube
    VAO lightVAO;
    lightVAO.Bind();
    VBO lightVBO(lightVertices, sizeof(lightVertices));
    EBO lightEBO(lightIndices, sizeof(lightIndices));
    Object lightCube(lightVAO, lightVBO);
    lightCube.Unbind();

    // Mirror
    VAO mirrorVAO;
    mirrorVAO.Bind();
    VBO mirrorVBO(mirrorVertices, sizeof(mirrorVertices));
    EBO mirrorEBO(mirrorIndices, sizeof(mirrorIndices));

    Object mirror(mirrorVAO, mirrorVBO);
    mirror.LinkAttributes();

    mirrorVAO.Unbind();
    mirrorVBO.Unbind();
    mirrorEBO.Unbind();

    // --- Set Up Textures --- //
    Texture brickTex("D:\\andrz\\Downloads\\brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    brickTex.texUnit(shaderProgram, "tex0", 0); 

    Texture floorTex("D:\\andrz\\Downloads\\wood_texture.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    floorTex.texUnit(shaderProgram, "tex0", 0);

    Texture sphereTex("D:\\andrz\\Downloads\\shrek.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    sphereTex.texUnit(shaderProgram, "tex0", 0);

    // --- Set Up Lights --- //
    // (Light uniforms would be set in the main shader if lighting calculations were performed.)
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

    // Create a texture for storing the reflection
    GLuint reflectionTexture;
    glGenTextures(1, &reflectionTexture);
    glBindTexture(GL_TEXTURE_2D, reflectionTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Create a framebuffer to render to the reflection texture
    // Compute a reflection matrix for the mirror plane (here: plane z = -3).
    glm::mat4 reflectionMatrix = glm::scale(glm::translate(
        glm::mat4(1.0f),
        glm::vec3(0, 0, -6)),
        glm::vec3(1, 1, -1)
    );
    GLuint reflectionFBO;
    glGenFramebuffers(1, &reflectionFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, reflectionFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, reflectionTexture, 0);

    // Check if framebuffer is correctly set up
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // --- Set Up Transformations --- //
    glm::vec3 pyramidPos = glm::vec3(0.0f);  // Pyramid centered at origin

    // --- Camera Setup --- //
    Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));
    camera.target = glm::vec3(0.0f); // For third-person/orbital modes

    glEnable(GL_DEPTH_TEST);

    // ---------------------------------------------------------------------------
    // Main Render Loop
    while (!glfwWindowShouldClose(window))
    {
        float time = glfwGetTime();

        // --- Update Input & Camera ---
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
            camera.mode = FIRST_PERSON;
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
            camera.mode = THIRD_PERSON;
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
            camera.mode = ORBITAL;

        // Clear the color, depth, and stencil buffers.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // Update camera input and compute matrices.
        camera.Inputs(window);
        camera.updateMatrix(45.0f, 0.1f, 100.0f);

        // --- Update Moving Object (Cube) and Spotlight ---
        glm::vec3 cubePos = glm::vec3(sinf(glfwGetTime()) * 3.0f, 0.5f, 1.0f);
        camera.attachedObject = &cubePos;

        dirLight.color = glm::vec3(1.0f, 1.0f, 0.0f + sin(time / 10));
        dirLight.setUniforms(shaderProgram, "dirLight");
        fixedLight.setUniforms(shaderProgram, "fixedLight");
        shaderProgram.setVec3("spotLight.position", spotLight.position);
        
        float angleDelta = 0.02f;

        // Handle spotlight direction change
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            spotLight.direction = glm::rotateY(spotLight.direction, angleDelta);
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            spotLight.direction = glm::rotateY(spotLight.direction, -angleDelta);
        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        {
            glm::vec3 right = glm::vec3(0.0f, 1.0f, 0.0f);
            right = glm::normalize(glm::cross(spotLight.direction, right));
            spotLight.direction = glm::rotate(spotLight.direction, angleDelta, right);
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            glm::vec3 right = glm::vec3(0.0f, 1.0f, 0.0f);
            right = glm::normalize(glm::cross(spotLight.direction, right));
            spotLight.direction = glm::rotate(spotLight.direction, -angleDelta, right);
        }

        spotLight.direction = glm::normalize(spotLight.direction);

        // --- Render the Main Scene (Pyramid, Cube, Sphere, Floor) ---
        shaderProgram.Activate();
        
        // Set fog uniforms 
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
        
        // Draw Pyramid.
        glm::mat4 pyramidModel = glm::mat4(1.0f);
        pyramidModel = glm::translate(pyramidModel, pyramidPos);
        shaderProgram.setMatrix4("model", pyramidModel);
        brickTex.Bind();
        pyramidVAO.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(pyramidIndices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
        
        // Draw Moving Cube.
        glm::mat4 cubeModel = glm::mat4(1.0f);
        cubeModel = glm::translate(cubeModel, cubePos);
        shaderProgram.setMatrix4("model", cubeModel);
        cubeVAO.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Attach spotlight to the cube
        glm::vec3 localOffset(0.2f, 0.2f, 0.0f); 
        glm::vec3 spotlightWorldPos = glm::vec3(cubeModel * glm::vec4(localOffset, 1.0f));
        spotLight.position = spotlightWorldPos;
        spotLight.setUniforms(shaderProgram, "spotLight");
        shaderProgram.setVec3("spotLight.position", spotLight.position);
        shaderProgram.setVec3("spotLight.direction", spotLight.direction);
        
        // Draw Rotating Sphere.
        glm::mat4 sphereModel = glm::mat4(1.0f);
        sphereModel = glm::translate(sphereModel, glm::vec3(2.0f, 1.0f, 0.0f));
        sphereModel = glm::rotate(sphereModel, time, glm::vec3(0.0f, 1.0f, 0.0f));
        shaderProgram.setMatrix4("model", sphereModel);
        sphereTex.Bind();
        sphereVAO.Bind();
        glDrawElements(GL_TRIANGLES, sphereInds.size(), GL_UNSIGNED_INT, 0);
        
        // Draw Floor.
        glm::mat4 floorModel = glm::mat4(1.0f);
        shaderProgram.setMatrix4("model", floorModel);
        floorTex.Bind();
        floorVAO.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        

        // --- Render the Light Cube ---
        glm::vec3 lightCubePos = glm::vec3(3.5f, 1.5f, 5.5f);
        glm::mat4 lightModel = glm::mat4(1.0f);
        lightModel = glm::translate(lightModel, lightCubePos);
        shaderProgram.setMatrix4("model", lightModel);
        // Assume "camMatrix" uniform is handled by camera.Matrix() if needed.
        shaderProgram.setMatrix4("camMatrix", camera.cameraMatrix);
        shaderProgram.setVec4("lightColor", glm::vec4(1.0f));
        lightVAO.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
        

        // --- Render Mirror Reflection Using the Stencil Buffer ---

        // Render the scene to the reflection texture
        glBindFramebuffer(GL_FRAMEBUFFER, reflectionFBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set up the reflection camera view (adjust the view matrix for reflection)
        glm::mat4 reflectionViewMatrix = camera.viewMatrix * reflectionMatrix;
        shaderProgram.setMatrix4("view", reflectionViewMatrix);
        shaderProgram.setMatrix4("projection", camera.projectionMatrix);

        // Render all objects in the scene (the objects that should be reflected)
        brickTex.Bind();
        pyramidVAO.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(pyramidIndices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
        cubeVAO.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Unbind the framebuffer after rendering the scene
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Render the mirror shape into the stencil buffer only.
        glEnable(GL_STENCIL_TEST);
        glStencilFunc(GL_ALWAYS, 1, 0xFF); 
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); 
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); 
        glDepthMask(GL_FALSE); 

        // Draw the mirror quad to mark its area in the stencil buffer.
        shaderProgram.Activate();
        glm::mat4 mirrorModel = glm::mat4(1.0f);
        shaderProgram.setMatrix4("model", mirrorModel);
        mirrorVAO.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Restore color and depth buffer writing.
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glDepthMask(GL_TRUE);

        // Render the reflected scene only in the mirror region.
        glStencilFunc(GL_EQUAL, 1, 0xFF); 
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

        // Use shaderProgram for the reflected scene.
        shaderProgram.Activate();
        shaderProgram.setMatrix4("view", camera.viewMatrix * reflectionMatrix);
        glm::vec3 reflectedCameraPos = glm::vec3(camera.Position.x, camera.Position.y, -camera.Position.z - 6);
        shaderProgram.setVec3("cameraPos", reflectedCameraPos);

        // Render the pyramid again with the reflection transformation.
        pyramidModel = glm::mat4(1.0f);
        pyramidModel = glm::translate(pyramidModel, pyramidPos);
        pyramidModel = reflectionMatrix * pyramidModel;
        mirrorShader.setMatrix4("model", pyramidModel);
        brickTex.Bind();
        pyramidVAO.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(pyramidIndices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
        
        // The same goes for other objects
        cubeModel = glm::mat4(1.0f);
        cubeModel = glm::translate(cubeModel, cubePos);
        shaderProgram.setMatrix4("model", cubeModel); 
        cubeVAO.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        
        sphereModel = glm::mat4(1.0f);
        sphereModel = glm::translate(sphereModel, glm::vec3(2.0f, 1.0f, 0.0f));
        sphereModel = glm::rotate(sphereModel, time, glm::vec3(0.0f, 1.0f, 0.0f));
        shaderProgram.setMatrix4("model", sphereModel); 
        sphereVAO.Bind();
        sphereTex.Bind();
        glDrawElements(GL_TRIANGLES, sphereInds.size(), GL_UNSIGNED_INT, 0);
        

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Disable stencil testing after reflection.
        glDisable(GL_STENCIL_TEST);

        // Draw the mirror surface normally.
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        mirrorShader.Activate();
        mirrorShader.setMatrix4("view", camera.viewMatrix);
        mirrorShader.setMatrix4("projection", camera.projectionMatrix);
        mirrorShader.setMatrix4("model", mirrorModel);
        mirrorShader.setVec4("mirrorColor", glm::vec4(1.0f, 1.0f, 1.0f, 0.3f)); // Semi-transparent mirror

        // Set the reflection texture in the shader
        mirrorShader.setInt("reflectionTexture", 0); 
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, reflectionTexture);

        // Draw the mirror surface
        mirrorVAO.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDisable(GL_BLEND);

        // Swap buffers and poll events.
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // --- Cleanup ---
    pyramid.Delete();
    cube.Delete();
    cube.Delete();
    floor.Delete();
    sphere.Delete();
    brickTex.Delete();
    floorTex.Delete();
    shaderProgram.Delete();
    lightCube.Delete();
    lightShader.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
