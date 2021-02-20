#version 330 core


out vec4 frag_color;
in vec3 texCoords;
uniform samplerCube cubemap;
void main()
{
	
	frag_color = texture(cubemap, texCoords);
}
