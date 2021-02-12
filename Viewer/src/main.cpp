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

#include <memory>
#include <random>
#include <stdlib.h>



static float mouse_x;
static float mouse_y;
static float previous_mouse_x = 0;
static float previous_mouse_y = 0;
static double previous_time = 0;
std::vector<MeshModel*> mouse_models;

glm::vec4 clearColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);

Scene scene = Scene();



bool show_demo_window = false;
bool show_another_window = false;
glm::vec4 clear_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.00f);
ImGuiIO* imgui;
ImGuiIO& SetupDearImgui(GLFWwindow* window);



GLFWwindow* window;

static void GlfwErrorCallback(int error, const char* description);
GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name);
ImGuiIO& SetupDearImgui(GLFWwindow* window);
bool Setup(int windowWidth, int windowHeight, const char* windowName);
void Cleanup(GLFWwindow* window);



void StartFrame();
void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io);
void DrawImguiMenus(ImGuiIO& io, Scene& scene);
void glfw_OnFramebufferSize(GLFWwindow* window, int width, int height);


/**
 * Function implementation
 */
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	// TODO: Handle mouse scroll here
}

int main(int argc, char** argv)
{
	int windowWidth = 1280, windowHeight = 720;

	if (!Setup(windowWidth, windowHeight, "Mesh Viewer"))
	{
		std::cerr << "Setup failed" << std::endl;
		return -1;
	}
	glfwMakeContextCurrent(window);
	int frameBufferWidth, frameBufferHeight;

	



	if (!window)
		return 1;
	Renderer renderer;
	//renderer.InitOpenGLRendering();
	renderer.LoadShaders();
	renderer.LoadTextures();

	scene.SetWidth(windowWidth);
	scene.SetHeight(windowHeight);
	scene.AddModel(Utils::LoadMeshModel("C:/Users/Eitan/Desktop/bunny.txt"));
	//scene.AddModel(Utils::LoadMeshModel("C:/Users/Eitan/Documents//GitHub/computergraphics2021-eitan-and-hadar/computergraphics2021-eitan-and-hadar/Data/handGun_C"));
	//scene.AddModel(Utils::LoadMeshModel("C:/Users/Eitan/Desktop/armadillo.txt"));
	scene.AddModel(Utils::LoadMeshModel("C:/Users/Eitan/Desktop/camera.txt"));

	



	ImGuiIO& io = SetupDearImgui(window);
	glfwSetScrollCallback(window, ScrollCallback);


	while (!glfwWindowShouldClose(window))
	{

		glfwPollEvents();
		StartFrame();
		glfwMakeContextCurrent(window);
		DrawImguiMenus(io, scene);
		ImGui::Render();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		RenderFrame(window, scene, renderer, io);


		

	}





	glfwTerminate();
	return 0;
}
//check




//void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
//{
//	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
//	// TODO: Handle mouse scroll here
//}
//
//int main(int argc, char** argv)
//{
//	int windowWidth = 1280, windowHeight = 720;
//	GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, "Mesh Viewer");
//	if (!window)
//		return 1;
//
//	int frameBufferWidth, frameBufferHeight;
//	glfwMakeContextCurrent(window);
//	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
//
//
//	Renderer renderer = Renderer(frameBufferWidth, frameBufferHeight);
//	Scene scene = Scene();
//	scene.SetWidth(windowWidth);
//	scene.SetHeight(windowHeight);
//	scene.AddModel(Utils::LoadMeshModel("C:/Users/Eitan/Desktop/bunny.txt"));
//	scene.AddModel(Utils::LoadMeshModel("C:/Users/Eitan/Desktop/camera.txt"));
//	//scene.AddModel(Utils::LoadMeshModel("C:/Users/Eitan/Desktop/camera.txt"));
//
//
//	ImGuiIO& io = SetupDearImgui(window);
//	glfwSetScrollCallback(window, ScrollCallback);
//	while (!glfwWindowShouldClose(window))
//	{
//
//		glfwPollEvents();
//		StartFrame();
//		DrawImguiMenus(io, scene);
//		RenderFrame(window, scene, renderer, io);
//	}
//
//	Cleanup(window);
//	return 0;
//}
static void GlfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}





