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

int min(int x, int y) {
	return x < y ? x : y;
}
int max(int x, int y) {
	return x > y ? x : y;
}

void DrawTriangle() {

	glm::vec3 p1 = glm::vec3(100, 100, 100);
	glm::vec3 p2 = glm::vec3(300, 100, 100);
	glm::vec3 p3 = glm::vec3(200, 200, 100);



}
/*
glm::vec3 Renderer::Convolution(glm::vec3 color_of_pt, glm::mat3x3 color_neighbers )
{
	float sum=0;
	glm::mat3x3 result = Gaussian_kernal * neighbers;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			sum += result[i, j] * Gaussian_kernal[i, j];
		}
	}
	return sum;
}
*/

glm::vec3 Renderer::Point_color_in_fog( glm::vec3 color_of_pt, float distance, float fog_density)
{
	float fog_color; 
	fog_color = Fog_color(distance, fog_density);
	return glm::vec3((fog_color + color_of_pt[0]) / 2, (fog_color + color_of_pt[1]) / 2, (fog_color + color_of_pt[2]) / 2);
}

float Renderer::Fog_color(float distance, float fog_density)
{
	return exp( -(distance * fog_density));
}

void Renderer::DrawLight(light light1)
{
	glm::mat4 posMat = light1.GetTransform();
	glm::vec3 pos(posMat[0][0], posMat[1][1], posMat[2][2]);
	float scale = pos[2]/15;
	for (int x = pos[0] - scale; x < pos[0] + scale; x++) {
		for (int y = pos[1] - scale; y < pos[1] + scale; y++) {
			PutPixel(x, y, glm::vec3(1, 1, 1));
		}
	}


}

float Renderer::area(float x1, float y1, float x2, float y2, float x3, float y3)
{
	return abs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0);
}
double Renderer::Linear_Interpolation_color(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, float color_v1, float color_v2, float color_v3, glm::vec3 pt) {
	double A_1 = area(v2[0], v2[1], v3[0], v3[1], pt[0], pt[1]);
	double A_2 = area(v1[0], v1[1], v3[0], v3[1], pt[0], pt[1]);
	double A_3 = area(v2[0], v2[1], v1[0], v1[1], pt[0], pt[1]);
	double A = A_1 + A_2 + A_3;
	float z = (A_1 / A) * color_v1 + (A_2 / A) * color_v2 + (A_3 / A) * color_v3;
	
	return z;
}
glm::vec3 Renderer::Flat_shading(light& light_source, MeshModel& mesh,glm::vec3 normal_of_polygon,int  user_angle, glm::vec3 camToPoint)
{
	
	//light_source.Set_N(normal_of_polygon);
	return light_source.Final_light(mesh.K_A, mesh.K_D, mesh.K_S, user_angle,camToPoint);
}
glm::vec3 To3(glm::vec4 vec) {
	return glm::vec3(vec[0], vec[1], vec[2]);
}


glm::vec3 Renderer::Gouraud_shading_for_vertix(light light_source, MeshModel mesh)
{

	return light_source.Final_light_gouraud(mesh.K_A, mesh.K_D, mesh.K_S);
}

glm::vec3 Renderer::Gouraud_shading_for_point_in_polygon(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3,glm::vec3 color_p1, glm::vec3 color_p2, glm::vec3 color_p3, glm::vec3 pt)
{
	/*std::cout << "----------------------------------------" << std::endl;
	std::cout << color_p1[0] << "," << color_p1[1] << "," << color_p1[2] << std::endl;
	std::cout << color_p2[0] << "," << color_p2[1] << "," << color_p2[2] << std::endl;
	std::cout << color_p3[0] << "," << color_p3[1] << "," << color_p3[2] << std::endl;*/
	float R_pt, G_pt, B_pt;
	R_pt = Linear_Interpolation_color(p1, p2, p3, color_p1[0], color_p2[0], color_p3[0], pt);
	G_pt = Linear_Interpolation_color(p1, p2, p3, color_p1[1], color_p2[1], color_p3[1], pt);
	B_pt = Linear_Interpolation_color(p1, p2, p3, color_p1[2], color_p2[2], color_p3[2], pt);
	glm::vec3 final = (glm::vec3(R_pt, G_pt, B_pt));
	//glm::vec3 final_normalized = glm::normalize(final);
	/*std::cout << final[0] << "," << final[1] << "," << final[2] << std::endl;
	std::cout << final_normalized[0] << "," << final_normalized[1] << "," << final_normalized[2] << std::endl;
	std::cout << "----------------------------------------" << std::endl;*/

	return final;
}




