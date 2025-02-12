#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

// This class encapsulates a renderable object's VAO/VBO/EBO setup.
// We assume that every vertex consists of 11 floats:
//   3 for position, 3 for color, 2 for texture coordinates, 3 for normals.
class RenderedObject {
public:
    VAO objectVAO;
    VBO objectVBO;
    // We use a pointer for the EBO; if the object is drawn without indices, this will remain nullptr.
    EBO* objectEBO;
    bool useEBO;
    GLsizei count; // either number of indices or number of vertices

    // Constructor for objects with indices.
    RenderedObject(GLfloat* vertices, size_t vertexSize, GLuint* indices, size_t indexSize)
        : objectVAO(), objectVBO(vertices, vertexSize), objectEBO(new EBO(indices, indexSize))
    {
        useEBO = true;
        count = static_cast<GLsizei>(indexSize / sizeof(GLuint));

        objectVAO.Bind();
        objectVAO.LinkAttrib(objectVBO, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
        objectVAO.LinkAttrib(objectVBO, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
        objectVAO.LinkAttrib(objectVBO, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
        objectVAO.LinkAttrib(objectVBO, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
        objectVAO.Unbind();
        objectVBO.Unbind();
        objectEBO->Unbind();
    }

    // Constructor for objects without indices (drawn with glDrawArrays).
    RenderedObject(GLfloat* vertices, size_t vertexSize)
        : objectVAO(), objectVBO(vertices, vertexSize), objectEBO(nullptr)
    {
        useEBO = false;
        count = static_cast<GLsizei>(vertexSize / (11 * sizeof(float)));

        objectVAO.Bind();
        objectVAO.LinkAttrib(objectVBO, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
        objectVAO.LinkAttrib(objectVBO, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
        objectVAO.LinkAttrib(objectVBO, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
        objectVAO.LinkAttrib(objectVBO, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
        objectVAO.Unbind();
        objectVBO.Unbind();
    }

    // Draws the object.
    void Draw() {
        objectVAO.Bind();
        if (useEBO)
            glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
        else
            glDrawArrays(GL_TRIANGLES, 0, count);
    }

    // Deletes buffers.
    void Delete() {
        objectVAO.Delete();
        objectVBO.Delete();
        if (objectEBO != nullptr) {
            objectEBO->Delete();
            delete objectEBO;
        }
    }
};

