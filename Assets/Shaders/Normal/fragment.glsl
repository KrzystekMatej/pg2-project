#version 460 core

layout(location = 0) out vec4 FragColor;

in vec3 worldNormal;

void main(void)
{
    vec3 normalColor = normalize(worldNormal) * 0.5 + 0.5;

    FragColor = vec4(normalColor, 1.0);
}
