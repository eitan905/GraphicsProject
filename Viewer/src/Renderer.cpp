#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

#include <iostream>
#include "light.h"
#include "Renderer.h"
#include "InitShader.h"
#include <glm\ext\matrix_transform.hpp>

#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)
#define Z_INDEX(width,x,y) ((x)+(y)*(width))






float Renderer::area(float x1, float y1, float x2, float y2, float x3, float y3)
{
	return abs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0);
}


Renderer::Renderer() :
	viewport_width_(1280),
	viewport_height_(720)
{

	//glewExperimental = GL_TRUE;
	//InitOpenGLRendering();
	//SwapBuffers();
	//CreateBuffers(viewport_width_, viewport_height_);

}




Renderer::~Renderer()
{
	delete[] color_buffer_;
}
void Renderer::PutPixel(int i, int j, const glm::vec3& color)
{
	if (i < 0) return; if (i >= viewport_width_) return;
	if (j < 0) return; if (j >= viewport_height_) return;

	color_buffer_[INDEX(viewport_width_, i, j, 0)] = color.x;
	color_buffer_[INDEX(viewport_width_, i, j, 1)] = color.y;
	color_buffer_[INDEX(viewport_width_, i, j, 2)] = color.z;
}

void Renderer::CreateBuffers(int w, int h)
{
	CreateOpenGLBuffer(); //Do not remove this line.
	//color_buffer_ = new float[3 * w * h];

	//mass_buffer = new float[3 * w * h * 4];
	//ClearColorBuffer(glm::vec3(0.0f, 0.0f, 0.0f));



}

void Renderer::ClearColorBuffer(const glm::vec3& color)
{
	for (int i = 0; i < viewport_width_; i++)
	{
		for (int j = 0; j < viewport_height_; j++)
		{
			PutPixel(i, j, color);

		}
	}

}



//compute nurmal faces
glm::vec3 normal(glm::vec3 x1, glm::vec3 x2, glm::vec3 x3)
{

	glm::vec3 temp1 = x1 - x3;
	glm::vec3 temp2 = x2 - x3;
	/*std::cout << "1" << temp1[0] << "," << temp1[1] << "," << temp1[2] <<std::endl;
	std::cout << "2" << temp2[0] << "," << temp2[1] << "," << temp2[2] << std::endl;*/
	glm::vec3 ans = glm::cross(temp1, temp2);

	double length = sqrt(pow(ans[0], 2) + pow(ans[1], 2) + pow(ans[2], 2));

	ans[0] /= length;
	ans[1] /= length;
	ans[2] /= length;
	return ans;
}





//##############################
//##OpenGL stuff. Don't touch.##
//##############################

// Basic tutorial on how opengl works:
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
// don't linger here for now, we will have a few tutorials about opengl later.


void Renderer::InitOpenGLRendering()
{
	// Creates a unique identifier for an opengl texture.
	glGenTextures(1, &gl_screen_tex_);

	// Same for vertex array object (VAO). VAO is a set of buffers that describe a renderable object.
	glGenVertexArrays(1, &gl_screen_vtc_);

	GLuint buffer;

	// Makes this VAO the current one.
	glBindVertexArray(gl_screen_vtc_);

	// Creates a unique identifier for a buffer.
	glGenBuffers(1, &buffer);

	// (-1, 1)____(1, 1)
	//	     |\  |
	//	     | \ | <--- The exture is drawn over two triangles that stretch over the screen.
	//	     |__\|
	// (-1,-1)    (1,-1)
	const GLfloat vtc[] = {
		-1, -1,
		 1, -1,
		-1,  1,
		-1,  1,
		 1, -1,
		 1,  1
	};

	const GLfloat tex[] = {
		0,0,
		1,0,
		0,1,
		0,1,
		1,0,
		1,1 };

	// Makes this buffer the current one.
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// This is the opengl way for doing malloc on the gpu. 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc) + sizeof(tex), NULL, GL_STATIC_DRAW);

	// memcopy vtc to buffer[0,sizeof(vtc)-1]
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);

	// memcopy tex to buffer[sizeof(vtc),sizeof(vtc)+sizeof(tex)]
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);

	// Loads and compiles a sheder.
	GLuint program = InitShader("vshader.glsl", "fshader.glsl");

	// Make this program the current one.
	glUseProgram(program);

	// Tells the shader where to look for the vertex position data, and the data dimensions.
	GLint  vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Same for texture coordinates data.
	GLint  vTexCoord = glGetAttribLocation(program, "vTexCoord");
	glEnableVertexAttribArray(vTexCoord);
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)sizeof(vtc));

	//glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );

	// Tells the shader to use GL_TEXTURE0 as the texture id.
	glUniform1i(glGetUniformLocation(program, "texture"), 0);

}