GLFWwindow* SetupGlfwWindow(int windowWidth, int windowHeight, const char* windowName)
{
	// Intialize GLFW
	if (!glfwInit())
	{
		// An error occured
		std::cerr << "GLFW initialization failed" << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// forward compatible with newer versions of OpenGL as they become available but not backward compatible (it will not run on devices that do not support OpenGL 3.3
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


	// Create an OpenGL 3.3 core, forward compatible context window
	window = glfwCreateWindow(windowWidth, windowHeight, windowName, NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	// Make the window's context the current one
	glfwMakeContextCurrent(window);

	// Setup window events callbacks
	glfwSetFramebufferSizeCallback(window, glfw_OnFramebufferSize);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		// An error occured
		std::cerr << "GLAD initialization failed" << std::endl;
		return false;
	}

	return window;
}


//
//GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name)
//{
//	glfwSetErrorCallback(GlfwErrorCallback);
//	if (!glfwInit())
//		return NULL;
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//	
//	#if __APPLE__
//		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//	#endif
//	
//	GLFWwindow* window = glfwCreateWindow(w, h, window_name, NULL, NULL);
//	glfwMakeContextCurrent(window);
//	glfwSwapInterval(1); // Enable vsync
//						 // very importent!! initialization of glad
//						 // https://stackoverflow.com/questions/48582444/imgui-with-the-glad-opengl-loader-throws-segmentation-fault-core-dumped
//
//	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
//	return window;
//}


bool Setup(int windowWidth, int windowHeight, const char* windowName)
{
	
	GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, windowName);
	if (!window)
	{
		std::cerr << "Window setup failed" << std::endl;
		return false;
	}
	imgui = &SetupDearImgui(window);

	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	glEnable(GL_DEPTH_TEST);

	return true;
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
int r = 0;
//RENDER FRAME func
void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io)
{

	//ImGui::Render();
	int frameBufferWidth, frameBufferHeight;
	//glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

	if (frameBufferWidth != renderer.GetViewportWidth() || frameBufferHeight != renderer.GetViewportHeight())
	{
		// TODO: Set new aspect ratio
	}
	// Handle keyboard events {a-left,d-right,w-up,s-down}
	if (scene.GetModelCount() != 0) {
		MeshModel& obj = scene.GetActiveModel();
		if (!io.WantCaptureKeyboard)
		{
			if (io.KeysDown[68])//d: move right
			{
				obj.LocalTranslateTransform(0.2, 0, 0);
			}
			if (io.KeysDown[65])//a: move left
			{
				obj.LocalTranslateTransform(-0.2, 0, 0);
			}
			if (io.KeysDown[83])//s: move down
			{
				obj.LocalTranslateTransform(0, -0.2, 0);
			}
			if (io.KeysDown[87])//w: move up
			{
				obj.LocalTranslateTransform(0, 0.2, 0);
			}
			if (io.KeysDown[80])//p: set local scale transform (2)
			{
				obj.LocalScaleTransform(1.01, 1.01, 1.01);
			}
			if (io.KeysDown[79])//o: set local scale transform (0.5)
			{
				obj.LocalScaleTransform(0.99, 0.99, 0.99);
			}
		}
	}

		if (scene.GetCameraCount() != 0) {
			if (io.KeysDown[67])//u: set local rotate transform (20 degree)
			{
				scene.GetActiveCamera().TranslateSpace(1, 0, 0);
			}
			if (io.KeysDown[90])//z: set local rotate transform (20 degree)
			{
				scene.GetActiveCamera().TranslateSpace(-1, 0, 0);
			}
			if (io.KeysDown[71])//g: set local rotate transform (20 degree)
			{
				scene.GetActiveCamera().SetDistance(1);
			}
			if (io.KeysDown[70])//f: set local rotate transform (20 degree)
			{
				scene.GetActiveCamera().SetDistance(-1);
			}
			if (io.KeysDown[72])//h: set local rotate transform (20 degree)
			{
				scene.GetActiveCamera().localTranslateTransform[3][2] += 1;
			}
			if (io.KeysDown[74])//j: set local rotate transform (20 degree)
			{
				scene.GetActiveCamera().localTranslateTransform[3][2] -= 1;
			}
		}
		if (scene.GetActiveLightIndex() != -1) {
			if (io.KeysDown[77])//m: set local rotate transform (20 degree)
			{
				scene.GetActiveLight().SetTransformX(1);
			}
			if (io.KeysDown[78])//n: set local rotate transform (20 degree)
			{
				scene.GetActiveLight().SetTransformX(-1);
			}
			if (io.KeysDown[73])//i: set local rotate transform (20 degree)
			{
				scene.GetActiveLight().SetTransformY(1);
			}
			if (io.KeysDown[75])//k: set local rotate transform (20 degree)
			{
				scene.GetActiveLight().SetTransformY(-1);
			}
			if (io.KeysDown[88])//b: set local rotate transform (20 degree)
			{
				scene.GetActiveLight().SetTransformZ(1);
			}
			if (io.KeysDown[86])//v: set local rotate transform (20 degree)
			{
				scene.GetActiveLight().SetTransformZ(-1);
			}
		}
	



	renderer.Render(scene);

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(window);
	glfwMakeContextCurrent(window);

}










