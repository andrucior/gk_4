#version 330 core

layout(location = 0) in vec3 aPos;       
layout(location = 1) in vec3 aNormal;    
layout(location = 2) in vec2 aTexCoords; 

// Outputs to the fragment shader
out vec3 FragPos;    
out vec3 Normal;     
out vec2 TexCoords;  
out float FogFactor; 

// Uniforms
uniform mat4 model;       
uniform mat4 view;      
uniform mat4 projection;  
uniform vec3 cameraPos;  
uniform float fogStart;
uniform float fogEnd;   

void main()
{
    // Transform vertex position to world space
    vec4 worldPos = model * vec4(aPos, 1.0);
    FragPos = worldPos.xyz;
    
    // Transform the normal vector properly to world space
    Normal = mat3(transpose(inverse(model))) * aNormal;
    
    // Pass texture coordinates to the fragment shader
    TexCoords = aTexCoords;
    
    // Compute the fog factor (exp(-density * (distance - fogStart)))
    float distance = length(FragPos - cameraPos);  // Compute distance from the camera
    float density = 0.05;  // Fog density 
    FogFactor = exp(-density * (distance - fogStart));  // Fog equation
    FogFactor = clamp(FogFactor, 0.0, 1.0);  // Clamp fog value between 0 (opaque) and 1 (clear)

    // Compute final vertex position in clip space
    gl_Position = projection * view * worldPos;
}