void Renderer::CreateOpenGLBuffer()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, gl_screen_tex_);

	// malloc for a texture on the gpu.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, viewport_width_, viewport_height_, 0, GL_RGB, GL_FLOAT, NULL);
	glViewport(0, 0, viewport_width_, viewport_height_);
}

void Renderer::SwapBuffers()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, gl_screen_tex_);

	// memcopy's colorBuffer into the gpu.
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, viewport_width_, viewport_height_, GL_RGB, GL_FLOAT, color_buffer_);

	// Tells opengl to use mipmapping
	glGenerateMipmap(GL_TEXTURE_2D);

	// Make glScreenVtc current VAO
	glBindVertexArray(gl_screen_vtc_);

	// Finally renders the data.
	glDrawArrays(GL_TRIANGLES, 0, 6);
}



void Renderer::Render(const Scene& scene)
{




	int cameraCount = scene.GetCameraCount();







	if (cameraCount > 0)
	{
		int modelCount = scene.GetModelCount();
		Camera& camera = scene.GetActiveCamera();
		bool envFlag = 0;

		//shader.use();
		//// ... set view and projection matrix
		//skyboxShader.setUniform("view", camera.GetCameraTransform());
		//skyboxShader.setUniform("projection", camera.GetProjectionTransformation());
		//glBindVertexArray(cubeVAO);
		//glBindTexture(GL_TEXTURE_2D, cubeMapTexture);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//glBindVertexArray(0);
	
		// ... draw rest of the scene
		// Draw our first triangle
		colorShader.use();


		


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);

		// Get the uniform locations
		colorShader.setUniform("model", scene.GetActiveModel().GetTransform());
		colorShader.setUniform("view", camera.GetCameraTransform());
		colorShader.setUniform("projection", camera.GetProjectionTransformation());
		colorShader.setUniform("normalFlag", scene.GetActiveModel().useNormalMapping);
		colorShader.setUniform("cameraPos", camera.GetPosition());
		colorShader.setUniformSampler("normalMap", 1);

		if (scene.GetActiveLightIndex() != -1) {
			light light1 = scene.GetActiveLight();
			colorShader.setUniform("lightPos", light1.GetPosVec());
			colorShader.setUniform("user_angle", light1.user_angle);
			colorShader.setUniform("lightTransform", light1.GetTransform());
			colorShader.setUniform("L_A", light1.L_A);
			colorShader.setUniform("L_D", light1.L_D);
			colorShader.setUniform("L_S", light1.L_S);
		}
	
		texture1.bind(0);
		// Drag our model's faces (triangles) in fill mode
		glBindVertexArray(camera.GetVAO());
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBindVertexArray(scene.GetActiveModel().GetVAO());
		glDrawArrays(GL_TRIANGLES, 0, scene.GetActiveModel().GetModelVertices().size());
		glBindVertexArray(0);
		// Bind Textures using texture units





		//glBindVertexArray(cubeVAO);
		//// Calculate the model matrix for each object and pass it to shader before drawing
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//glBindVertexArray(0);
		texture1.unbind(0);

		/*glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBindVertexArray(scene.GetActiveModel().GetVAO());
		glDrawArrays(GL_TRIANGLES, 0, scene.GetActiveModel().GetModelVertices().size());
		glBindVertexArray(0);*/
















		//Draw skybox as last
		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.use();
		glm::mat4 view = glm::mat4(glm::mat3(camera.GetTransform()));	// Remove any translation component of the view matrix

		skyboxShader.setUniform("view", view);
		skyboxShader.setUniform("projection", camera.GetProjectionTransformation());

		// skybox cube
		texture3.bind(cubeMapTexture);
		glBindVertexArray(skyboxVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // Set depth function back to default
		texture3.unbind(cubeMapTexture);
		







		//for (int currentModelIndex = 0; currentModelIndex < modelCount; currentModelIndex++)
		//{
		//	MeshModel& currentModel = scene.GetModel(currentModelIndex);
		//	if (currentModelIndex == scene.GetModelCount() - 1)
		//	{
		//		envFlag = 1;
		//	}
		//	// Activate the 'colorShader' program (vertex and fragment shaders)
		//	colorShader.use();

		//	
		//	// Set the uniform variables
		//	colorShader.setUniform("model", currentModel.GetTransform());
		//	colorShader.setUniform("view", camera.GetCameraTransform());
		//	colorShader.setUniform("projection", camera.GetProjectionTransformation());
		//	colorShader.setUniform("material.textureMap", 0);
		//	colorShader.setUniform("normalFlag", currentModel.useNormalMapping);
		//	colorShader.setUniform("K_A", currentModel.K_A);
		//	colorShader.setUniform("K_D", currentModel.K_D);
		//	colorShader.setUniform("K_S", currentModel.K_S);
		//	colorShader.setUniform("envFlag", envFlag);
		//	colorShader.setUniform("textureDir", camera.GetDirection());
		//	if (scene.GetActiveLightIndex() != -1) {
		//		light light1 = scene.GetActiveLight();
		//		colorShader.setUniform("lightPos", light1.GetPosVec());
		//		colorShader.setUniform("user_angle", light1.user_angle);
		//		colorShader.setUniform("lightTransform", light1.GetTransform());
		//		colorShader.setUniform("L_A", light1.L_A);
		//		colorShader.setUniform("L_D", light1.L_D);
		//		colorShader.setUniform("L_S", light1.L_S);
		//	}
		//	colorShader.setUniform("cameraPos", camera.GetPosition());
		//	colorShader.setUniformSampler("normalMap", 1);
		//	colorShader.setUniformSampler("cubemap", 2);
		//	// Set 'texture1' as the active texture at slot #	0
		//	texture1.bind(0);
		//	texture2.bind(1);
		//	
		//	

		//	// Drag our model's faces (triangles) in fill mode
		//	glBindVertexArray(camera.GetVAO());
		//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//	glBindVertexArray(currentModel.GetVAO());
		//	glDrawArrays(GL_TRIANGLES, 0, currentModel.GetModelVertices().size());
		//	glBindVertexArray(0);

		//	// Unset 'texture1' as the active texture at slot #0
		//	texture1.unbind(0);
		//	texture2.unbind(1);

		//	//colorShader.setUniform("color", glm::vec3(0, 0, 0));

		//	// Drag our model's faces (triangles) in line mode (wireframe)
		//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//	glBindVertexArray(currentModel.GetVAO());
		//	glDrawArrays(GL_TRIANGLES, 0, currentModel.GetModelVertices().size());
		//	glBindVertexArray(0);
		//	
		//}
	}

}

