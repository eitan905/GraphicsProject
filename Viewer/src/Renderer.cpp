#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

#include <iostream>

#include "Renderer.h"
#include "InitShader.h"

#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)
#define Z_INDEX(width,x,y) ((x)+(y)*(width))

Renderer::Renderer(int viewport_width, int viewport_height) :
	viewport_width_(viewport_width),
	viewport_height_(viewport_height)
{
	InitOpenGLRendering();
	CreateBuffers(viewport_width, viewport_height);
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

void Renderer::DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color)
{
	int delta_x, delta_y, x_1, y_1, x_2, y_2, distance_to_line, doubled_delta_y,doubled_delta_x, doubled_delta_x_y, distance_to_end = 0,direction_x,direction_y,flag=0;

	x_1 = p1.x;
	y_1 = p1.y;
	x_2 = p2.x;
	y_2 = p2.y;

	//compute delta_x and delta_y
	delta_x = abs(x_2 - x_1);
	delta_y = abs(y_2 - y_1);

	//compute move_x,move_y
	if (x_1 < x_2) direction_x = 1;
	else direction_x = -1;

	if (y_1 < y_2) direction_y = 1;
	else direction_y = -1;

	if (delta_x == 0) {
		direction_x = 0;
	}
	
	//compute the step of the while

	if (delta_x < delta_y) {
		distance_to_end = delta_y;
		flag = 1;
	}
	else {
		distance_to_end = delta_x;
	}
	

	doubled_delta_y = 2 * delta_y;
	doubled_delta_x = 2 * delta_x;
	doubled_delta_x_y = doubled_delta_y - doubled_delta_x;
	//load (x_0,y_0) pixel
	PutPixel(x_1, y_1, color);

	//compute some const
	distance_to_line = 2 * delta_y - delta_x;
	while (distance_to_end > 0&&flag==0) {
		if (distance_to_line < 0 ) {
			x_1 += direction_x;
			distance_to_line = distance_to_line + doubled_delta_y;
		}
		else {
			x_1+= direction_x;
			y_1+= direction_y;
			distance_to_line = distance_to_line + doubled_delta_x_y;
		}
		PutPixel(x_1, y_1, color);
		distance_to_end--;
	}


	//compute some const
	distance_to_line = 2 * delta_x - delta_y;
	doubled_delta_x_y = doubled_delta_x - doubled_delta_y;
	while (distance_to_end > 0 && flag == 1) {
		if (distance_to_line < 0) {
			y_1 += direction_y;
			distance_to_line = distance_to_line + doubled_delta_x;
		} 
		else {
			y_1 += direction_y;
			x_1 += direction_x;
			distance_to_line = distance_to_line + doubled_delta_x_y;
		}
		PutPixel(x_1, y_1, color);
		distance_to_end--;
	}
}



void Renderer::CreateBuffers(int w, int h)
{
	CreateOpenGLBuffer(); //Do not remove this line.
	color_buffer_ = new float[3 * w * h];
	ClearColorBuffer(glm::vec3(0.0f, 0.0f, 0.0f));
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
	const GLfloat vtc[]={
		-1, -1,
		 1, -1,
		-1,  1,
		-1,  1,
		 1, -1,
		 1,  1
	};

	const GLfloat tex[]={
		0,0,
		1,0,
		0,1,
		0,1,
		1,0,
		1,1};

	// Makes this buffer the current one.
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// This is the opengl way for doing malloc on the gpu. 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc)+sizeof(tex), NULL, GL_STATIC_DRAW);

	// memcopy vtc to buffer[0,sizeof(vtc)-1]
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);

	// memcopy tex to buffer[sizeof(vtc),sizeof(vtc)+sizeof(tex)]
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);

	// Loads and compiles a sheder.
	GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );

	// Make this program the current one.
	glUseProgram(program);

	// Tells the shader where to look for the vertex position data, and the data dimensions.
	GLint  vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition,2,GL_FLOAT,GL_FALSE,0,0 );

	// Same for texture coordinates data.
	GLint  vTexCoord = glGetAttribLocation( program, "vTexCoord" );
	glEnableVertexAttribArray( vTexCoord );
	glVertexAttribPointer( vTexCoord,2,GL_FLOAT,GL_FALSE,0,(GLvoid *)sizeof(vtc) );

	//glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );

	// Tells the shader to use GL_TEXTURE0 as the texture id.
	glUniform1i(glGetUniformLocation(program, "texture"),0);

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

void Renderer::Render(const Scene& scene)
{
	// TODO: Replace this code with real scene rendering code
	int half_width = viewport_width_ / 2;
	int half_height = viewport_height_ / 2;
	int thickness = 15;

	DrawModel(scene.GetModel(0));

	if (scene.GetModelCount() == 2)
	{
		DrawModel(scene.GetModel(1));
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

void Renderer::UseDrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color)
{
	DrawLine(p1, p2, color);
}

void Renderer::DrawModel(MeshModel obj)
{
	for (int j = 0; j < obj.getVerticesSize(); j++) {
		glm::vec4 temp = obj.GetTransform() *
			glm::vec4(obj.getVerticeAtIndex(j),1);
		obj.getVerticeAtIndex(j)[0] = temp[0];
		obj.getVerticeAtIndex(j)[1] = temp[1];
		obj.getVerticeAtIndex(j)[2] = temp[2];
	}
	for (int i = 0; i < obj.GetFacesCount(); i++) {
		Face face = obj.GetFace(i);
		int point0 = face.GetVertexIndex(0) - 1;
		int point1 = face.GetVertexIndex(1) - 1;
		int point2 = face.GetVertexIndex(2) - 1;

		glm::vec2 p1(obj.getVerticeAtIndex(point0)[0], obj.getVerticeAtIndex(point0)[1]);
		glm::vec2 p2(obj.getVerticeAtIndex(point1)[0], obj.getVerticeAtIndex(point1)[1]);
		glm::vec2 p3(obj.getVerticeAtIndex(point2)[0], obj.getVerticeAtIndex(point2)[1]);



		DrawLine(p1, p2, glm::vec3(1, 0, 1));
		DrawLine(p1, p3, glm::vec3(1, 0, 1));
		DrawLine(p2, p3, glm::vec3(1, 0, 1));
	}
}