//phong shading for vertix =Gouraud_shading_for_vertix
//this is Phong_shading for point in polygon
glm::vec3 Renderer::Phong_shading(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 normal_p1, glm::vec3 normal_p2, glm::vec3 normal_p3, glm::vec3 pt, light light_source, MeshModel mesh, int  user_angle)
{
	glm::vec3 normal_of_polygon= glm::vec3(0,0,0);
	normal_of_polygon[0] = Linear_Interpolation_by_choice(p1,p2,p3,pt, normal_p1[0], normal_p2[0], normal_p3[0]);
	normal_of_polygon[1] = Linear_Interpolation_by_choice(p1, p2, p3, pt, normal_p1[1], normal_p2[1], normal_p3[1]);
	normal_of_polygon[2] = Linear_Interpolation_by_choice(p1, p2, p3, pt, normal_p1[2], normal_p2[2], normal_p3[2]);
	light_source.Set_N(glm::normalize(normal_of_polygon));
	return light_source.Final_light(mesh.K_A, mesh.K_D, mesh.K_S, user_angle,glm::vec3(1,1,1));
}

Renderer::Renderer(int viewport_width, int viewport_height) :
	viewport_width_(viewport_width),     
	viewport_height_(viewport_height)
{
	InitOpenGLRendering();
	CreateBuffers(viewport_width, viewport_height);
}

void Renderer::Draw_Gouraud(glm::vec3 colorP1, glm::vec3 colorP2, glm::vec3 colorP3, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3
	, Camera& camera, Scene& scene, MeshModel& mesh, glm::vec3 p1_normal, glm::vec3 p2_normal, glm::vec3 p3_normal, glm::vec3 realP1, glm::vec3 realP2, glm::vec3 realP3)
{

	int minX = max(min(p1[0], min(p2[0], p3[0])), 1);
	int minY = max(min(p1[1], min(p2[1], p3[1])), 1);
	int maxX = min(max(p1[0], max(p2[0], p3[0])), viewport_width_ - 1);
	int maxY = min(max(p1[1], max(p2[1], p3[1])), viewport_height_ - 1);

	
	glm::vec3 final_light;
	glm::vec3 pointToCam;
	glm::vec3 cameraPos = camera.GetPosition();
	light light1 = scene.GetActiveLight();
	glm::vec3 lightPos = light1.GetPosVec();
	
	if (scene.GetShading() != "Phong") {


		pointToCam = glm::vec3(cameraPos[0] - realP1[0], cameraPos[1] - realP1[1], cameraPos[2] - realP1[2]);
		pointToCam = glm::normalize(pointToCam);
		light1.Set_N(p1_normal);
		light1.Set_I(glm::normalize(glm::vec3(lightPos[0] - realP1[0], lightPos[1] - realP1[1], lightPos[2] - realP1[2])));
		light1.Set_V(pointToCam);
		colorP1 = Flat_shading(light1, mesh,p1_normal,1,pointToCam);
		//std::cout << colorP1[0] << ",";



		pointToCam = glm::vec3(cameraPos[0] - realP2[0], cameraPos[1] - realP2[1], cameraPos[2] - realP2[2]);
		pointToCam = glm::normalize(pointToCam);
		light1.Set_I(glm::normalize(glm::vec3(lightPos[0] - realP2[0], lightPos[1] - realP2[1], lightPos[2] - realP2[2])));
		light1.Set_V(pointToCam);
		light1.Set_N(p2_normal);

		colorP2 = Flat_shading(light1, mesh, p1_normal, 1, pointToCam);
		//std::cout << colorP2[0] << ",";




		pointToCam = glm::vec3(cameraPos[0] - realP3[0], cameraPos[1] - realP3[1], cameraPos[2] - realP3[2]);
		pointToCam = glm::normalize(pointToCam);
		light1.Set_I(glm::normalize(glm::vec3(lightPos[0] - realP3[0], lightPos[1] - realP3[1], lightPos[2] - realP3[2])));
		light1.Set_V(pointToCam);
		light1.Set_N(p3_normal);

		colorP3 = Flat_shading(light1, mesh, p1_normal, 1, pointToCam);
		//std::cout << colorP3[0] << std::endl;

	}


	for (int y = minY; y <= maxY; y++)
	{
		for (int x = minX; x <= maxX; x++)
		{

			if (PointInTriangle(glm::vec2(x, y), p1, p2, p3)) {
				float z = Linear_Interpolation(realP1,realP2, realP3, glm::vec2(x, y));
				if (1) {
					if (z < camera.zNear && z > camera.zFar && z > z_buffer[Z_INDEX(viewport_width_, x, y)]) {
						z_buffer[Z_INDEX(viewport_width_, x, y)] = z;
						if (scene.GetShading() == "Phong") {
							final_light = Phong_shading(p1, p2, p3, p1_normal, p2_normal, p3_normal, glm::vec3(x, y, z), light1, mesh, 1);
						}
						else {
							final_light = Gouraud_shading_for_point_in_polygon(p1, p2, p3, colorP1, colorP2, colorP3, glm::vec3(x, y, z));
							/*std::cout << p1[0] << "," << p2[0] << "," << p3[0] << std::endl;
							std::cout << p1[1] << "," << p2[1] << "," << p3[1] << std::endl;
							std::cout << x << "," << y << std::endl;
							std::cout << final_light[0] << std::endl;*/
						}
						//temp = Point_color_in_fog(color, c,1.3);
						if (mesh.GetModelName() == "Sphere.obj") {
							PutPixel(x, y, glm::vec3(1, 1, 1));
						}
						else {
							PutPixel(x, y, final_light);
						}
					}
				}
			}
		}
	}
	
	
	

}


