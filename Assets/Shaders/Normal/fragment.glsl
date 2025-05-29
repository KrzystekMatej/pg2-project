#version 460 core

in vec3 worldNormal;

out vec4 fragColor;

void main(void)
{
    vec3 normalColor = normalize(worldNormal) * 0.5 + 0.5;
    fragColor = vec4(normalColor, 1.0);
}
