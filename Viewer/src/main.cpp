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



static float mouse_x;
static float mouse_y;
static float previous_mouse_x = 0;
static float previous_mouse_y = 0;
static double previous_time = 0;
std::vector<MeshModel*> mouse_models;




//sigh func- help for the PointInTriangle 
float sign(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3)
{
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}


// compute if the point pt is in the triangle v1 v2 v3
bool PointInTriangle(glm::vec2 pt, glm::vec2 v1, glm::vec2 v2, glm::vec2 v3)
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


//copute the area of Triangle
float area(float x1, float y1, float x2, float y2, float x3, float y3)
{
	return abs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0);
}

//compute if the point [x,y] is in the triangle [x1,y1] [x2,y2] [x3,y3]
bool isInside(float x1, float y1, float x2, float y2, float x3, float y3, float x, float y)
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
	scene.AddModel(Utils::LoadMeshModel("C:/Users/Eitan/Desktop/bunny.txt"));
	scene.AddModel(Utils::LoadMeshModel("C:/Users/Eitan/Desktop/camera.txt"));

	
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
//check
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
int r = 0;
//RENDER FRAME func
void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io)
{
	MeshModel& obj = scene.GetActiveModel();

	ImGui::Render();
	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
	
	if (frameBufferWidth != renderer.GetViewportWidth() || frameBufferHeight != renderer.GetViewportHeight())
	{
		// TODO: Set new aspect ratio
	}
	// Handle keyboard events {a-left,d-right,w-up,s-down}
	if (!io.WantCaptureKeyboard)
	{
		if (io.KeysDown[68])//d: move right
		{
			obj.LocalTranslateTransform(4,0,0);		
		}
		if (io.KeysDown[65])//a: move left
		{
			obj.LocalTranslateTransform(-4, 0, 0);
		}
		if (io.KeysDown[83])//s: move down
		{
			obj.LocalTranslateTransform(0, -4, 0);
		}
		if (io.KeysDown[87])//w: move up
		{
			obj.LocalTranslateTransform(0, 4, 0);
		}
		if (io.KeysDown[80])//p: set local scale transform (2)
		{
			obj.LocalScaleTransform(1.05, 1.05, 1.05);
		}
		if (io.KeysDown[79])//o: set local scale transform (0.5)
		{
			obj.LocalScaleTransform(0.95, 0.95, 0.95);
		}
		if (io.KeysDown[73])//I: set local rotate transform (20 degree)
		{
			obj.position += 5;
			obj.LocalRotationTransform_Z(obj.position);
			
		}
		if (io.KeysDown[85])//u: set local rotate transform (20 degree)
		{
			obj.position -= 5;
			obj.LocalRotationTransform_Z(obj.position);

		}
		if (io.KeysDown[67])//u: set local rotate transform (20 degree)
		{
			scene.GetActiveCamera().TranslateSpace(5, 0, 0);
		}
		if (io.KeysDown[90])//u: set local rotate transform (20 degree)
		{
			scene.GetActiveCamera().TranslateSpace(-5, 0, 0);
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

	//Capture mouse event - drag
	if (!io.WantCaptureMouse)
	{

		double time = ImGui::GetTime();

		//mouse click
		if (io.MouseDown[0])
		{
			bool isMouseOnModel = false;
			mouse_x = io.MousePos[0];
			mouse_y = io.MousePos[1];
			if ((time - previous_time) > 0.3) {
				previous_mouse_x = io.MousePos[0];
				previous_mouse_y = io.MousePos[1];
				mouse_models.clear();				
			}
				for (int i = 0; i < scene.GetModelCount(); i++) {
					MeshModel temp_obj = scene.GetModel(i);
					for (int j = 0; j < temp_obj.getVerticesSize(); j++) {
						//Update obj vertices according to the transformation
						glm::vec4 temp = temp_obj.GetTransform() * glm::vec4(temp_obj.getVerticeAtIndex(j), 1);
						temp_obj.getVerticeAtIndex(j)[0] = temp[0];
						temp_obj.getVerticeAtIndex(j)[1] = temp[1];
						temp_obj.getVerticeAtIndex(j)[2] = temp[2];
					}
					for (int k = 0; k < temp_obj.GetFacesCount(); k++) {
						Face face = temp_obj.GetFace(k);
						int point0 = face.GetVertexIndex(0) - 1;
						int point1 = face.GetVertexIndex(1) - 1;
						int point2 = face.GetVertexIndex(2) - 1;
						//Goes through all the possible triangles
						glm::vec2 p1(temp_obj.getVerticeAtIndex(point0)[0], temp_obj.getVerticeAtIndex(point0)[1]);
						glm::vec2 p2(temp_obj.getVerticeAtIndex(point1)[0], temp_obj.getVerticeAtIndex(point1)[1]);
						glm::vec2 p3(temp_obj.getVerticeAtIndex(point2)[0], temp_obj.getVerticeAtIndex(point2)[1]);
						//check if the current click is in one of the triangles's model
						if (isInside(p1[0],p1[1], p2[0], p2[1], p3[0], p3[1], mouse_x,720- mouse_y)) {
							isMouseOnModel = true;
						}
					}
					//Takes all the models in which the click is within
					if (isMouseOnModel) {
						std::cout << "found model" ;
						mouse_models.push_back(&scene.GetModel(i));
						scene.SetActiveModelIndex(i);
						isMouseOnModel = false;
					}
				}
			//Goes through all the active models and repositions them to the mouse position accordingly
			for (int t = 0; t < mouse_models.size(); t++) {
				if (previous_mouse_x != 0) {
					mouse_models[t]->LocalTranslateTransform(mouse_x - previous_mouse_x, previous_mouse_y - mouse_y , 0);
				}
				previous_mouse_x = mouse_x;
				previous_mouse_y = mouse_y;
			}
			previous_time = time;
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
	
	if (scene.GetModelCount() != 0) {
		MeshModel& obj = scene.GetActiveModel();


		static float rotate = 0.0f, x = 0.0f, y = 0.0f, z = 0.0f, alpha = 0.0f;

		static int i1 = 0, i2 = 0, i3 = 0;
		static int counter = 0;
		//ImGuiInputTextFlags INPUT_TEXT_CHARS_DECIMAL =0;
		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Cameras", &show_another_window);

		//simple GUI
		//create the obj list
		static const char* currentModels[50];
		for (int i = 0; i < scene.GetModelCount(); i++) {
			std::string str = scene.GetModel(i).GetModelName();
			currentModels[i] = strcpy(new char[str.length() + 1], str.c_str());
		}
		static int selecteItem;
		static float scaleValue;
		scaleValue = obj.GetScaleBarValue();
		selecteItem = scene.GetActiveModelIndex();
		ImGui::ListBox("active model", &selecteItem, currentModels, scene.GetModelCount(), 2);
		scene.SetActiveModelIndex(selecteItem);

		ImGui::InputFloat("x", &x, 0.01f, 1.0f);
		ImGui::InputFloat("y", &y, 0.01f, 1.0f);
		ImGui::InputFloat("z", &z, 0.01f, 1.0f);
		ImGui::InputFloat("alfa", &alpha, 0.01f, 1.0f);
		//transformations GUI
		bool translationFlag = ImGui::Button("Translate");
		ImGui::SameLine();
		bool scalingFlag = ImGui::Button("Scale");
		ImGui::SameLine();
		bool rotateFlag = ImGui::Button("Rotate");
		ImGui::SameLine();
		bool clearFlag = ImGui::Button("Clear");




		static bool checkedLocal = true;
		static bool checkedWorld = false;

		ImGui::SameLine();
		//local \ world GUI
		bool localFlag = ImGui::Checkbox("local", &checkedLocal);
		ImGui::SameLine();
		bool worldFlag = ImGui::Checkbox("world", &checkedWorld);
		//Operates the transformations according to the flag operated
		if (clearFlag) {

		}
		//translation
		if (translationFlag) {

			if (checkedLocal) {
				obj.LocalTranslateTransform(x, y, z);
			}
			if (checkedWorld) {
				obj.WorldTranslateTransform(x, y, z);
			}
			x = y = z = 0.0f;

		}

		//scaling
		if (scalingFlag) {

			if (checkedLocal) {
				obj.LocalScaleTransform(x, y, z);
			}
			if (checkedWorld) {
				obj.WorldScaleTransform(x, y, z);
			}
			x = y = z = 0.0f;
		}

		//rotate
		if (rotateFlag) {
			r = 1;
			if (checkedLocal) {
				obj.LocalRotationTransform_Z(alpha);
				//obj.position = alpha;
				alpha = 0.0f;
			}
			if (checkedWorld) {
				obj.WorldRotationTransform(alpha);
				alpha = 0.0f;
			}
		}


		else r = 0;
		
		//Scale Slider
		ImGui::SliderFloat("ScaleSlider", &scaleValue, 1.0f, 1000.0f);
		obj.SetScaleBarValue(scaleValue);

		ImGui::SliderFloat("Rotate_Z", &obj.localRotateBarValue_Z , 0, 360.0f);
		ImGui::SliderFloat("Rotate_X", &obj.localRotateBarValue_X , 0, 360.0f);
		ImGui::SliderFloat("Rotate_Y", &obj.localRotateBarValue_Y , 0, 360.0f);

		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color



		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();



		// 3. Show another simple window.
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			
			static float scaleValue1;
			Camera& camera = scene.GetActiveCamera();
			scaleValue1 = camera.GetScaleBarValue();
			ImGui::Begin("Cameras", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)

			camera.SetActiveProjection(ImGui::Button("change Projection"));
			ImGui::SliderFloat("ScaleSlider1", &scaleValue1, 0.0f, 1000.0f);
			camera.SetScaleBarValue(scaleValue1);
			ImGui::SliderFloat("Rotate_Z", &camera.localRotateBarValue_Z, 0, 360.0f);
			ImGui::SliderFloat("Rotate_X", &camera.localRotateBarValue_X, 0, 360.0f);
			ImGui::SliderFloat("Rotate_Y", &camera.localRotateBarValue_Y, 0, 360.0f);
			if (ImGui::TreeNode("Perspeective"))
			{


				ImGui::SliderFloat("fovy", &camera.fovy, -90.0f, 90.0f);
				ImGui::SliderFloat("aspect", &camera.aspect, 0.5f, 2.0f);
				ImGui::SliderFloat("near", &camera.zNear, 0, 500.0f);
				ImGui::SliderFloat("far", &camera.zFar, camera.zNear, 1000.0f);
				static bool closable_group = true;
				ImGui::Checkbox("Show 2nd header", &closable_group);
				if (ImGui::CollapsingHeader("Header", ImGuiTreeNodeFlags_None))
				{
					ImGui::Text("IsItemHovered: %d", ImGui::IsItemHovered());
					for (int i = 0; i < 5; i++)
						ImGui::Text("Some content %d", i);
				}
				if (ImGui::CollapsingHeader("Header with a close button", &closable_group))
				{
					ImGui::Text("IsItemHovered: %d", ImGui::IsItemHovered());
					for (int i = 0; i < 5; i++)
						ImGui::Text("More content %d", i);
				}
				/*
				if (ImGui::CollapsingHeader("Header with a bullet", ImGuiTreeNodeFlags_Bullet))
					ImGui::Text("IsItemHovered: %d", ImGui::IsItemHovered());
				*/
				ImGui::TreePop();
			}

			//scene.GetActiveCamera().SetActiveProjection(ImGui::Button("change Projection"));
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}
	}
}


