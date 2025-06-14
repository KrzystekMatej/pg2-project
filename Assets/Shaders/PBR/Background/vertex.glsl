#version 460 core
layout(location = 0) in vec3 localPosition;

uniform mat4 projection;
uniform mat4 view;

out vec3 worldPosition;

void main()
{
	worldPosition = localPosition;
	mat4 rotView = mat4(mat3(view));
	vec4 clipPos = projection * rotView * vec4(worldPosition, 1.0);
	gl_Position = clipPos.xyww;
}