Renderer::~Renderer()
{
	delete[] color_buffer_;
	delete[] z_buffer;
}





void Renderer::PutPixel(int i, int j, const glm::vec3& color)
{
	if (i < 0) return; if (i >= viewport_width_) return;
	if (j < 0) return; if (j >= viewport_height_) return;
	
	color_buffer_[INDEX(viewport_width_, i, j, 0)] = color.x;
	color_buffer_[INDEX(viewport_width_, i, j, 1)] = color.y;
	color_buffer_[INDEX(viewport_width_, i, j, 2)] = color.z;
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
double Renderer::Linear_Interpolation_by_choice(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec2 pt, float choice_v1, float choice_v2, float choice_v3) {
	double A_1 = area(v2[0], v2[1], v3[0], v3[1], pt[0], pt[1]);
	double A_2 = area(v1[0], v1[1], v3[0], v3[1], pt[0], pt[1]);
	double A_3 = area(v2[0], v2[1], v1[0], v1[1], pt[0], pt[1]);
	double A = A_1 + A_2 + A_3;
	float z = (A_1 / A) * choice_v1 + (A_2 / A) * choice_v2 + (A_3 / A) * choice_v3;

	return z;
}

double Renderer::Linear_Interpolation(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec2 pt) {
	double A_1 = area(v2[0], v2[1], v3[0], v3[1], pt[0], pt[1]);
	double A_2 = area(v1[0], v1[1], v3[0], v3[1], pt[0], pt[1]);
	double A_3 = area(v2[0], v2[1], v1[0], v1[1], pt[0], pt[1]);
	double A = A_1 + A_2 + A_3;
	float z = (A_1 / A) * v1[2] + (A_2 / A) * v2[2] + (A_3 / A) * v3[2];

	return z;
}

void Renderer::CreateBuffers(int w, int h)
{
	CreateOpenGLBuffer(); //Do not remove this line.
	color_buffer_ = new float[3 * w * h];
	z_buffer = new float[w * h];
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
			z_buffer[Z_INDEX(viewport_width_, i, j)] = -INFINITY;
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
		DrawModel(scene.GetModel(i), scene, scene.GetModel(i).color);
	}
	for (int i = 0; i < scene.GetCameraCount(); i++) {
		if (scene.GetActiveCameraIndex() != i) {
			DrawCamera(scene.GetCameraAtIndex(i),scene);
		}
	}
	if (scene.GetActiveLightIndex() != -1) {

		DrawModel(scene.GetActiveLight(),scene, glm::vec3(1,1,1));
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
	obj.getPosition() = center;
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


void Renderer::DrawCamera(Camera cameraobj, Scene scene)
{

	//std::cout << scene.GetModelCount() << std::endl;
	//std::cout << "1" << std::endl;
	float tempZ;
	Camera camera = scene.GetActiveCamera();
	//std::cout << "2" << std::endl;
	glm::mat4x4 perspective = scene.GetPerspectiveTransform(cameraobj,tempZ);
	glm::mat4x4 ortho = scene.GetOrthographicTransform(cameraobj, tempZ);
	glm::mat4x4 projection = scene.GetProjection(cameraobj, tempZ);
	glm::mat4x4 normal_projection = camera.GetCameraTransform() * cameraobj.GetTransform();

	glm::mat4 temp2 = glm::mat4(1);
	temp2[2][2] = -1;
	//
	

	for (int j = 0; j < cameraobj.getVerticesSize(); j++) {
		glm::vec3& currentVer = cameraobj.getVerticeAtIndex(j);
		glm::vec4 temp = projection * glm::vec4(currentVer, 1);

		currentVer = HomToCartesian(temp);
		currentVer = camera.GetViewPortTransformation(currentVer, viewport_width_, viewport_height_);
	}

	for (int i = 0; i < cameraobj.GetFacesCount(); i++) {
		Face face = cameraobj.GetFace(i);
		int point0 = face.GetVertexIndex(0) - 1;
		int point1 = face.GetVertexIndex(1) - 1;
		int point2 = face.GetVertexIndex(2) - 1;

		glm::vec2 p1(cameraobj.getVerticeAtIndex(point0)[0], cameraobj.getVerticeAtIndex(point0)[1]);
		glm::vec2 p2(cameraobj.getVerticeAtIndex(point1)[0], cameraobj.getVerticeAtIndex(point1)[1]);
		glm::vec2 p3(cameraobj.getVerticeAtIndex(point2)[0], cameraobj.getVerticeAtIndex(point2)[1]);


		//std::cout << "MODEL_X " << p1[0] << " MODEL_Y" << p1[1] << std::endl;

		DrawLine(p1, p2, glm::vec3(1, 0, 1));
		DrawLine(p1, p3, glm::vec3(1, 0, 1));
		DrawLine(p2, p3, glm::vec3(1, 0, 1));
	}
}



void Renderer::DrawModel(MeshModel obj,Scene scene,glm::vec3 color)
{
	
	//std::cout << scene.GetModelCount() << std::endl;
	//std::cout << "1" << std::endl;
	MeshModel realMesh(obj);
	Camera camera = scene.GetActiveCamera();
	//std::cout << "2" << std::endl;
	float tempZ;
	glm::mat4x4 perspective = camera.GetPerspectiveNormalization();
	glm::mat4x4 ortho = camera.GetOrthoNormalization();
	glm::mat4x4 projection = scene.GetProjection(obj,tempZ);
	glm::mat4x4 normal_projection = camera.GetCameraTransform() * obj.GetTransform();
	glm::mat4x4 divideZ = camera.GetProjectionTransformation();
	
	
	if (scene.GetActiveLightIndex() != -1) {
		light& light1 = scene.GetActiveLight();
		Camera& camera2 = scene.GetActiveCamera();

		light1.SetPos((scene.GetProjection(light1,tempZ) * glm::vec4(light1.GetVerAtIndex(1), 1)));
		camera2.SetPos(camera.GetCameraTransform() * glm::vec4(camera.getVerticeAtIndex(1), 1));

	}
	/*if (scene.GetActiveLightIndex() != -1) {
		light& light1 = scene.GetActiveLight();
		light1.SetPos(glm::vec4(light1.GetVerAtIndex(1), 1));
		Camera& camera2 = scene.GetActiveCamera();
		camera2.SetPos(glm::vec3(0,0,0));

	}*/
	glm::mat4 temp2 = glm::mat4(1);
	temp2[2][2] = -1;
	//
	if (scene.displayBox) {
		DrawBoundingBox(obj, projection, camera);
	}
	if (scene.displayNormals) {
		Draw_Normals(obj, camera, projection, normal_projection);
	}
	for (int j = 0; j < obj.getVerticesSize(); j++) {
		glm::vec3& currentVer = obj.getVerticeAtIndex(j);
		glm::vec3& currentRealVer = realMesh.getVerticeAtIndex(j);




		currentRealVer = realMesh.GetTransform() * glm::vec4(currentRealVer,1);



		glm::vec4 temp =  projection * glm::vec4(currentVer, 1);
		if (camera.GetActiveProjection() == 0) {
			tempZ = temp[2];
			temp = divideZ * perspective * temp;
		}
		if (camera.GetActiveProjection() == 1) {
			tempZ = temp[2];
			temp = ortho * temp;

		}
		currentVer = HomToCartesian(temp);
		currentVer[2] = tempZ;
		
		currentVer = camera.GetViewPortTransformation(currentVer, viewport_width_, viewport_height_);
		
	}
	

	for (int i = 0; i < obj.GetFacesCount(); i++) {
		
		Face face = obj.GetFace(i);
		int point0 = face.GetVertexIndex(0) - 1;
		int point1 = face.GetVertexIndex(1) - 1;
		int point2 = face.GetVertexIndex(2) - 1;

		glm::vec3 p1(obj.getVerticeAtIndex(point0)[0], obj.getVerticeAtIndex(point0)[1], obj.getVerticeAtIndex(point0)[2]);
		glm::vec3 p2(obj.getVerticeAtIndex(point1)[0], obj.getVerticeAtIndex(point1)[1], obj.getVerticeAtIndex(point1)[2]);
		glm::vec3 p3(obj.getVerticeAtIndex(point2)[0], obj.getVerticeAtIndex(point2)[1], obj.getVerticeAtIndex(point2)[2]);
		
		glm::vec3 p1_normal;
		glm::vec3 p2_normal;
		glm::vec3 p3_normal;


		if (!scene.GetNormals()) {
			p1_normal = glm::normalize(obj.GetTransform() * glm::vec4(obj.GetNormalAtIndex(face.GetNormalIndex(0) - 1), 1));
			p2_normal = glm::normalize(obj.GetTransform() * glm::vec4(obj.GetNormalAtIndex(face.GetNormalIndex(1) - 1), 1));
			p3_normal = glm::normalize(obj.GetTransform() * glm::vec4(obj.GetNormalAtIndex(face.GetNormalIndex(2) - 1), 1));
		}
		else {
			p1_normal = glm::normalize(camera.GetViewPortTransformation(HomToCartesian(divideZ * perspective * camera.GetCameraTransform() * obj.GetTransform() * glm::vec4(obj.GetNormalAtIndex(face.GetNormalIndex(0) - 1), 1)), viewport_width_, viewport_height_));
			p2_normal = glm::normalize(camera.GetViewPortTransformation(HomToCartesian(divideZ * perspective * camera.GetCameraTransform() * obj.GetTransform() * glm::vec4(obj.GetNormalAtIndex(face.GetNormalIndex(1) - 1), 1)), viewport_width_, viewport_height_));
			p3_normal = glm::normalize(camera.GetViewPortTransformation(HomToCartesian(divideZ * perspective * camera.GetCameraTransform() * obj.GetTransform() * glm::vec4(obj.GetNormalAtIndex(face.GetNormalIndex(2) - 1), 1)), viewport_width_, viewport_height_));
		}

		/*p1_normal[0] = -p1_normal[0];
		p1_normal[1] = -p1_normal[1];
		p1_normal[2] = -p1_normal[2];

		p2_normal[0] = -p2_normal[0];
		p2_normal[1] = -p2_normal[1];
		p2_normal[2] = -p2_normal[2];

		p3_normal[0] = -p3_normal[0];
		p3_normal[1] = -p3_normal[1];
		p3_normal[2] = -p3_normal[2];*/

		glm::vec3 realP1(realMesh.getVerticeAtIndex(point0)[0], realMesh.getVerticeAtIndex(point0)[1], realMesh.getVerticeAtIndex(point0)[2]);
		glm::vec3 realP2(realMesh.getVerticeAtIndex(point1)[0], realMesh.getVerticeAtIndex(point1)[1], realMesh.getVerticeAtIndex(point1)[2]);
		glm::vec3 realP3(realMesh.getVerticeAtIndex(point2)[0], realMesh.getVerticeAtIndex(point2)[1], realMesh.getVerticeAtIndex(point2)[2]);




		
		//std::cout << ver1[0] << std::endl;
		if (scene.GetActiveLightIndex() != -1 && scene.GetShading() == "Gouraud" || scene.GetShading() == "Phong") {
			Draw_Gouraud(color, color, color, p1, p2, p3, camera, scene, obj, p1_normal, p2_normal, p3_normal, realP1, realP2, realP3);

		}
		if (scene.GetShading() == "Flat") {
			FloodFillUtil(p1[0], p1[1], color, p1, p2, p3, camera, scene, obj, glm::vec3(1, 1, 1), realP1, realP2, realP3);
		}
	}	
	
}


float Renderer::sign(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3)
{
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool Renderer::PointInTriangle(glm::vec2 pt, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3)
{
	float d1, d2, d3;
	bool has_neg, has_pos;

	d1 = sign(pt, v1, v2);
	d2 = sign(pt, v2, v3);
	d3 = sign(pt, v3, v1);

	has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
	has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

	return !(has_neg && has_pos);
}

void Renderer::FloodFillUtil(int x, int y, glm::vec3 color, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, Camera& camera, Scene& scene, MeshModel& mesh, glm::vec3 face_normal,glm::vec3 realP1, glm::vec3 realP2, glm::vec3 realP3)
{
	
	
	int minX = max(min(p1[0], min(p2[0], p3[0])), 1);
	int minY = max(min(p1[1], min(p2[1], p3[1])), 1);
	int maxX = min(max(p1[0], max(p2[0], p3[0])), viewport_width_-1);
	int maxY = min(max(p1[1], max(p2[1], p3[1])), viewport_height_-1);
	float c;
	glm::vec3 temp = color;
	glm::vec3 after_fog;

	if (mesh.GetModelName() != "Sphere.obj") {

		glm::vec3 center((realP1[0] + realP2[0] + realP3[0]) / 3, (realP1[2] + realP2[2] + realP3[2]) / 3, (realP1[2] + realP2[2] + realP3[2]) / 3);
		//glm::vec3 center((p1[0] + p2[0] + p3[0]) / 3, (p1[2] + p2[2] + p3[2]) / 3, (p1[2] + p2[2] + p3[2]) / 3);
		glm::vec3 cameraPos = camera.GetPosition();

		glm::vec3 pointToCam(cameraPos[0] - center[0], cameraPos[1] - center[1], cameraPos[2] - center[2]);
		pointToCam = glm::normalize(pointToCam);
		glm::vec3 point_after_cam1 = camera.GetCameraTransform() * glm::vec4(realP1,1);
		glm::vec3 point_after_cam2 = camera.GetCameraTransform() * glm::vec4(realP2,1);
		glm::vec3 point_after_cam3 = camera.GetCameraTransform() * glm::vec4(realP3,1);
		
		if (scene.GetActiveLightIndex() != -1) {
			light& light1 = scene.GetActiveLight();
			glm::vec3 lightPos = light1.GetPosVec();
			glm::vec3 I = glm::normalize(glm::vec3(lightPos[0] - center[0], lightPos[1] - center[1], lightPos[2] - center[2]));
			//glm::vec3 I = glm::normalize(glm::vec3(center[0] - lightPos[0], center[1] - lightPos[1], center[2] - lightPos[2]));
			light1.Set_I(I);
			if (!scene.GetNormals()) {
				light1.Set_N(normal(realP1, realP2, realP3));
			}
			else {
				light1.Set_N(normal(p1, p2, p3));
			}
			light1.Set_V(pointToCam);
			temp = Flat_shading(light1, mesh, normal(p1,p2,p3), 2, pointToCam);
		}
	}

	for (int y = minY; y <= maxY; y++)
	{
		for (int x = minX; x <= maxX; x++)
		{
			if (PointInTriangle(glm::vec2(x, y), p1, p2, p3)) {
				float z = Linear_Interpolation(realP1,realP2,realP3, glm::vec2(x, y));
				//std::cout << z << std::endl;
				if (1) {

					if (z < camera.zNear && z > camera.zFar && z > z_buffer[Z_INDEX(viewport_width_, x, y)]) {
						z_buffer[Z_INDEX(viewport_width_, x, y)] = z;

						if (mesh.GetModelName() == "Sphere.obj") {
							PutPixel(x, y, glm::vec3(1,1,1));
						}
						else {
							after_fog = Point_color_in_fog(temp,abs(camera.zNear - z)/abs(camera.zFar), 1.2);
							PutPixel(x, y, after_fog);
						}
						//PutPixel(x + 300, y, glm::vec3(c,c,c));
					}
				}
			}
		}
	}





}
//int on_eage(int x, int y,glm::vec2 p1, glm::vec2 p2) {
//	int m = (p1[1] - p2[1]) / (p1[0] - p2[0]);
//	int y_on_line = m * (x - p1[0]) + p1[1];
//	if (y_on_line == y)return 1;
//	else return 0; 
//
//}
//void Renderer::scan_convertion(int x,int y, glm::vec3 color, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3)
//{
//	// Base cases
//	if (0 <= x < viewport_width_ && 0 <= y <= viewport_height_) {
//		if (on_eage(x, y, p1, p2) || on_eage(x, y, p1, p3) || on_eage(x, y, p3, p2)) {
//			flag = !flag;
//			if (flag)
//				PutPixel(x, y, color);
//			// Recur for north, east, south and west
//			scan_convertion(x + 1, y, color, p1, p2, p3);
//		}
//	}
//	if (x == viewport_width_) {
//		if (on_eage(x, y, p1, p2) || on_eage(x, y, p1, p3) || on_eage(x, y, p3, p2)) {
//			flag = !flag;
//			if (flag)
//				PutPixel(x, y, color);
//			// Recur for north, east, south and west
//			scan_convertion(0, y-1, color, p1, p2, p3);
//		}
//	}
//	else {
//			return;
//	}
//
//}



//compute if the point [x,y] is in the triangle [x1,y1] [x2,y2] [x3,y3]
bool Renderer::isInside(float x1, float y1, float x2, float y2, float x3, float y3, float x, float y)
{
	/* Calculate area of triangle ABC */
	float A = area(x1, y1, x2, y2, x3, y3);

	/* Calculate area of triangle PBC */
	float A1 = area(x, y, x2, y2, x3, y3);

	/* Calculate area of triangle PAC */
	float A2 = area(x1, y1, x, y, x3, y3);

	/* Calculate area of triangle PAB */
	float A3 = area(x1, y1, x2, y2, x, y);

	/* Check if sum of A1, A2 and A3 is same as A */
	return (A == A1 + A2 + A3);
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


void To2(glm::vec3& vec) {
	glm::vec3(vec[0]/vec[2], vec[1]/vec[2],vec[3]);
}

void Renderer::Draw_Normals(MeshModel obj, Camera camera,glm::mat4x4 projection,glm::mat4x4 normal_projection)
{
	
	
	double scale;
	std::vector<int[3]> thenorms;
	
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
		scale = obj.GetScaleTransform()[0][0] /10;

		DrawLine(glm::vec2(x, y), glm::vec2(x + (temp[0] * scale), y + (temp[1] * scale)), glm::vec3(1, 0, 1));
		glm::vec3 temp_new = glm::vec3(x + (temp[0] * scale),y + (temp[1] * scale),z + (temp[2] * scale));

	}
	scale = obj.GetScaleTransform()[0][0] / 12;
	for (int i = 0; i < obj.getVerticesSize(); i++) {
		  
		scale = obj.GetScaleTransform()[0][0] * 5.0f;
		if (camera.GetActiveProjection()) {
			scale /= 25;
		}
		glm::vec3 norm = obj.GetNormals()[i];
		glm::vec4 after = glm::normalize(projection * glm::vec4(norm, 1));
			norm[0]= after[0];
			norm[1]= after[1];
			norm[2]= after[2];
		glm::vec3 ver1 = camera.GetViewPortTransformation(HomToCartesian(projection * glm::vec4(obj.getVerticeAtIndex(i), 1)),viewport_width_,viewport_height_);
		DrawLine(glm::vec2(ver1[0], ver1[1]), glm::vec2(ver1[0] + (norm[0]) * scale, ver1[1] + (norm[1])* scale), glm::vec3(1,  0, 1));

	}
	

}



glm::vec3 Renderer::HomToCartesian(glm::vec4 vec)
{

	if (vec[3] == 0) {
		return glm::vec3(vec[0], vec[1], vec[2]);

	}
	return glm::vec3(vec[0] / vec[3], vec[1] / vec[3], vec[2] / vec[3]);
}

