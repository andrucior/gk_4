//#pragma once
//
//#include <iostream>
//#include <vector>
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <optional>
//#include "Texture.h"
//#include "Shader.h"
//#include "VAO.h"
//#include "VBO.h"
//#include "EBO.h"
//#include "Camera.h"
//#include "LightSource.h"
//#include "Object.h"
//
//class Program {
//public:
//    Program(int width, int height) : width(width), height(height), camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f)) {}
//
//    int Run() {
//        if (!InitializeWindow()) return -1;
//        SetupOpenGL();
//        CreateObjects();
//        SetupTextures();
//        SetupShaders();
//        SetupLights();
//        SetupReflection();
//
//        while (!glfwWindowShouldClose(window)) {
//            float currentFrame = glfwGetTime();
//            deltaTime = currentFrame - lastFrame;
//            lastFrame = currentFrame;
//
//            ProcessInput();
//            Update(deltaTime);
//            Render();
//            glfwSwapBuffers(window);
//            glfwPollEvents();
//        }
//
//        Cleanup();
//        return 0;
//    }
//
//private:
//    GLFWwindow* window;
//    int width, height;
//    float lastFrame = 0.0f, deltaTime = 0.0f;
//
//    // Graphics objects
//    std::optional<Object> pyramid, cube, floorObj, sphere, lightCube, mirror;
//    Shader mainShader, mirrorShader, lightShader;
//    Texture brickTex, floorTex, sphereTex;
//
//    // Lighting
//    LightSource fixedLight, spotLight, dirLight;
//    Camera camera;
//
//    // Reflection
//    GLuint reflectionFBO = 0, reflectionTexture = 0;
//    glm::mat4 reflectionMatrix;
//
//    bool InitializeWindow() {
//        glfwInit();
//        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//        window = glfwCreateWindow(width, height, "OpenGL Program", NULL, NULL);
//        if (!window) {
//            std::cerr << "Failed to create GLFW window" << std::endl;
//            glfwTerminate();
//            return false;
//        }
//        glfwMakeContextCurrent(window);
//        return true;
//    }
//
//    void SetupOpenGL() {
//        gladLoadGL();
//        glViewport(0, 0, width, height);
//        glEnable(GL_DEPTH_TEST);
//        glEnable(GL_STENCIL_TEST);
//        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
//    }
//
//    void CreateObjects() {
//        CreatePyramid();
//        CreateCube();
//        CreateFloor();
//        CreateSphere();
//        CreateLightCube();
//        CreateMirror();
//    }
//
//    void CreatePyramid() {
//        GLfloat vertices[] = 
//        {   //  POSITION             COLOR              TexCoord         NORMAL
//            -0.5f, 0.0f,  0.5f,      0.83f,0.70f,0.44f,   0.0f, 0.0f,      0.0f, -1.0f, 0.0f,
//            -0.5f, 0.0f, -0.5f,      0.83f,0.70f,0.44f,   0.0f, 5.0f,      0.0f, -1.0f, 0.0f,
//             0.5f, 0.0f, -0.5f,      0.83f,0.70f,0.44f,   5.0f, 5.0f,      0.0f, -1.0f, 0.0f,
//             0.5f, 0.0f,  0.5f,      0.83f,0.70f,0.44f,   5.0f, 0.0f,      0.0f, -1.0f, 0.0f,
//
//            -0.5f, 0.0f,  0.5f,      0.83f,0.70f,0.44f,   0.0f, 0.0f,     -0.8f, 0.5f,  0.0f,
//            -0.5f, 0.0f, -0.5f,      0.83f,0.70f,0.44f,   5.0f, 0.0f,     -0.8f, 0.5f,  0.0f,
//             0.0f, 0.8f,  0.0f,      0.92f,0.86f,0.76f,   2.5f, 5.0f,     -0.8f, 0.5f,  0.0f,
//
//            -0.5f, 0.0f, -0.5f,      0.83f,0.70f,0.44f,   5.0f, 0.0f,      0.0f, 0.5f, -0.8f,
//             0.5f, 0.0f, -0.5f,      0.83f,0.70f,0.44f,   0.0f, 0.0f,      0.0f, 0.5f, -0.8f,
//             0.0f, 0.8f,  0.0f,      0.92f,0.86f,0.76f,   2.5f, 5.0f,      0.0f, 0.5f, -0.8f,
//
//             0.5f, 0.0f, -0.5f,      0.83f,0.70f,0.44f,   0.0f, 0.0f,      0.8f, 0.5f,  0.0f,
//             0.5f, 0.0f,  0.5f,      0.83f,0.70f,0.44f,   5.0f, 0.0f,      0.8f, 0.5f,  0.0f,
//             0.0f, 0.8f,  0.0f,      0.92f,0.86f,0.76f,   2.5f, 5.0f,      0.8f, 0.5f,  0.0f,
//
//             0.5f, 0.0f,  0.5f,      0.83f,0.70f,0.44f,   5.0f, 0.0f,      0.0f, 0.5f,  0.8f,
//            -0.5f, 0.0f,  0.5f,      0.83f,0.70f,0.44f,   0.0f, 0.0f,      0.0f, 0.5f,  0.8f,
//             0.0f, 0.8f,  0.0f,      0.92f,0.86f,0.76f,   2.5f, 5.0f,      0.0f, 0.5f,  0.8f
//        };
//        GLuint indices[] = 
//        {
//            0, 1, 2, 0, 2, 3,
//            4, 6, 5,
//            7, 9, 8,
//            10, 12, 11,
//            13, 15, 14 
//        };
//        GLuint vertexCount = sizeof(vertices) / (11 * sizeof(GLfloat));
//        GLuint indexCount = sizeof(indices) / sizeof(GLuint);
//        VAO vao;
//        vao.Bind();
//        VBO vbo(vertices, sizeof(vertices));
//        EBO ebo(indices, sizeof(indices));
//        pyramid = Object(vao, vbo, vertexCount);
//        pyramid.value().LinkAttributes();
//        pyramid.value().SetEBO(ebo, indexCount);
//        pyramid.value().Unbind();
//    }
//
//    void CreateCube() {
//        GLfloat vertices[] = 
//        {
//            // Back face
//            1.0f, -0.5f, -0.5f,  1,0,0,   0.0f, 0.0f,    0,0,-1,
//            2.0f,  0.5f, -0.5f,  1,0,0,   1.0f, 1.0f,    0,0,-1,
//            2.0f, -0.5f, -0.5f,  1,0,0,   1.0f, 0.0f,    0,0,-1,
//            2.0f,  0.5f, -0.5f,  1,0,0,   1.0f, 1.0f,    0,0,-1,
//            1.0f, -0.5f, -0.5f,  1,0,0,   0.0f, 0.0f,    0,0,-1,
//            1.0f,  0.5f, -0.5f,  1,0,0,   0.0f, 1.0f,    0,0,-1,
//            // Front face
//            1.0f, -0.5f,  0.5f,  0,1,0,   0.0f, 0.0f,    0,0,1,
//            2.0f, -0.5f,  0.5f,  0,1,0,   1.0f, 0.0f,    0,0,1,
//            2.0f,  0.5f,  0.5f,  0,1,0,   1.0f, 1.0f,    0,0,1,
//            2.0f,  0.5f,  0.5f,  0,1,0,   1.0f, 1.0f,    0,0,1,
//            1.0f,  0.5f,  0.5f,  0,1,0,   0.0f, 1.0f,    0,0,1,
//            1.0f, -0.5f,  0.5f,  0,1,0,   0.0f, 0.0f,    0,0,1,
//            // Left face
//            1.0f,  0.5f,  0.5f,  0,0,1,   1.0f, 0.0f,   -1,0,0,
//            1.0f,  0.5f, -0.5f,  0,0,1,   1.0f, 1.0f,   -1,0,0,
//            1.0f, -0.5f, -0.5f,  0,0,1,   0.0f, 1.0f,   -1,0,0,
//            1.0f, -0.5f, -0.5f,  0,0,1,   0.0f, 1.0f,   -1,0,0,
//            1.0f, -0.5f,  0.5f,  0,0,1,   0.0f, 0.0f,   -1,0,0,
//            1.0f,  0.5f,  0.5f,  0,0,1,   1.0f, 0.0f,   -1,0,0,
//            // Right face
//            2.0f,  0.5f,  0.5f,  1,1,0,   1.0f, 0.0f,    1,0,0,
//            2.0f, -0.5f, -0.5f,  1,1,0,   0.0f, 1.0f,    1,0,0,
//            2.0f,  0.5f, -0.5f,  1,1,0,   1.0f, 1.0f,    1,0,0,
//            2.0f, -0.5f, -0.5f,  1,1,0,   0.0f, 1.0f,    1,0,0,
//            2.0f,  0.5f,  0.5f,  1,1,0,   1.0f, 0.0f,    1,0,0,
//            2.0f, -0.5f,  0.5f,  1,1,0,   0.0f, 0.0f,    1,0,0,
//            // Bottom face
//            1.0f, -0.5f, -0.5f,  1,0,1,   0.0f, 1.0f,    0,-1,0,
//            2.0f, -0.5f, -0.5f,  1,0,1,   1.0f, 1.0f,    0,-1,0,
//            2.0f, -0.5f,  0.5f,  1,0,1,   1.0f, 0.0f,    0,-1,0,
//            2.0f, -0.5f,  0.5f,  1,0,1,   1.0f, 0.0f,    0,-1,0,
//            1.0f, -0.5f,  0.5f,  1,0,1,   0.0f, 0.0f,    0,-1,0,
//            1.0f, -0.5f, -0.5f,  1,0,1,   0.0f, 1.0f,    0,-1,0,
//            // Top face
//            1.0f,  0.5f, -0.5f,  0,1,1,   0.0f, 1.0f,    0,1,0,
//            2.0f,  0.5f,  0.5f,  0,1,1,   1.0f, 0.0f,    0,1,0,
//            2.0f,  0.5f, -0.5f,  0,1,1,   1.0f, 1.0f,    0,1,0,
//            2.0f,  0.5f,  0.5f,  0,1,1,   1.0f, 0.0f,    0,1,0,
//            1.0f,  0.5f, -0.5f,  0,1,1,   0.0f, 1.0f,    0,1,0,
//            1.0f,  0.5f,  0.5f,  0,1,1,   0.0f, 0.0f,    0,1,0,
//        };
//        VAO vao;
//        vao.Bind();
//        VBO vbo(vertices, sizeof(vertices));
//        GLuint vertexCount = sizeof(vertices) / (11 * sizeof(GLfloat));
//        cube = Object(vao, vbo, vertexCount);
//        cube.value().LinkAttributes();
//        cube.value().Unbind();
//    }
//
//    void CreateFloor() {
//        GLfloat vertices[] = 
//        {
//            // positions              // colors       // texCoords   // normals
//            -10.0f, 0.0f,  10.0f,      1,1,1,         0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
//             10.0f, 0.0f,  10.0f,      1,1,1,         1.0f, 0.0f,   0.0f, 1.0f, 0.0f,
//             10.0f, 0.0f, -10.0f,      1,1,1,         1.0f, 1.0f,   0.0f, 1.0f, 0.0f,
//            -10.0f, 0.0f, -10.0f,      1,1,1,         0.0f, 1.0f,   0.0f, 1.0f, 0.0f,
//        };
//        GLuint indices[] = 
//        {
//            0,1,2,
//            0,2,3
//        };
//
//        VAO vao;
//        vao.Bind();
//        VBO vbo(vertices, sizeof(vertices));
//        EBO ebo(indices, sizeof(indices));
//        floorObj = Object(vao, vbo, 4);
//        floorObj.value().LinkAttributes();
//        GLuint indexCount = sizeof(indices) / sizeof(GLuint);
//        floorObj.value().SetEBO(ebo, indexCount);
//        floorObj.value().Unbind();
//    }
//
//    void CreateSphere() {
//        std::vector<float> vertices;
//        std::vector<unsigned int> indices;
//        GenerateSphere(0.5f, 36, 18, vertices, indices);
//
//        VAO vao;
//        vao.Bind();
//        VBO vbo(vertices.data(), vertices.size() * sizeof(float));
//        EBO ebo(indices.data(), indices.size() * sizeof(unsigned int));
//        sphere = Object(vao, vbo, vertices.size());
//        sphere.value().LinkAttributes();
//        sphere.value().SetEBO(ebo, indices.size());
//        sphere.value().Unbind();
//    }
//
//    void CreateLightCube() {
//        GLfloat vertices[] = 
//        {
//            -0.1f, -0.1f,  0.1f,
//            -0.1f, -0.1f, -0.1f,
//             0.1f, -0.1f, -0.1f,
//             0.1f, -0.1f,  0.1f,
//            -0.1f,  0.1f,  0.1f,
//            -0.1f,  0.1f, -0.1f,
//             0.1f,  0.1f, -0.1f,
//             0.1f,  0.1f,  0.1f
//        };
//        GLuint indices[] = 
//        {
//            0,1,2,  0,2,3,
//            0,4,7,  0,7,3,
//            3,7,6,  3,6,2,
//            2,6,5,  2,5,1,
//            1,5,4,  1,4,0,
//            4,5,6,  4,6,7
//        };
//
//        VAO vao;
//        vao.Bind();
//        VBO vbo(vertices, sizeof(vertices));
//        EBO ebo(indices, sizeof(indices));
//        GLuint vertexCount = sizeof(vertices) / (3 * sizeof(GLfloat)); // 24 / 3 = 8
//        GLuint indexCount = sizeof(indices) / sizeof(GLuint); // 36
//        lightCube = Object(vao, vbo,vertexCount);
//        lightCube.value().LinkAttributes();
//        lightCube.value().SetEBO(ebo, indexCount);
//        lightCube.value().Unbind();
//    }
//
//    void CreateMirror() {
//        GLfloat vertices[] = 
//        {
//            // positions            // colors   // texCoords  // normals
//            -1.0f, 0.0f, -3.0f,      1,1,1,     0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
//             1.0f, 0.0f, -3.0f,      1,1,1,     1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
//             1.0f, 2.0f, -3.0f,      1,1,1,     1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
//            -1.0f, 2.0f, -3.0f,      1,1,1,     0.0f, 1.0f,  0.0f, 0.0f, 1.0f
//        };
//        GLuint indices[] = 
//        {
//            0,1,2,
//            0,2,3 
//        };
//
//        VAO vao;
//        vao.Bind();
//        VBO vbo(vertices, sizeof(vertices));
//        EBO ebo(indices, sizeof(indices));
//        GLuint vertexCount = sizeof(vertices) / (11 * sizeof(GLfloat));
//        GLuint indexCount = sizeof(indices) / sizeof(GLuint);
//        mirror = Object(vao, vbo, vertexCount);
//        mirror.value().LinkAttributes();
//        mirror.value().SetEBO(ebo, indexCount);
//        mirror.value().Unbind();
//    }
//
//    void SetupTextures() {
//        brickTex = Texture("brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
//        floorTex = Texture("wood_texture.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
//        sphereTex = Texture("shrek.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
//    }
//
//    void SetupShaders() {
//        mainShader = Shader("default.vert", "default.frag");
//        mirrorShader = Shader("mirror.vert", "mirror.frag");
//        // lightShader = Shader("light.vert", "light.frag");
//
//        brickTex.texUnit(mainShader, "tex0", 0);
//        floorTex.texUnit(mainShader, "tex1", 0);
//        sphereTex.texUnit(mainShader, "tex2", 0);
//    }
//
//    void SetupLights() {
//        fixedLight = LightSource(POINT_LIGHT, glm::vec3(0.5f, 0.5f, 6.5f), glm::vec3(1.0f, 0.3f, 0.3f));
//        spotLight = LightSource(SPOT_LIGHT, glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(1.0f, 1.0f, 0.8f));
//        dirLight = LightSource(DIRECTIONAL_LIGHT, glm::vec3(5.0f, 5.0f, 1.0f), glm::vec3(0.9f, 0.9f, 0.9f));
//        dirLight.direction = glm::normalize(glm::vec3(0.0f, -2.0f, 0.0f));
//
//        UpdateLightUniforms();
//    }
//
//    void SetupReflection() {
//        reflectionMatrix = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -6)), glm::vec3(1, 1, -1));
//
//        glGenFramebuffers(1, &reflectionFBO);
//        glBindFramebuffer(GL_FRAMEBUFFER, reflectionFBO);
//
//        glGenTextures(1, &reflectionTexture);
//        glBindTexture(GL_TEXTURE_2D, reflectionTexture);
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, reflectionTexture, 0);
//
//        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//            std::cerr << "Framebuffer not complete!" << std::endl;
//
//        glBindFramebuffer(GL_FRAMEBUFFER, 0);
//    }
//
//    void ProcessInput() {
//        camera.Inputs(window);
//
//        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) camera.mode = FIRST_PERSON;
//        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) camera.mode = THIRD_PERSON;
//        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) camera.mode = ORBITAL;
//    }
//
//    void Update(float deltaTime) {
//        camera.updateMatrix(45.0f, 0.1f, 100.0f);
//        UpdateLightPositions();
//        UpdateLightUniforms();
//    }
//
//    void UpdateLightPositions() {
//        static glm::vec3 cubePos = glm::vec3(sinf(glfwGetTime()) * 3.0f, 0.5f, 1.0f);
//        camera.attachedObject = &cubePos;
//
//        // Update spotlight position based on cube position
//        glm::vec3 localOffset(0.2f, 0.2f, 0.0f);
//        spotLight.position = cubePos + localOffset;
//    }
//
//    void UpdateLightUniforms() {
//        mainShader.Activate();
//        fixedLight.setUniforms(mainShader, "fixedLight");
//        spotLight.setUniforms(mainShader, "spotLight");
//        dirLight.setUniforms(mainShader, "dirLight");
//
//        mainShader.setVec3("cameraPos", camera.Position);
//        mainShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
//        mainShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
//    }
//
//    void Render() {
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
//
//        RenderMainScene();
//        RenderMirror();
//    }
//
//    void RenderMainScene() {
//        mainShader.Activate();
//        mainShader.setMatrix4("view", camera.viewMatrix);
//        mainShader.setMatrix4("projection", camera.projectionMatrix);
//
//        RenderPyramid();
//        RenderCube();
//        RenderSphere();
//        RenderFloor();
//        RenderLightCube();
//    }
//
//    void RenderPyramid() {
//        glm::mat4 model = glm::mat4(1.0f);
//        mainShader.setMatrix4("model", model);
//        brickTex.Bind();
//        pyramid.value().Bind();
//        glDrawElements(GL_TRIANGLES, pyramid.value().GetIndexCount(), GL_UNSIGNED_INT, 0);
//    }
//
//    void RenderCube() {
//        glm::mat4 model = glm::mat4(1.0f);
//        model = glm::translate(model, *camera.attachedObject);
//        mainShader.setMatrix4("model", model);
//        cube.value().Bind();
//        glDrawArrays(GL_TRIANGLES, 0, 36);
//    }
//
//    void RenderSphere() {
//        glm::mat4 model = glm::mat4(1.0f);
//        model = glm::translate(model, glm::vec3(2.0f, 1.0f, 0.0f));
//        model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
//        mainShader.setMatrix4("model", model);
//        sphereTex.Bind();
//        sphere.value().Bind();
//        glDrawElements(GL_TRIANGLES, sphere.value().GetIndexCount(), GL_UNSIGNED_INT, 0);
//    }
//
//    void RenderFloor() {
//        glm::mat4 model = glm::mat4(1.0f);
//        mainShader.setMatrix4("model", model);
//        floorTex.Bind();
//        floorObj.value().Bind();
//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//    }
//
//    void RenderLightCube() {
//        mainShader.Activate();
//        mainShader.setMatrix4("view", camera.viewMatrix);
//        mainShader.setMatrix4("projection", camera.projectionMatrix);
//
//        glm::mat4 model = glm::mat4(1.0f);
//        model = glm::translate(model, glm::vec3(3.5f, 1.5f, 5.5f));
//        lightShader.setMatrix4("model", model);
//        lightCube.value().Bind();
//        glDrawElements(GL_TRIANGLES, lightCube.value().GetIndexCount(), GL_UNSIGNED_INT, 0);
//    }
//
//    void RenderMirror() {
//        RenderReflection();
//        RenderMirrorSurface();
//    }
//
//    void RenderReflection() {
//        glBindFramebuffer(GL_FRAMEBUFFER, reflectionFBO);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//        mainShader.Activate();
//        mainShader.setMatrix4("view", camera.viewMatrix * reflectionMatrix);
//        mainShader.setMatrix4("projection", camera.projectionMatrix);
//
//        RenderPyramid();
//        RenderCube();
//        RenderSphere();
//
//        glBindFramebuffer(GL_FRAMEBUFFER, 0);
//    }
//
//    void RenderMirrorSurface() {
//        glEnable(GL_STENCIL_TEST);
//        glStencilFunc(GL_ALWAYS, 1, 0xFF);
//        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
//        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
//        glDepthMask(GL_FALSE);
//
//        mainShader.Activate();
//        glm::mat4 model = glm::mat4(1.0f);
//        mainShader.setMatrix4("model", model);
//        mirror.value().Bind();
//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//
//        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
//        glDepthMask(GL_TRUE);
//        glStencilFunc(GL_EQUAL, 1, 0xFF);
//        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
//
//        mirrorShader.Activate();
//        mirrorShader.setMatrix4("view", camera.viewMatrix);
//        mirrorShader.setMatrix4("projection", camera.projectionMatrix);
//        mirrorShader.setMatrix4("model", model);
//        mirrorShader.setInt("reflectionTexture", 0);
//
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, reflectionTexture);
//
//        glEnable(GL_BLEND);
//        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//        mirror.value().Bind();
//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//        glDisable(GL_BLEND);
//        glDisable(GL_STENCIL_TEST);
//    }
//
//    static void GenerateSphere(float radius, unsigned sectorCount, unsigned stackCount,
//        std::vector<float>& vertices, std::vector<unsigned int>& indices) {
//        // Sphere generation implementation
//    }
//
//    void Cleanup() {
//        pyramid.value().Delete();
//        cube.value().Delete();
//        floorObj.value().Delete();
//        sphere.value().Delete();
//        lightCube.value().Delete();
//        mirror.value().Delete();
//
//        brickTex.Delete();
//        floorTex.Delete();
//        sphereTex.Delete();
//
//        mainShader.Delete();
//        mirrorShader.Delete();
//        lightShader.Delete();
//
//        glDeleteFramebuffers(1, &reflectionFBO);
//        glDeleteTextures(1, &reflectionTexture);
//
//        glfwDestroyWindow(window);
//        glfwTerminate();
//    }
//};