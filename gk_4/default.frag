#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in float FogFactor;

out vec4 FragColor;

uniform sampler2D tex0;
uniform vec3 camPos;
uniform vec3 fogColor;

// Light structure definition.
struct Light {
    vec3 position;
    vec3 color;
    vec3 direction;
    float cutOff;
    float outerCutOff;
};

// Uniforms for different lights.
uniform Light fixedLight;
uniform Light spotLight;
uniform Light dirLight;

// Function to calculate lighting (Phong model example).
vec3 calculateLighting(Light light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir;
    float attenuation = 1.0;

    // Spotlight calculation
    if (light.cutOff > 0.0) 
    {
        lightDir = normalize(light.position - FragPos);
        float theta = dot(lightDir, normalize(-light.direction));
        float epsilon = light.cutOff - light.outerCutOff;
        float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
        attenuation = intensity;
    }
    // Directional light
    else if (light.direction != vec3(0.0)) 
    {
        lightDir = normalize(-light.direction);
    }
    // Point light
    else 
    {
        lightDir = normalize(light.position - FragPos);
        float distance = length(light.position - FragPos);
        attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));
    }

    // Diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    // Combine
    vec3 ambient = light.color * 0.2;
    vec3 diffuse = light.color * diff;
    vec3 specular = light.color * spec * 0.5;

    return (ambient + diffuse + specular) * attenuation;
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(camPos - FragPos);

    // Sum the lighting contributions.
    vec3 lighting = calculateLighting(fixedLight, norm, viewDir) +
                    calculateLighting(spotLight, norm, viewDir) +
                    calculateLighting(dirLight, norm, viewDir);
                    
    // Apply texture and multiply by lighting.
    vec4 objectColor = texture(tex0, TexCoords) * vec4(lighting, 1.0);
    
    // Mix the object color with the fog color.
    vec3 finalColor = mix(fogColor, objectColor.rgb, FogFactor);
    FragColor = vec4(finalColor, objectColor.a);
}
