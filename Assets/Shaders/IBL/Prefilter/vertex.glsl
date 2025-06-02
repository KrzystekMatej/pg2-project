#version 460 core
layout(location = 0) in vec3 localPosition;

out vec3 worldPosition;

uniform mat4 pv;

void main()
{
    WorldPosition = localPosition;
    gl_Position = pv * vec4(worldPosition, 1.0);
}