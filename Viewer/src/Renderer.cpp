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
}



void Renderer::LoadTextures()
{
	if (!texture1.loadTexture("C://Users//Eitan//Documents//GitHub//computergraphics2021-eitan-and-hadar//computergraphics2021-eitan-and-hadar//Data//board.jpg", true))
	{
		//texture1.loadTexture("C://Users//Eitan//Documents//GitHub//computergraphics2021-eitan-and-hadar//computergraphics2021-eitan-and-hadar//Data//handGun_C.jpg", true);
		//texture1.loadTexture("C://Users//Eitan//Documents//GitHub//computergraphics2021-eitan-and-hadar//computergraphics2021-eitan-and-hadar//Data//handGun_N.jpg", true);
		//texture1.loadTexture("C://Users//Eitan//Documents//GitHub//computergraphics2021-eitan-and-hadar//computergraphics2021-eitan-and-hadar//Data//handGun_S.jpg", true);
		texture1.loadTexture("C://Users//Eitan//Documents//GitHub//computergraphics2021-eitan-and-hadar//computergraphics2021-eitan-and-hadar//Data//board.jpg", true);
	}
}



