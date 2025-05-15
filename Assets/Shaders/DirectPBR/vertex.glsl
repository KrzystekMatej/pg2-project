#version 460 core

layout(location = 0) in vec3 localPosition;
layout(location = 1) in vec3 localNormal;
layout(location = 2) in vec3 color;
layout(location = 3) in vec2 texCoords;
layout(location = 4) in vec3 tangent;
layout(location = 5) in uint materialIndex;

out vec3 worldPosition;
out vec3 worldNormal;
in vec2 fragTexCoords;
out vec3 worldTangent;
flat out uint fragMaterialIndex;

uniform mat4 pvmMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

void main(void)
{
    gl_Position = pvmMatrix * vec4(localPosition, 1.f);
    vec4 worldPositionH = modelMatrix * vec4(localPosition, 1.f);
    worldPosition = worldPositionH.xyz / worldPositionH.w;
    worldNormal = normalize(normalMatrix * localNormal);
    worldTangent = normalize(normalMatrix * localTangent);

    fragTexCoords = texCoords;
    fragMaterialIndex = materialIndex;
}