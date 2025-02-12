#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in float FogFactor;

out vec4 FragColor;

uniform sampler2D textureSampler;
uniform vec3 fogColor;

void main()
{
    // Sample the texture
    vec4 objectColor = texture(textureSampler, TexCoords);

    // Apply the fog effect by blending the color
    vec3 finalColor = mix(fogColor, objectColor.rgb, FogFactor);

    FragColor = vec4(finalColor, objectColor.a);
}
