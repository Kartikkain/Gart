#pragma once


#include "Core.h"
#include "Window.h"
#include "Events/Event.h"
#include "BSS/LayerStack.h"
#include "Events/ApplicationEvent.h"
#include "imgui/ImGuiLayer.h"
//#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Renderer/OrthoGraphicCamera.h"
#include "Renderer/Renderer.h"
namespace BSS
{
	class BSS_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
		static Application* s_Instance;
		/*Gart::OrthoGraphicCamera m_OrthoCamera;

		std::shared_ptr<Gart::Shader> m_Shader;
		std::shared_ptr<Gart::VertexArray> m_VertexArray;
		std::shared_ptr<Gart::VertexBuffer> m_vertexBuffer;
		std::shared_ptr<Gart::IndexBuffer> m_IndexBuffer;*/
	};

	Application* CreateApplication();
}

