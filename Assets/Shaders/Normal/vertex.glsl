#version 460 core

layout(location=0) in vec3 localPosition;
layout(location=1) in vec3 localNormal;

uniform mat4 pvmMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

out vec3 worldPosition;
out vec3 worldNormal;

void main(void)
{
    gl_Position = pvmMatrix * vec4(localPosition, 1.f);
    vec4 worldPositionH = modelMatrix * vec4(localPosition, 1.f);
    worldPosition = worldPositionH.xyz / worldPositionH.w;
    worldNormal = normalize(normalMatrix * localNormal);
}