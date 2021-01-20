#version 330 core

struct Material
{
	sampler2D textureMap;
	
};

uniform Material material;
uniform vec3 K_A;
uniform vec3 K_S;
uniform vec3 K_D;
uniform vec3 L_S;
uniform vec3 L_D;
uniform vec3 L_A;
uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform float user_angle;


in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoords;



out vec4 frag_color;

void main()
{
	vec3 textureColor = vec3(texture(material.textureMap, fragTexCoords));
	vec3 N = normalize(fragNormal);
	vec3 I = normalize(lightPos - (fragPos.xyz ));
	vec3 V = normalize(cameraPos - (fragPos.xyz ));
	vec3 R = normalize(reflect(-I, N));

	vec3 I_A = (K_A * L_A);
	vec3 I_D = (max(dot(N, I), 0.0f) * normalize(K_D * L_D));
	vec3 I_S = (pow(max(dot(R, V),0.0f), user_angle) * normalize(K_S * L_S));

	vec3 final_light = I_A + I_D + I_S;
	
	frag_color = vec4(final_light,1);
}
