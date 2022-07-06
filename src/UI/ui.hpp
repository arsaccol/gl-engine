#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <functional>

struct GUIstate
{

};

class UI
{
public:
	UI() {}
	~UI() {}
	void Init(GLFWwindow* windowPtr);
	void Shutdown();
	void Frame(const std::function<void()>& OpenGLRenderFunction);
	void BeginFrame();
	void EndFrame();

public:
	void RenderGUI();

private:
	GLFWwindow* window = nullptr;


};

void UI::Init(GLFWwindow* windowPtr)
{
	window = windowPtr;

	// setup ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	// setup ImGui GLFW binding
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	ImGui::StyleColorsDark();
}

void UI::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void UI::BeginFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	// ImGui window/draw calls should start here 
}

void UI::EndFrame()
{
	// ImGui window/draw calls should have ended prior to this point
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void UI::Frame(const std::function<void()>& SceneRenderFunction)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	SceneRenderFunction();
	RenderGUI();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void UI::RenderGUI()
{
	using namespace ImGui;

	SetNextWindowSize(ImVec2{ 100, 50 });
	Begin("Hello");
#ifdef _DEBUG
	Text("DEBUG");
#endif

	Button("world!");
	//ShowDemoWindow();
	End();
}
