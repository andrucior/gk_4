#version 330 core

in vec2 TexCoords;  // Receive texture coordinates from vertex shader

out vec4 FragColor;

uniform vec4 mirrorColor;  
uniform sampler2D reflectionTexture;  // The reflection texture

void main()
{
    // Sample the reflection texture with the given texture coordinates
    FragColor = texture(reflectionTexture, TexCoords) * mirrorColor;
}
