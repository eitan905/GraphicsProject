#define _USE_MATH_DEFINES
#include <cmath>
#include <imgui/imgui.h>
 
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nfd.h>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Renderer.h"
#include "Scene.h"
#include "Utils.h"

// "Transformations.h"
glm::vec4 ToHomogeneous4(glm::vec3 vector)
{
	return  (glm::vec4(vector[0], vector[1], vector[2], 1));

}

glm::vec3 FromHomogeneous4(glm::vec4 vector)
{

	glm::vec3 temp(vector[0] / vector[3], vector[1] / vector[3], vector[2] / vector[3]);
	return temp;
}






void  Transform3(float x, float y, float z, glm::vec3& vector)
{
	glm::mat4 transform(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		x, y, z, 1);

	glm::vec4 temp = ToHomogeneous4(vector);

	temp = transform * temp;

	vector = FromHomogeneous4(temp);

}


void Scale3(double x, double y, double z, glm::vec3& vector)
{
	glm::mat3 temp(
		x, 0, 0,
		0, y, 0,
		0, 0, z);
	Transform3(-700, -300, 0, vector);
	vector = temp * vector;
	Transform3(700, 300, 0, vector);

}


void Rotate(double alpha, glm::vec3& vector) {

	alpha = (alpha * 3.14) / 180;
	glm::mat3x3 rotate(
		cos(alpha), sin(alpha), 0,
		-sin(alpha), cos(alpha), 0,
		0, 0, 1
	);

	Transform3(-700, -300, 0, vector);
	vector = rotate * vector;
	Transform3(700, 300, 0, vector);


}


/**
 * Fields
 */
bool show_demo_window = false;
bool show_another_window = false;
glm::vec4 clear_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);

/**
 * Function declarations
 */
static void GlfwErrorCallback(int error, const char* description);
GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name);
ImGuiIO& SetupDearImgui(GLFWwindow* window);
void StartFrame();
void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io);
void Cleanup(GLFWwindow* window);
void DrawImguiMenus(ImGuiIO& io, Scene& scene);

/**
 * Function implementation
 */
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	// TODO: Handle mouse scroll here
}

int main(int argc, char **argv)
{
	int windowWidth = 1280, windowHeight = 720;
	GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, "Mesh Viewer");
	if (!window)
		return 1;

	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);


	Renderer renderer = Renderer(frameBufferWidth, frameBufferHeight);
	Scene scene = Scene();

	scene.AddModel(Utils::LoadMeshModel("C:\\Users\\user\\Desktop\\HADAR LIMUDIM\\TextFile1.txt"));
	

	
	ImGuiIO& io = SetupDearImgui(window);
	glfwSetScrollCallback(window, ScrollCallback);
    while (!glfwWindowShouldClose(window))
    {

        glfwPollEvents();
		StartFrame();
		DrawImguiMenus(io, scene);
		RenderFrame(window, scene, renderer, io);
    }

	Cleanup(window);
    return 0;
}

static void GlfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name)
{
	glfwSetErrorCallback(GlfwErrorCallback);
	if (!glfwInit())
		return NULL;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	#if __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif
	
	GLFWwindow* window = glfwCreateWindow(w, h, window_name, NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync
						 // very importent!! initialization of glad
						 // https://stackoverflow.com/questions/48582444/imgui-with-the-glad-opengl-loader-throws-segmentation-fault-core-dumped

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	return window;
}

ImGuiIO& SetupDearImgui(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();
	return io;
}

void StartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io)
{
	MeshModel& obj = scene.GetModel(0);
	ImGui::Render();
	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
	
	if (frameBufferWidth != renderer.GetViewportWidth() || frameBufferHeight != renderer.GetViewportHeight())
	{
		// TODO: Set new aspect ratio
	}

	if (!io.WantCaptureKeyboard)
	{
		// TODO: Handle keyboard events here
		if (io.KeysDown[68])
		{
			glm::mat4x4 temp(
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				3, 0, 0, 1
			);
		
				std::cout << "local flag" << std::endl;
				obj.SetLocalTransform(temp);
				
		}
		if (io.KeysDown[65])
		{
			glm::mat4x4 temp(
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				-3, 0, 0, 1
			);

			std::cout << "local flag" << std::endl;
			obj.SetLocalTransform(temp);

		}
		if (io.KeysDown[83])
		{
			glm::mat4x4 temp(
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, -3, 0, 1
			);

			std::cout << "local flag" << std::endl;
			obj.SetLocalTransform(temp);

		}
		if (io.KeysDown[87])
		{
			glm::mat4x4 temp(
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 3, 0, 1
			);

			std::cout << "local flag" << std::endl;
			obj.SetLocalTransform(temp);

		}

	}

	if (!io.WantCaptureMouse)
	{
		// TODO: Handle mouse events here
		if (io.MouseDown[0])
		{
			// Left mouse button is down
		}
	}

	renderer.ClearColorBuffer(clear_color);
	renderer.Render(scene);
	renderer.SwapBuffers();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwMakeContextCurrent(window);
	glfwSwapBuffers(window);
}

