part1 TODO
part 3:

vertex shader
#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

// The model/view/projection matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// These outputs will be available in the fragment shader as inputs
out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragTexCoords;

void main()
{
	
	orig_fragPos = vec3(vec4(pos, 1.0f));
	fragPos = vec3(model * vec4(pos, 1.0f));
	fragNormal = mat3(model) * normal;


	fragTexCoords = texCoords;

	gl_Position = projection * view *  model * vec4(pos, 1.0f);
}


fragment shader

#version 330 core

struct Material
{
	sampler2D textureMap;

};

uniform Material material;

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoords;

out vec4 frag_color;

void main()
{
	vec3 textureColor = vec3(texture(material.textureMap, fragTexCoords));

	frag_color = vec4(fragPos,1);
}





part 4:
#version 330 core

struct Material
{
	sampler2D textureMap;

};

uniform Material material;

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoords;

out vec4 frag_color;

void main()
{
	vec3 textureColor = vec3(texture(material.textureMap, fragTexCoords));

	frag_color = vec4(1,0,0,1);
}


part 5: 
void Renderer::Render(const Scene& scene)
{




	int cameraCount = scene.GetCameraCount();
	if (cameraCount > 0)
	{
		int modelCount = scene.GetModelCount();
		Camera& camera = scene.GetActiveCamera();

		for (int currentModelIndex = 0; currentModelIndex < modelCount; currentModelIndex++)
		{
			MeshModel& currentModel = scene.GetModel(currentModelIndex);

			// Activate the 'colorShader' program (vertex and fragment shaders)
			colorShader.use();

		
			// Set the uniform variables
			colorShader.setUniform("model", currentModel.GetTransform());
			colorShader.setUniform("view", camera.GetCameraTransform());
			colorShader.setUniform("projection", camera.GetProjectionTransformation());
			colorShader.setUniform("material.textureMap", 0);
			colorShader.setUniform("K_A", currentModel.K_A);
			colorShader.setUniform("K_D", currentModel.K_D);
			colorShader.setUniform("K_S", currentModel.K_S);
			if (scene.GetActiveLightIndex() != -1) {
				light light1 = scene.GetActiveLight();
				colorShader.setUniform("lightPos", light1.GetPosVec());
				colorShader.setUniform("user_angle", light1.user_angle);
				colorShader.setUniform("lightTransform", light1.GetTransform());
				colorShader.setUniform("L_A", light1.L_A);
				colorShader.setUniform("L_D", light1.L_D);
				colorShader.setUniform("L_S", light1.L_S);
			}
			colorShader.setUniform("cameraPos", camera.GetPosition());
			
			// Set 'texture1' as the active texture at slot #	0
			texture1.bind(0);

			// Drag our model's faces (triangles) in fill mode
			glBindVertexArray(camera.GetVAO());
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glBindVertexArray(currentModel.GetVAO());
			glDrawArrays(GL_TRIANGLES, 0, currentModel.GetModelVertices().size());
			glBindVertexArray(0);

			// Unset 'texture1' as the active texture at slot #0
			texture1.unbind(0);

			//colorShader.setUniform("color", glm::vec3(0, 0, 0));

			// Drag our model's faces (triangles) in line mode (wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glBindVertexArray(currentModel.GetVAO());
			glDrawArrays(GL_TRIANGLES, 0, currentModel.GetModelVertices().size());
			glBindVertexArray(0);
		}
	}

}
light from below - alpha 1:

