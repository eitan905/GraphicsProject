#version 330 core

struct Material
{
	sampler2D textureMap;
	
};
uniform samplerCube cubemap;
uniform Material material;
uniform bool normalFlag;
uniform bool envFlag;
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
uniform samplerCube skybox;


in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoords;
in vec3 textureDir;


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
	if(normalFlag){
		N = normal;
	}
	float levels = 6;
	float bright = max(dot(N, I), 0.0f);
	float level = floor(bright * levels);
	bright = level/levels;
	vec3 I_A = (textureColor * L_A) * bright;
	vec3 I_D = (max(dot(N, I), 0.0f) * normalize(textureColor * L_D)) * bright;
	vec3 I_S = (pow(max(dot(R, V),0.0f), user_angle) * normalize(textureColor * L_S)) * bright;
	vec3 final_light = (I_A + I_D + I_S);

	final_light.x = final_light.x/1.5;
	final_light.y = final_light.y/1.5;
	final_light.z = final_light.z/1.5;
	//textureColor.x = textureColor.x * bright * 1.3;
	//textureColor.y = textureColor.y * bright * 1.3;
	//textureColor.z = textureColor.z * bright * 1.3;

	

	I = normalize(vec3((cameraPos.x - fragPos.x), (cameraPos.y - fragPos.y) , (cameraPos.z + fragPos.z)));
    R = reflect(-I, N);
	frag_color = vec4(textureColor,1);
}
