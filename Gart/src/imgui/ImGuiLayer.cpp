 
#include "bsspch.h"
#include "ImGuiLayer.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "Core/Application.h"

#define IMGUI_IMPL_API
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

// temperory
#include <GLFW/glfw3.h>

#include "ImGuizmo.h"

namespace BSS
{
	ImGuiLayer::ImGuiLayer()
		:Layer("ImGuiLayer")
	{
	}
	ImGuiLayer::~ImGuiLayer()
	{
	}
	void ImGuiLayer::OnAttach()
	{
		GART_PROFILE_FUNCTION();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); 
		(void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		float fontSize = 18.0f;
		io.Fonts->AddFontFromFileTTF("assets/Fonts/Roboto_Condensed/static/RobotoCondensed-Bold.ttf", fontSize);
		io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/Fonts/Roboto_Condensed/static/RobotoCondensed-Regular.ttf", fontSize);
		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}
		
		SetDarkThemeColors();
		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());


		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}
	void ImGuiLayer::OnDitach()
	{
		GART_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	

	void ImGuiLayer::OnImGuiRender()
	{
		GART_PROFILE_FUNCTION();

	}

	void ImGuiLayer::OnEvent(Event& event)
	{
		if (m_blockEvent)
		{
			ImGuiIO& io = ImGui::GetIO();
			event.m_Handled |= event.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			event.m_Handled |= event.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}

	void ImGuiLayer::Begin()
	{
		GART_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void ImGuiLayer::End()
	{
		GART_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	uint32_t ImGuiLayer::ImguiActiveWidget()
	{
		return GImGui->ActiveId;
	}

	void ImGuiLayer::SetDarkThemeColors()
	{
		auto& color = ImGui::GetStyle().Colors;

		color[ImGuiCol_WindowBg] = ImVec4{ 0.1f,0.105f,0.11f,1.0f };

		//Header
		color[ImGuiCol_Header] = ImVec4{0.2f,0.205f,0.21f,1.0f};
		color[ImGuiCol_HeaderHovered] = ImVec4{0.3f,0.305f,0.31f,1.0f};
		color[ImGuiCol_HeaderActive] = ImVec4{0.15f,0.1505f,0.151f,1.0f};

		//Button
		color[ImGuiCol_Button] = ImVec4{ 0.2f,0.205f,0.21f,1.0f };
		color[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f,0.305f,0.31f,1.0f };
		color[ImGuiCol_ButtonActive] = ImVec4{ 0.15f,0.1505f,0.151f,1.0f };

		//Frame
		color[ImGuiCol_FrameBg] = ImVec4{ 0.2f,0.205f,0.21f,1.0f };
		color[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f,0.305f,0.31f,1.0f };
		color[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f,0.1505f,0.151f,1.0f };

		//Tabs
		color[ImGuiCol_Tab] = ImVec4{ 0.15f,0.1505f,0.151f,1.0f };
		color[ImGuiCol_TabHovered] = ImVec4{ 0.3f,0.305f,0.31f,1.0f };
		color[ImGuiCol_TabActive] = ImVec4{ 0.2f,0.205f,0.21f,1.0f };
		color[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f,0.1505f,0.151f,1.0f };
		color[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f,0.205f,0.21f,1.0f };

		//TitleBar
		color[ImGuiCol_TitleBg] = ImVec4{ 0.15f,0.1505f,0.151f,1.0f };
		color[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f,0.1505f,0.151f,1.0f };
		color[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.95f,0.1505f,0.951f,1.0f };
	}
}