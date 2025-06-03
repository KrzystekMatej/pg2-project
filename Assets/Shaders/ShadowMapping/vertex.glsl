#version 460 core
layout(location = 0) in vec3 localPosition;

uniform mat4 model;

void main()
{
    gl_Position = model * vec4(localPosition, 1.0);
}