#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out float FogFactor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraPos;
uniform float fogStart;
uniform float fogEnd;

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    FragPos = worldPos.xyz;
    
    // Transform the normal correctly.
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;
    
    // Compute fog factor (linear fog)
    float distance = length(FragPos - cameraPos);
    float density = 0.05; // Adjust this value for softer fog
    FogFactor = exp(-density * (distance - fogStart));
    FogFactor = clamp(FogFactor, 0.0, 1.0);
    
    gl_Position = projection * view * worldPos;
}