void Cleanup(GLFWwindow* window)
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

void DrawImguiMenus(ImGuiIO& io, Scene& scene)
{
	/**
	 * MeshViewer menu
	 */
	ImGui::Begin("MeshViewer Menu");
	
	// Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open", "CTRL+O"))
			{
				nfdchar_t* outPath = NULL;
				nfdresult_t result = NFD_OpenDialog("obj;", NULL, &outPath);
				if (result == NFD_OKAY)
				{
					scene.AddModel(Utils::LoadMeshModel(outPath));
					free(outPath);
				}
				else if (result == NFD_CANCEL)
				{
				}
				else
				{
				}

			}
			ImGui::EndMenu();
		}

		// TODO: Add more menubar items (if you want to)
		ImGui::EndMainMenuBar();
	}

	// Controls
	ImGui::ColorEdit3("Clear Color", (float*)&clear_color);
	// TODO: Add more controls as needed
	
	ImGui::End();

	/**
	 * Imgui demo - you can remove it once you are familiar with imgui
	 */
	
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f,x=0.0f, y = 0.0f, z = 0.0f,alpha=0.0f;
		MeshModel& obj = scene.GetModel(scene.GetActiveModelIndex());
		
		static int i1 = 0,i2=0,i3=0;
		static int counter = 0;
		//ImGuiInputTextFlags INPUT_TEXT_CHARS_DECIMAL =0;
		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
	
		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::InputFloat("x", &x, 0.01f, 1.0f);
		ImGui::InputFloat("y", &y, 0.01f, 1.0f);
		ImGui::InputFloat("z", &z, 0.01f, 1.0f);
		ImGui::InputFloat("alfa", &alpha, 0.01f, 1.0f);
		
		static const char* currentModels[]{ "bunny", "cow" };
		static int selecteItem = scene.GetActiveModelIndex();
		
		ImGui::ListBox("active model", &selecteItem, currentModels, 2, 2);


		static ImGuiColorEditFlags alpha_flags = 1;
		static unsigned int beta_flags = 1;
		static bool checkedLocal = true;
		static bool checkedWorld = false;
		bool translationFlag=ImGui::RadioButton("Translation", &alpha_flags,1) ;
		bool scalingFlag = ImGui::RadioButton("scaling ", &alpha_flags, 2);
		bool rotateFlag = ImGui::RadioButton("rotate ", &alpha_flags, 3);

		ImGui::SameLine();
		//ImGui::RadioButton("scaling ", &alpha_flags);
		bool localFlag = ImGui::Checkbox ("local", &checkedLocal);
		bool worldFlag = ImGui::Checkbox ("world", &checkedWorld);

		
			if (x != 0 || y != 0 || z != 0 || alpha != 0 ) {
				if (translationFlag) {
					std::cout << "translation flag" << std::endl;
					glm::mat4x4 temp(
					1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					x, y, z, 1
					);
					if (checkedLocal) {
						std::cout << "local flag" << std::endl;
						obj.SetLocalTransform(temp);
						x = y = z = 0.0f;
					}
					if (checkedWorld) {
						std::cout << "world flag" << std::endl;
						obj.SetWorldTransform(temp);
						x = y = z = 0.0f;
					}
				}


				if (scalingFlag) {
					std::cout << "scaling flag" << std::endl;
					glm::mat4x4 temp(
						x, 0, 0, 0,
						0, y, 0, 0,
						0, 0, z, 0,
						0, 0, 0, 1
					);
					if (checkedLocal) {
						std::cout << "localflag" << std::endl;
						obj.SetLocalTransform(temp);
						x = y = z = 0.0f;
					}
					if (checkedWorld) {
						std::cout << "world flag" << std::endl;
						obj.SetWorldTransform(temp);
						x = y = z = 0.0f;
					}
				}

				if (rotateFlag) {

					alpha = (alpha * 3.14) / 180;
					glm::mat4x4 rotate(
						cos(alpha), sin(alpha), 0, 0,
						-sin(alpha), cos(alpha), 0, 0,
						0, 0, 1 , 0,
						0, 0, 0, 1
					);
					if (checkedLocal) {
						std::cout << "local flag" << std::endl;
						obj.SetLocalRotationTransform(rotate);
						alpha = 0.0f;
					}
					if (checkedWorld) {
						std::cout << "world flag" << std::endl;
						obj.SetWorldTransform(rotate);
						alpha = 0.0f;
					}



				}
				
				
			}

		
	
		
		//ImGui::SameLine();
		//ImGui::RadioButton("Both", &alpha_flags, ImGuiColorEditFlags_AlphaPreviewHalf);


		ImGui::SliderFloat("translation X", &f, -700.0f, 500.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		/*for (int i = 0; i < obj.getVerticesSize(); i++) {
			obj.getVerticeAtIndex(i)[0] = f;
		}*/
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
		

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);
		
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}
}