void glfw_OnFramebufferSize(GLFWwindow* window, int width, int height)
{

	glViewport(0, 0, width, height);
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
	if (scene.GetModelCount() != 0) {
		ImGui::ColorEdit3("Clear Color", (float*)&clear_color);
		ImGui::ColorEdit3("model K_A", (float*)&scene.GetActiveModel().K_A);
		ImGui::ColorEdit3("model K_S", (float*)&scene.GetActiveModel().K_S);
		ImGui::ColorEdit3("model K_D", (float*)&scene.GetActiveModel().K_D);
	}




	// TODO: Add more controls as needed

	ImGui::End();

	/**
	 * Imgui demo - you can remove it once you are familiar with imgui
	 */

	 // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.

	if (scene.GetModelCount() != 0) {

		MeshModel& obj = scene.GetActiveModel();
		static float rotate = 0.0f, x = 0.0f, y = 0.0f, z = 0.0f, alpha = 0.0f;

		static int i1 = 0, i2 = 0, i3 = 0;
		static int counter = 0;
		//ImGuiInputTextFlags INPUT_TEXT_CHARS_DECIMAL =0;
		ImGui::Begin("GUI");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Checkbox("Cameras", &show_another_window);
		ImGui::Checkbox("Bounding Box", &scene.displayBox);
		ImGui::SameLine();
		ImGui::Checkbox("Normals", &scene.displayNormals);


		//simple GUI
		//create the obj list
		static const char* currentModels[50];
		for (int i = 0; i < scene.GetModelCount(); i++) {
			std::string str = scene.GetModel(i).GetModelName();
			currentModels[i] = strcpy(new char[str.length() + 1], str.c_str());
		}
		static int selecteItem;
		selecteItem = scene.GetActiveModelIndex();
		ImGui::ListBox("active model", &selecteItem, currentModels, scene.GetModelCount(), 2);
		scene.SetActiveModelIndex(selecteItem);

		//translation


		//scaling






		//Scale Slider


		static float scaleValue1;
		if (scene.GetCameraCount() != 0) {
			Camera& camera = scene.GetActiveCamera();
			scaleValue1 = camera.GetScaleBarValue();
		}
		if (ImGui::Button("Reset object")) {
			obj.Reset();
		}
		if (ImGui::TreeNode("Object Transforms"))
		{
			ImGui::PushItemWidth(100);
			ImGui::Text("< or.x");
			ImGui::SameLine();
			ImGui::SliderFloat("or.y", &obj.localRotateBarValue_X, 0, 360.0f);
			ImGui::SameLine();
			ImGui::SliderFloat("or.z", &obj.localRotateBarValue_Y, 0, 360.0f);
			ImGui::SameLine();
			ImGui::SliderFloat("> object rotate", &obj.localRotateBarValue_Z, 0, 360.0f);
			ImGui::Text("< ot.x");
			ImGui::SameLine();
			ImGui::SliderFloat("ot.y", &obj.localTranslateBarValue_X, -100, 100);
			ImGui::SameLine();
			ImGui::SliderFloat("ot.z", &obj.localTranslateBarValue_Y, -100, 100);
			ImGui::SameLine();
			ImGui::SliderFloat("> object translate", &obj.localTranslateBarValue_Z, -600, 200);

			ImGui::SetNextItemWidth(400);
			ImGui::SliderFloat("Object_Scale", &obj.localScaleBarValue, 0, 1000);



			ImGui::TreePop();
		}

		//
		if (ImGui::TreeNode("World Transforms"))
		{
			ImGui::PushItemWidth(100);
			ImGui::Text("< wr.x");
			ImGui::SameLine();
			ImGui::SliderFloat("wr.y", &obj.worldRotateBarValue_X, 0, 360.0f);
			ImGui::SameLine();
			ImGui::SliderFloat("wr.z", &obj.worldRotateBarValue_Y, 0, 360.0f);
			ImGui::SameLine();
			ImGui::SliderFloat("> world rotate", &obj.worldRotateBarValue_Z, 0, 360.0f);
			ImGui::Text("< wt.x");
			ImGui::SameLine();
			ImGui::SliderFloat("wt.y", &obj.worldTranslateBarValue_X, -100, 100);
			ImGui::SameLine();
			ImGui::SliderFloat("wt.z", &obj.worldTranslateBarValue_Y, -100, 100);
			ImGui::SameLine();
			ImGui::SliderFloat("> world translate", &obj.worldTranslateBarValue_Z, -100, 100);

			ImGui::SetNextItemWidth(400);
			ImGui::SliderFloat("World_Scale", &obj.worldScaleBarValue, 0, 1000);

			ImGui::PushItemWidth(100);

			ImGui::TreePop();
		}




		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
		ImGui::ColorEdit3("model K_A", (float*)&scene.GetActiveModel().K_A);
		ImGui::ColorEdit3("model K_D", (float*)&scene.GetActiveModel().K_D);
		ImGui::ColorEdit3("model K_S", (float*)&scene.GetActiveModel().K_S);

		if (ImGui::Button("Add Light")) {
			scene.AddModel(Utils::LoadMeshModel("C:/Users/Eitan/Documents/GitHub/computergraphics2021-eitan-and-hadar/computergraphics2021-eitan-and-hadar/Data/Sphere.obj"));
		}
		static const char* currentLights[50];
		if (scene.GetLightsCount() != 0) {
			for (int i = 0; i < scene.GetLightsCount(); i++) {
				std::string str = "light " + std::to_string(i);
				currentLights[i] = strcpy(new char[str.length() + 1], str.c_str());
			}
			static int selectedLight;
			selectedLight = scene.GetActiveLightIndex();

			ImGui::ListBox("active light", &selectedLight, currentLights, scene.GetLightsCount(), 2);
			scene.SetActiveLightIndex(selectedLight);

			if (ImGui::Button("Gouraud")) {
				scene.SetShading("Gouraud");
			}
			ImGui::SameLine();
			if (ImGui::Button("Phong")) {
				scene.SetShading("Phong");
			}
			ImGui::SameLine();
			if (ImGui::Button("Flat")) {
				scene.SetShading("Flat");
			}
			if (ImGui::Button("Parallel")) {
				scene.GetActiveLight().SetActiveLight("Parallel");
			}
			ImGui::SameLine();
			if (ImGui::Button("Point")) {
				scene.GetActiveLight().SetActiveLight("Point");
			}
			ImGui::SameLine();
			if (ImGui::Button("realP")) {
				scene.SetNormals(0);
			}
			ImGui::SameLine();
			if (ImGui::Button("p")) {
				scene.SetNormals(1);
			}

			if (ImGui::Button("normal")) {
				scene.SetPreProcessing("normal");
			}
			ImGui::SameLine();
			if (ImGui::Button("fog")) {
				scene.SetPreProcessing("fog");
			}
			ImGui::SameLine();
			if (ImGui::Button("MASS")) {
				scene.SetPreProcessing("MASS");
			}
			ImGui::SameLine();
			if (ImGui::Button("bloom")) {
				scene.SetBloom(1 - scene.GetBloom());
			}

			ImGui::InputFloat3("paralel", (float*)&scene.GetActiveLight().paralel, 2);
			ImGui::InputFloat("user_angle", (float*)&scene.GetActiveLight().user_angle);
			ImGui::ColorEdit3("L_A", (float*)&scene.GetActiveLight().L_A);
			ImGui::ColorEdit3("L_D", (float*)&scene.GetActiveLight().L_D);
			ImGui::ColorEdit3("L_S", (float*)&scene.GetActiveLight().L_S);


		}

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();



		// 3. Show another simple window.
		if (show_another_window && scene.GetCameraCount() != 0)
		{

			static float scaleValue1;
			Camera& camera = scene.GetActiveCamera();


			ImGui::Begin("Cameras", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)

			camera.SetActiveProjection(ImGui::Button("change Projection"));
			//ImGui::ShowDemoWindow();
			//ImGui::Combo();

			ImGui::SetNextItemWidth(80);
			static const char* currentCameras[50];
			for (int i = 0; i < scene.GetCameraCount(); i++) {
				std::string str = scene.GetCamera(i).GetModelName();
				currentCameras[i] = strcpy(new char[str.length() + 1], str.c_str());
			}
			static int selectedCamera;
			selectedCamera = scene.GetActiveCameraIndex();
			ImGui::ListBox("active model", &selectedCamera, currentCameras, scene.GetCameraCount(), 2);
			scene.SetActiveCameraIndex(selectedCamera);

			if (ImGui::TreeNode("LookAt")) {
				ImGui::PushItemWidth(100);
				ImGui::Text("< e.x");
				ImGui::SameLine();
				ImGui::SliderFloat("e.y", &camera.eye[0], -50.0f, 50.0f);

				ImGui::SameLine();
				ImGui::SliderFloat("e.z", &camera.eye[1], -50.0f, 50.0f);
				ImGui::SameLine();
				ImGui::SliderFloat("> eye", &camera.eye[2], -50.0f, 50.0f);

				ImGui::Text("< a.x");
				ImGui::SameLine();
				ImGui::SliderFloat("a.y", &camera.at[0], -50.0f, 50.0f);
				ImGui::SameLine();
				ImGui::SliderFloat("a.z", &camera.at[1], -50.0f, 50.0f);
				ImGui::SameLine();
				ImGui::SliderFloat("> at", &camera.at[2], -50.0f, 50.0f);

				ImGui::Text("< u.x");
				ImGui::SameLine();
				ImGui::SliderFloat("u.y", &camera.up[0], -50.0f, 50.0f);
				ImGui::SameLine();
				ImGui::SliderFloat("u.z", &camera.up[1], -50.0f, 50.0f);
				ImGui::SameLine();
				ImGui::SliderFloat("> up", &camera.up[2], -50.0f, 50.0f);
				ImGui::TreePop();
			}
			ImGui::PushItemWidth(100);
			if (ImGui::TreeNode("Perspeective"))
			{
				ImGui::PushItemWidth(100);


				ImGui::SliderFloat("fovy", &camera.fovy, -90.0f, 0);
				ImGui::SameLine();
				ImGui::SliderFloat("aspect", &camera.aspect, 0.5f, 2.0f);
				ImGui::SliderFloat("near", &camera.zNear, 0, 300.0f);
				ImGui::SameLine();
				ImGui::SliderFloat("far", &camera.zFar, camera.zNear, 800.0f);

				/*
				if (ImGui::CollapsingHeader("Header with a bullet", ImGuiTreeNodeFlags_Bullet))
					ImGui::Text("IsItemHovered: %d", ImGui::IsItemHovered());
				*/
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Object Transforms"))
			{
				ImGui::Text("< cr.x");
				ImGui::SameLine();
				ImGui::SliderFloat("cr.y", &camera.localRotateBarValue_X, 0, 360.0f);
				ImGui::SameLine();
				ImGui::SliderFloat("cr.z", &camera.localRotateBarValue_Y, 0, 360.0f);
				ImGui::SameLine();
				ImGui::SliderFloat("> camera rotate", &camera.localRotateBarValue_Z, 0, 360.0f);
				ImGui::Text("< ct.x");
				ImGui::SameLine();
				ImGui::SliderFloat("ct.y", &camera.localTranslateBarValue_X, -100, 100);
				ImGui::SameLine();
				ImGui::SliderFloat("ct.z", &camera.localTranslateBarValue_Y, -100, 100);
				ImGui::SameLine();
				ImGui::SliderFloat("> camera translate", &camera.localTranslateBarValue_Z, -100, 100);
				ImGui::SliderFloat("Camera_Scale", &camera.localScaleBarValue, 1, 10);



				ImGui::TreePop();
			}

			//
			if (ImGui::TreeNode("World Transforms"))
			{

				ImGui::Text("< wr.x");
				ImGui::SameLine();
				ImGui::SliderFloat("wr.y", &camera.worldRotateBarValue_X, 0, 360.0f);
				ImGui::SameLine();
				ImGui::SliderFloat("wr.z", &camera.worldRotateBarValue_Y, 0, 360.0f);
				ImGui::SameLine();
				ImGui::SliderFloat("> world rotate", &camera.worldRotateBarValue_Z, 0, 360.0f);
				ImGui::Text("< wt.x");
				ImGui::SameLine();
				ImGui::SliderFloat("wt.y", &camera.worldTranslateBarValue_X, -100, 100);
				ImGui::SameLine();
				ImGui::SliderFloat("wt.z", &camera.worldTranslateBarValue_Y, -100, 100);
				ImGui::SameLine();
				ImGui::SliderFloat("> world translate", &camera.worldTranslateBarValue_Z, -100, 100);
				ImGui::SliderFloat("Camera_Scale", &camera.worldScaleBarValue, 0, 100);



				ImGui::TreePop();
			}
			//scene.GetActiveCamera().SetActiveProjection(ImGui::Button("change Projection"));
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}
	}
}







void Cleanup(GLFWwindow* window)
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}





