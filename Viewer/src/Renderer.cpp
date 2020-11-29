#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

#include <iostream>

#include "Renderer.h"
#include "InitShader.h"
#include <glm\ext\matrix_transform.hpp>

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
	const glm::ivec2 p1(half_width, 0);
	const glm::ivec2 p2(half_width, viewport_height_);
	const glm::vec3 color(1, 0, 1);
	DrawLine(p1,  p2, color);
	const glm::ivec2 p3(0, half_height);
	const glm::ivec2 p4(viewport_width_, half_height);
	const glm::vec3 color1(1, 0, 1);
	DrawLine(p3, p4, color1);
	

	for (int i = 0; i < scene.GetModelCount(); i++) {
		
		DrawModel(scene.GetModel(i),scene);
	
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

void Renderer::DrawBoundingBox(MeshModel obj, glm::mat4x4 projection, Camera camera)
{
	double T = obj.getVerticeAtIndex(0)[1];
	double L = obj.getVerticeAtIndex(0)[0];
	double B = obj.getVerticeAtIndex(0)[1];
	double R = obj.getVerticeAtIndex(0)[0];
	double F = obj.getVerticeAtIndex(0)[2];
	double N = obj.getVerticeAtIndex(0)[2];

	for (int i = 0; i < obj.getVerticesSize(); i++) {

		glm::vec3 temp_new = obj.getVerticeAtIndex(i);
		if (temp_new[1] >= T)
			T = temp_new[1];
		if (temp_new[1] <= B)
			B = temp_new[1];

		if (temp_new[0] >= L)
			L = temp_new[0];
		if (temp_new[0] <= R)
			R = temp_new[0];

		if (temp_new[2] >= N)
			N = temp_new[2];
		if (temp_new[2] <= F)
			F = temp_new[2];
	}
	glm::vec3 center = glm::vec3((R + L) / 2, (T + B) / 2, (F + N) / 2);
	double scale = obj.GetScaleTransform()[0][0] / 1000;
	double range_Y = abs((T - B) / 2)+scale;
	double range_X = abs((R - L) / 2)+scale;
	double range_Z = abs((F - N) / 2)+scale;

	glm::vec3 x1 = glm::vec3(center[0] - range_X, center[1] + range_Y, center[2] + range_Z);
	glm::vec3 x2 = glm::vec3(center[0] - range_X, center[1] + range_Y, center[2] - range_Z);
	glm::vec3 x3 = glm::vec3(center[0] - range_X, center[1] - range_Y, center[2] - range_Z);
	glm::vec3 x4 = glm::vec3(center[0] - range_X, center[1] - range_Y, center[2] + range_Z);

	glm::vec3 x5 = glm::vec3(center[0] + range_X, center[1] + range_Y, center[2] + range_Z);
	glm::vec3 x6 = glm::vec3(center[0] + range_X, center[1] + range_Y, center[2] - range_Z);
	glm::vec3 x7 = glm::vec3(center[0] + range_X, center[1] - range_Y, center[2] - range_Z);
	glm::vec3 x8 = glm::vec3(center[0] + range_X, center[1] - range_Y, center[2] + range_Z);


	x1 = camera.GetViewPortTransformation(HomToCartesian(projection * glm::vec4(x1, 1)), viewport_width_, viewport_height_);
	x2 = camera.GetViewPortTransformation(HomToCartesian(projection * glm::vec4(x2, 1)), viewport_width_, viewport_height_);
	x3 = camera.GetViewPortTransformation(HomToCartesian(projection * glm::vec4(x3, 1)), viewport_width_, viewport_height_);
	x4 = camera.GetViewPortTransformation(HomToCartesian(projection * glm::vec4(x4, 1)), viewport_width_, viewport_height_);
	x5 = camera.GetViewPortTransformation(HomToCartesian(projection * glm::vec4(x5, 1)), viewport_width_, viewport_height_);
	x6 = camera.GetViewPortTransformation(HomToCartesian(projection * glm::vec4(x6, 1)), viewport_width_, viewport_height_);
	x7 = camera.GetViewPortTransformation(HomToCartesian(projection * glm::vec4(x7, 1)), viewport_width_, viewport_height_);
	x8 = camera.GetViewPortTransformation(HomToCartesian(projection * glm::vec4(x8, 1)), viewport_width_, viewport_height_);


	DrawLine(x5, x6, glm::vec3(1, 0, 1));
	DrawLine(x1, x2, glm::vec3(1, 0, 1));
	DrawLine(x1, x5, glm::vec3(1, 0, 1));
	DrawLine(x2, x6, glm::vec3(1, 0, 1));

	DrawLine(x3, x4, glm::vec3(1, 0, 1));
	DrawLine(x4, x8, glm::vec3(1, 0, 1));
	DrawLine(x7, x8, glm::vec3(1, 0, 1));
	DrawLine(x3, x7, glm::vec3(1, 0, 1));

	DrawLine(x2, x3, glm::vec3(1, 0, 1));
	DrawLine(x1, x4, glm::vec3(1, 0, 1));
	DrawLine(x6, x7, glm::vec3(1, 0, 1));
	DrawLine(x5, x8, glm::vec3(1, 0, 1));


}

void Renderer::DrawModel(MeshModel obj,Scene scene)
{
	
	//std::cout << scene.GetModelCount() << std::endl;
	//std::cout << "1" << std::endl;

	Camera camera = scene.GetActiveCamera();
	//std::cout << "2" << std::endl;
	glm::mat4x4 perspective = scene.GetPerspectiveTransform();
	glm::mat4x4 ortho = scene.GetOrthographicTransform();
	glm::mat4x4 projection = scene.GetProjection();
	glm::mat4x4 normal_projection = camera.GetCameraTransform() * obj.GetTransform();
	
	glm::mat4 temp2 = glm::mat4(1);
	temp2[2][2] = -1;
	//
	//DrawBoundingBox(obj, projection, camera);
	//Draw_Square(obj, camera, projection , normal_projection);
	
	for (int j = 0; j < obj.getVerticesSize(); j++) {
		glm::vec3& currentVer = obj.getVerticeAtIndex(j);
		glm::vec4 temp =  projection * glm::vec4(currentVer, 1);
		//glm::vec4 temp3 = obj.GetTransform() * glm::vec4(currentVer, 1);
		
		currentVer = HomToCartesian(temp);
		currentVer = camera.GetViewPortTransformation(currentVer, viewport_width_, viewport_height_);
	}

	for (int i = 0; i < obj.GetFacesCount(); i++) {
		Face face = obj.GetFace(i);
		int point0 = face.GetVertexIndex(0) - 1;
		int point1 = face.GetVertexIndex(1) - 1;
		int point2 = face.GetVertexIndex(2) - 1;

		glm::vec2 p1(obj.getVerticeAtIndex(point0)[0], obj.getVerticeAtIndex(point0)[1]);
		glm::vec2 p2(obj.getVerticeAtIndex(point1)[0], obj.getVerticeAtIndex(point1)[1]);
		glm::vec2 p3(obj.getVerticeAtIndex(point2)[0], obj.getVerticeAtIndex(point2)[1]);


		//std::cout << "MODEL_X " << p1[0] << " MODEL_Y" << p1[1] << std::endl;
		
		DrawLine(p1, p2, glm::vec3(1, 0, 1));
		DrawLine(p1, p3, glm::vec3(1, 0, 1));	
		DrawLine(p2, p3, glm::vec3(1, 0, 1));
	}
}


//compute nurmal faces
glm::vec3 normal(glm::vec3 x1, glm::vec3 x2, glm::vec3 x3)
{
	
	glm::vec3 temp1 = x1 - x2;
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

glm::vec3 new_normals(glm::vec3 point0, glm::vec3 point1, glm::vec3 point2) {
	float normal_x = ((point1[1] - point0[1])
		* (point2[2] - point0[2]))
		- ((point1[2] - point0[2])
			* (point2[1] - point0[1]));

	float normal_y = ((point1[2] - point0[2])
		* (point2[0] - point0[0]))
		- ((point1[0] - point0[0])
			* (point2[2] - point0[2]));

	float normal_z = ((point1[0] - point0[0])
		* (point2[1] - point0[1])
		- ((point1[1] - point0[1])
			* (point2[0] - point0[0])));
	
	return glm::vec3(normal_x, normal_y, normal_z);
}

glm::vec3 To3(glm::vec4 vec) {
	return glm::vec3(vec[0],vec[1], vec[2]);
}

void To2(glm::vec3& vec) {
	glm::vec3(vec[0]/vec[2], vec[1]/vec[2],vec[3]);
}

void Renderer::Draw_Square(MeshModel obj, Camera camera,glm::mat4x4 projection,glm::mat4x4 normal_projection)
{
	
	
	double scale;
	
	for (int i = 0; i < obj.GetFacesCount(); i++) {
		Face face = obj.GetFace(i);

		int point0 = face.GetVertexIndex(0) - 1;
		int point1 = face.GetVertexIndex(1) - 1;
		int point2 = face.GetVertexIndex(2) - 1;
		int norm0 = face.GetNormalIndex(0) - 1;
		int norm1 = face.GetNormalIndex(1) - 1;
		int norm2 = face.GetNormalIndex(2) - 1;
		std::vector<glm::vec3> normals = obj.GetNormals();
		glm::vec3 temp = normal(To3(normal_projection* glm::vec4(obj.getVerticeAtIndex(point0),1)),
			To3(normal_projection * glm::vec4(obj.getVerticeAtIndex(point1), 1)),
			To3(normal_projection * glm::vec4(obj.getVerticeAtIndex(point2), 1)));

		
		//glm::vec3 temp = normal(obj.getVerticeAtIndex(point0), obj.getVerticeAtIndex(point1), obj.getVerticeAtIndex(point2));
		glm::vec3 ver1 = HomToCartesian(projection * glm::vec4(obj.getVerticeAtIndex(point0), 1));
		glm::vec3 ver2 = HomToCartesian(projection * glm::vec4(obj.getVerticeAtIndex(point1), 1));
		glm::vec3 ver3 = HomToCartesian(projection * glm::vec4(obj.getVerticeAtIndex(point2), 1));

		ver1 = camera.GetViewPortTransformation(ver1, viewport_width_, viewport_height_);
		ver2= camera.GetViewPortTransformation(ver2, viewport_width_, viewport_height_);
		ver3 = camera.GetViewPortTransformation(ver3, viewport_width_, viewport_height_);
		//std::cout << ver1[0] << std::endl;

		float x = (ver1[0] + ver2[0] +ver3[0]) / 3;
		float y = (ver1[1] + ver2[1] +ver3[1]) / 3;
		float z = (ver1[2] + ver2[2] + ver3[2]) / 3;
		scale = obj.GetScaleTransform()[0][0] / 10;

		DrawLine(glm::vec2(x, y), glm::vec2(x + (temp[0] * scale), y + (temp[1] * scale)), glm::vec3(1, 0, 1));
		glm::vec3 temp_new = glm::vec3(x + (temp[0] * scale),y + (temp[1] * scale),z + (temp[2] * scale));
		//std::cout << temp_new[0] << "," << temp_new[1] << "," << temp_new[2] << std::endl;

	}
	scale = obj.GetScaleTransform()[0][0] / 12;
	for (int i = 0; i < obj.getVerticesSize(); i++) {

		glm::vec3 norm = obj.GetNormals()[i];
		glm::vec3 ver1 = camera.GetViewPortTransformation(HomToCartesian(projection * glm::vec4(obj.getVerticeAtIndex(i), 1)),viewport_width_,viewport_height_);
		DrawLine(glm::vec2(ver1[0], ver1[1]), glm::vec2(ver1[0] + (norm[0] * scale), ver1[1] + (norm[1] * scale)), glm::vec3(1,  0, 1));





	}
	

}



glm::vec3 Renderer::HomToCartesian(glm::vec4 vec)
{

	if (vec[3] == 0) {
		return glm::vec3(vec[0], vec[1], vec[2]);

	}
	return glm::vec3(vec[0] / vec[3], vec[1] / vec[3], vec[2] / vec[3]);
}