int Renderer::GetViewportWidth() const
{
	return viewport_width_;
}

int Renderer::GetViewportHeight() const
{
	return viewport_height_;
}




void Renderer::LoadShaders()
{
	colorShader.loadShaders("vshader.glsl", "fshader.glsl");
	shader.loadShaders("vcubemap.glsl", "fcubemap.glsl");
	skyboxShader.loadShaders("vskybox.glsl", "fskybox.glsl");
}



void Renderer::LoadTextures()
{
	if (!texture1.loadTexture("C://Users//Eitan//Documents//GitHub//computergraphics2021-eitan-and-hadar//computergraphics2021-eitan-and-hadar//Data//board.jpg", true))
	{
		//texture1.loadTexture("C://Users//Eitan//Documents//GitHub//computergraphics2021-eitan-and-hadar//computergraphics2021-eitan-and-hadar//Data//handGun_C.jpg", true);
		//texture1.loadTexture("C://Users//Eitan//Documents//GitHub//computergraphics2021-eitan-and-hadar//computergraphics2021-eitan-and-hadar//Data//handGun_N.jpg", true);
		//texture1.loadTexture("C://Users//Eitan//Documents//GitHub//computergraphics2021-eitan-and-hadar//computergraphics2021-eitan-and-hadar//Data//handGun_S.jpg", true);
		//texture1.loadTexture("C://Users//Eitan//Documents//GitHub//computergraphics2021-eitan-and-hadar//computergraphics2021-eitan-and-hadar//Data//normal_4k.jpg", true);
		texture1.loadTexture("C://Users//Eitan//Documents//GitHub//computergraphics2021-eitan-and-hadar//computergraphics2021-eitan-and-hadar//Data//board.jpg", true);
	}
	if (!texture2.loadTexture("C://Users//Eitan//Documents//GitHub//computergraphics2021-eitan-and-hadar//computergraphics2021-eitan-and-hadar//Data//Tex_0002.jpg", true))
	{
		texture2.loadTexture("C://Users//Eitan//Documents//GitHub//computergraphics2021-eitan-and-hadar//computergraphics2021-eitan-and-hadar//Data//Tex_0002.jpg", true);
		
	}


	GLfloat cubeVertices[] =
	{
		// Positions          // Texture Coords
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};
	//shader.use();
	// Setup cube VAO
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindVertexArray(0);

	//skyboxShader.use();
	// Setup skybox VAO
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);



	std::vector<std::string> faces
	{
		"C://Users//Eitan//Documents//GitHub//computergraphics2021-eitan-and-hadar//computergraphics2021-eitan-and-hadar//Data//posx.jpg",
		"C://Users//Eitan//Documents//GitHub//computergraphics2021-eitan-and-hadar//computergraphics2021-eitan-and-hadar//Data//negx.jpg",
		"C://Users//Eitan//Documents//GitHub//computergraphics2021-eitan-and-hadar//computergraphics2021-eitan-and-hadar//Data//posy.jpg",
		"C://Users//Eitan//Documents//GitHub//computergraphics2021-eitan-and-hadar//computergraphics2021-eitan-and-hadar//Data//negy.jpg",
		"C://Users//Eitan//Documents//GitHub//computergraphics2021-eitan-and-hadar//computergraphics2021-eitan-and-hadar//Data//posz.jpg",
		"C://Users//Eitan//Documents//GitHub//computergraphics2021-eitan-and-hadar//computergraphics2021-eitan-and-hadar//Data//negz.jpg"
	};
	cubeMapTexture = texture3.loadCubemap(faces);



}






//float temp = (brightness.x + brightness.y + brightness.z) / 3;
//if (temp > 1) {
//	brightness.x = 1;
//	brightness.y = 1;
//	brightness.z = 1;
//}
//
//if (temp > 0.75 && temp < 1) {
//	brightness.x = 0.75;
//	brightness.y = 0.75;
//	brightness.z = 0.75;
//}
//if (temp > 0.5 && temp < 0.75) {
//	brightness.x = 0.5;
//	brightness.y = 0.5;
//	brightness.z = 0.5;
//}
//if (temp > 0.25 && temp < 0.5) {
//	brightness.x = 0.25;
//	brightness.y = 0.25;
//	brightness.z = 0.25;
//}
//if (temp < 0.25) {
//	brightness.x = 0;
//	brightness.y = 0;
//	brightness.z = 0;
//}