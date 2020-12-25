#pragma once
#include "Scene.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

class Renderer
{
public:
	bool isInside(float x1, float y1, float x2, float y2, float x3, float y3, float x, float y);
	float area(float x1, float y1, float x2, float y2, float x3, float y3);
	void Draw_Normals(MeshModel obj, Camera camera,glm::mat4x4 projection,glm::mat4x4 normal_projection);
	Renderer(int viewportWidth, int viewportHeight);
	virtual ~Renderer();
	void Render(const Scene& scene);
	void SwapBuffers();
	void ClearColorBuffer(const glm::vec3& color);
	int GetViewportWidth() const;
	int GetViewportHeight() const;
	void UseDrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color);
	void DrawModel(MeshModel obj,Scene scene,glm::vec3 color);
	void DrawCamera(Camera camera,Scene scene);
	glm::vec3 HomToCartesian(glm::vec4 vec);
	void DrawBoundingBox(MeshModel obj, glm::mat4x4 projection, Camera camera);
	void PutPixel(const int i, const int j, const glm::vec3& color);
	void FloodFillUtil(int x, int y, glm::vec3 color, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3,Camera& camera);
	void scan_convertion(int x, int y, glm::vec3 color, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3); 
	int flag = 0;
	int on_eage(int x, int y, glm::vec2 p1, glm::vec2 p2);
	bool PointInTriangle(glm::vec2 pt, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);
	float sign(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3);
	double Linear_Interpolation(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec2 pt);
	double Renderer::Linear_Interpolation_by_choice(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec2 pt, int choice_v1, int choice_v2, int choice_v3);
	glm::vec3 Flat_shading(light light_source, MeshModel mesh, glm::vec3 normal_of_polygon, int user_angle);
	glm::vec3 Gouraud_shading_for_vertix(light light_source, MeshModel mesh, glm::vec3 normal_of_polygon, int  user_angle);
	glm::vec3 Renderer::Gouraud_shading_for_point_in_polygon(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 color_p1, glm::vec3 color_p2, glm::vec3 color_p3, glm::vec3 pt);
	double Renderer::Linear_Interpolation_color(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, int color_v1, int color_v2, int color_v3, glm::vec3 pt);
	glm::vec3 Renderer::Phong_shading(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 normal_p1, glm::vec3 normal_p2, glm::vec3 normal_p3, glm::vec3 pt, light light_source, MeshModel mesh, int  user_angle);
private:
	void DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color);

	void CreateBuffers(int w, int h);
	void CreateOpenGLBuffer();
	void InitOpenGLRendering();

	float* color_buffer_;
	float* z_buffer;
	int viewport_width_;
	int viewport_height_;
	GLuint gl_screen_tex_;
	GLuint gl_screen_vtc_;	

};
