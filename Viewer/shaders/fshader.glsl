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
uniform sampler2D normalMap;

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoords;



out vec4 frag_color;

void main()
{
	// obtain normal from normal map in range [0,1]
    vec3 normal = vec3(texture(normalMap, fragTexCoords));
    //transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  
	vec3 textureColor = vec3(texture(material.textureMap, fragTexCoords));
	vec3 normalColor = vec3(texture(normalMap, fragTexCoords));
	vec3 N = normalize(fragNormal);
	vec3 I = normalize(lightPos - (fragPos.xyz ));
	vec3 V = normalize(cameraPos - (fragPos.xyz ));
	vec3 R = normalize(reflect(-I, N));
	vec3 I_A = (textureColor * L_A);
	vec3 I_D = (max(dot(normal, I), 0.0f) * normalize(textureColor * L_D));
	vec3 I_S = (pow(max(dot(R, V),0.0f), user_angle) * normalize(textureColor * L_S));

	vec3 final_light = (I_A + I_D + I_S);
	final_light.x = final_light.x/2;
	final_light.y = final_light.y/2;
	final_light.z = final_light.z/2;
	frag_color = vec4(textureColor,1);
}
