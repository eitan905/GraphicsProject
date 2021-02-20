#pragma once
#include "Scene.h"
#include "ShaderProgram.h"
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <memory>
#include "Texture2D.h"

class Renderer
{
public:

	
	glm::vec3 color_neighbers;
	float area(float x1, float y1, float x2, float y2, float x3, float y3);
	Renderer();



	



	virtual ~Renderer();
	void Render(const Scene& scene);
	void SwapBuffers();
	//void ClearColorBuffer(const glm::vec3& color);
	int GetViewportWidth() const;
	int GetViewportHeight() const;
	void PutPixel(int i, int j, const glm::vec3& color);
	void ClearColorBuffer(const glm::vec3& color);
	void LoadShaders();
	void LoadTextures();

	void CreateOpenGLBuffer();
	void InitOpenGLRendering();
	void CreateBuffers(int w, int h);
	


private:

	

	float* color_buffer_;
	float* z_buffer;
	float* mass_buffer;
	float* only_bright;
	float* bloom;
	int viewport_width_;
	int viewport_height_;
	GLuint gl_screen_tex_;
	GLuint gl_screen_vtc_;	
	ShaderProgram lightShader;
	ShaderProgram colorShader;
	ShaderProgram shader;
	ShaderProgram skyboxShader;
	Texture2D texture1;
	Texture2D texture2;
	Texture2D texture3;
	GLuint cubeMapTexture;
	GLuint cubeVAO, cubeVBO;
	GLuint skyboxVAO, skyboxVBO;

};
