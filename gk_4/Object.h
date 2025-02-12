#pragma once
#include <optional>
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

class Object {
public:
    VAO ObjectVAO;
    VBO ObjectVBO;
    std::optional<EBO> ObjectEBO;


    void LinkAttributes() {
        // Position attribute
        ObjectVAO.LinkAttrib(ObjectVBO, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
        // Color attribute
        ObjectVAO.LinkAttrib(ObjectVBO, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
        // Texture coordinates attribute
        ObjectVAO.LinkAttrib(ObjectVBO, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
        // Normal attribute
        ObjectVAO.LinkAttrib(ObjectVBO, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
    }

    Object(VAO& vao, VBO& vbo)
        : ObjectVAO(vao), ObjectVBO(vbo) {}

    Object(VAO& vao, VBO& vbo, EBO& ebo)
        : ObjectVAO(vao), ObjectVBO(vbo), ObjectEBO(ebo) {}
    void SetEBO(EBO& ebo)
    {
        ObjectEBO = ebo;
    }

    // Unified Bind method
    void Bind()
    {
        ObjectVAO.Bind();
        ObjectVBO.Bind();
        if (ObjectEBO.has_value()) {
            ObjectEBO.value().Bind();
        }
    }


    void Unbind()
    {
        ObjectVAO.Unbind();
        ObjectVBO.Unbind();
        if (ObjectEBO.has_value())
        {
            ObjectEBO.value().Unbind();
        }
    }

    void Delete()
    {
        ObjectVAO.Delete();
        ObjectVBO.Delete();
        if (ObjectEBO.has_value())
        {
            ObjectEBO.value().Delete();
        }
    }
};