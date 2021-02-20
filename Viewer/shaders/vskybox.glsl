#version 330
layout(location = 0) in vec3 pos;




uniform mat4 view;
uniform mat4 projection;

out vec3 texCoords;

void main()
{
	vec4 temp = projection * view * vec4(pos, 1.0f);
	gl_Position = temp.xyww;
	texCoords = pos;
}