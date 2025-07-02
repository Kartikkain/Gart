#pragma once


#include "Core.h"
#include "Window.h"
#include "Events/Event.h"
#include "Core/LayerStack.h"
#include "Events/ApplicationEvent.h"
#include "imgui/ImGuiLayer.h"
#include "Core/TimeStep.h"
#include "Log.h"
#include "Renderer/VertexArray.h"
#include "Renderer/OrthoGraphicCamera.h"
#include "Renderer/Renderer.h"
namespace BSS
{
	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const
		{
			BSS_CORE_ASSERT(index < Count,"Something's Wrong");
			return Args[index];
		}
	};

	class BSS_API Application
	{
	public:
		//Application(const std::string& name = "Gart App");
		Application(const std::string& name = "Gart App",ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
		virtual ~Application();
		void Run();
		void Close();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

		ApplicationCommandLineArgs GetCommandLineArgs() const { return m_CommandLineArgs; }
	private:

		float m_LastFrameTime = 0.0f;

		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		Gart::Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimize = false;
		LayerStack m_LayerStack;
		ApplicationCommandLineArgs m_CommandLineArgs;
		static Application* s_Instance;
		
	};

	//Application* CreateApplication();
	Application* CreateApplication(ApplicationCommandLineArgs args);
}

