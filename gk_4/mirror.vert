#version 330 core

layout(location = 0) in vec3 aPos;       // Object position
layout(location = 1) in vec2 aTexCoords; // Texture coordinates

out vec2 TexCoords;  // Output texture coordinates to fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Pass texture coordinates to fragment shader
    TexCoords = aTexCoords;

    // Calculate the final position of the vertex
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