![alt text](https://github.com/HaifaGraphicsCourses/computergraphics2021-eitan-and-hadar/blob/master/Assignment3Report/light%20from%20below%20-%20alpha%201.png)

light from below - alpha 5:

![alt text](https://github.com/HaifaGraphicsCourses/computergraphics2021-eitan-and-hadar/blob/master/Assignment3Report/light%20from%20below%20-%20alpha%205.png)

light from camera - alpha 2:

![alt text](https://github.com/HaifaGraphicsCourses/computergraphics2021-eitan-and-hadar/blob/master/Assignment3Report/light%20from%20camera%20-%20alpha%202.png)

light from camera - alpha 20:

![alt text](https://github.com/HaifaGraphicsCourses/computergraphics2021-eitan-and-hadar/blob/master/Assignment3Report/light%20from%20camera%20-%20alpha%2020.png)

light from camera - alpha 5:

![alt text](https://github.com/HaifaGraphicsCourses/computergraphics2021-eitan-and-hadar/blob/master/Assignment3Report/light%20from%20camera%20-%20alpha%205.png)

light from the right - alpha 10:

![alt text](https://github.com/HaifaGraphicsCourses/computergraphics2021-eitan-and-hadar/blob/master/Assignment3Report/light%20from%20the%20right%20-%20alpha%2010.png)

light front - alpha 2:

![alt text](https://github.com/HaifaGraphicsCourses/computergraphics2021-eitan-and-hadar/blob/master/Assignment3Report/light%20front%20-%20alpha%202.png)

light front - alpha 5:

![alt text](https://github.com/HaifaGraphicsCourses/computergraphics2021-eitan-and-hadar/blob/master/Assignment3Report/1.png)

![alt text](https://github.com/HaifaGraphicsCourses/computergraphics2021-eitan-and-hadar/blob/master/Assignment3Report/2.png)

![alt text](https://github.com/HaifaGraphicsCourses/computergraphics2021-eitan-and-hadar/blob/master/Assignment3Report/3.png)

![alt text](https://github.com/HaifaGraphicsCourses/computergraphics2021-eitan-and-hadar/blob/master/Assignment3Report/4.png)

![alt text](https://github.com/HaifaGraphicsCourses/computergraphics2021-eitan-and-hadar/blob/master/Assignment3Report/5.png)

![alt text](https://github.com/HaifaGraphicsCourses/computergraphics2021-eitan-and-hadar/blob/master/Assignment3Report/6.png)

![alt text](https://github.com/HaifaGraphicsCourses/computergraphics2021-eitan-and-hadar/blob/master/Assignment3Report/7.png)


part 7:

planar:
![alt text](https://github.com/HaifaGraphicsCourses/computergraphics2021-eitan-and-hadar/blob/master/Assignment3Report/planar_projection.png)

cylindrical:
![alt text](https://github.com/HaifaGraphicsCourses/computergraphics2021-eitan-and-hadar/blob/master/Assignment3Report/cylindrical_projection.png)

sphearical:
![alt text](https://github.com/HaifaGraphicsCourses/computergraphics2021-eitan-and-hadar/blob/master/Assignment3Report/sphearical_1.png)

![alt text](https://github.com/HaifaGraphicsCourses/computergraphics2021-eitan-and-hadar/blob/master/Assignment3Report/spherical_2.png)

textured mesh with texture mapping:

planar:
![alt text](https://github.com/HaifaGraphicsCourses/computergraphics2021-eitan-and-hadar/blob/master/Assignment3Report/goku_planar.png)

cylincdircal: i have no idea why or how this turned out to be well fitted but different color
![alt text](https://github.com/HaifaGraphicsCourses/computergraphics2021-eitan-and-hadar/blob/master/Assignment3Report/goku_cylindrical.png)
sphearical:
![alt text](https://github.com/HaifaGraphicsCourses/computergraphics2021-eitan-and-hadar/blob/master/Assignment3Report/goku_sphearical.png)




part 8:

normal mapping:
without normals:
![alt text](https://github.com/HaifaGraphicsCourses/computergraphics2021-eitan-and-hadar/blob/master/Assignment3Report/normal_2.png)


with normals:
![alt text](https://github.com/HaifaGraphicsCourses/computergraphics2021-eitan-and-hadar/blob/master/Assignment3Report/normal_1.png)

environmental mapping:


![alt text](https://github.com/HaifaGraphicsCourses/computergraphics2021-eitan-and-hadar/blob/master/Assignment3Report/env_mapping_1.png)
![alt text](https://github.com/HaifaGraphicsCourses/computergraphics2021-eitan-and-hadar/blob/master/Assignment3Report/env_mapping_2.png)
![alt text](https://github.com/HaifaGraphicsCourses/computergraphics2021-eitan-and-hadar/blob/master/Assignment3Report/env_mapping_3.png)
![alt text](https://github.com/HaifaGraphicsCourses/computergraphics2021-eitan-and-hadar/blob/master/Assignment3Report/env_mapping_4.png)
![alt text](https://github.com/HaifaGraphicsCourses/computergraphics2021-eitan-and-hadar/blob/master/Assignment3Report/env_mapping_5.png)
![alt text](https://github.com/HaifaGraphicsCourses/computergraphics2021-eitan-and-hadar/blob/master/Assignment3Report/env_mapping_6.png)


toon shading:

![alt text](https://github.com/HaifaGraphicsCourses/computergraphics2021-eitan-and-hadar/blob/master/Assignment3Report/toon_shading_1.png)
![alt text](https://github.com/HaifaGraphicsCourses/computergraphics2021-eitan-and-hadar/blob/master/Assignment3Report/toon_shading_2.png)
![alt text](https://github.com/HaifaGraphicsCourses/computergraphics2021-eitan-and-hadar/blob/master/Assignment3Report/toon_shading_3.png)
![alt text](https://github.com/HaifaGraphicsCourses/computergraphics2021-eitan-and-hadar/blob/master/Assignment3Report/toon_shading_4.png)
![alt text](https://github.com/HaifaGraphicsCourses/computergraphics2021-eitan-and-hadar/blob/master/Assignment3Report/toon_shading_5.png)
![alt text](https://github.com/HaifaGraphicsCourses/computergraphics2021-eitan-and-hadar/blob/master/Assignment3Report/toon_shading_6.png)




