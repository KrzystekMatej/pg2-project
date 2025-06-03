#version 460 core

in vec4 fragPosition;

uniform vec3 lightPosition;
uniform float farPlane;

void main()
{
    float lightDistance = length(fragPosition.xyz - lightPosition);

    lightDistance = lightDistance / farPlane;
    gl_FragDepth = lightDistance;
}