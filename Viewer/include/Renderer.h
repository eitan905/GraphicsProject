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
	void DrawModel(MeshModel obj,Scene scene);
	void DrawCamera(Camera camera,Scene scene);
	glm::vec3 HomToCartesian(glm::vec4 vec);
	void DrawBoundingBox(MeshModel obj, glm::mat4x4 projection, Camera camera);
	void PutPixel(const int i, const int j, const glm::vec3& color);
	void FloodFillUtil(int x, int y, glm::vec3 color, glm::vec2 p1, glm::vec2 p2, glm::vec2 p3);
	
private:
	void DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color);

	void CreateBuffers(int w, int h);
	void CreateOpenGLBuffer();
	void InitOpenGLRendering();

	float* color_buffer_;
	int viewport_width_;
	int viewport_height_;
	GLuint gl_screen_tex_;
	GLuint gl_screen_vtc_;	

};
