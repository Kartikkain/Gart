#include "bsspch.h"
#include "Application.h"
#include "Log.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Input.h"


namespace BSS
{
#define BSS_EVENT_FN(x) std::bind(&Application::x,this,std::placeholders::_1)
	
	Application* Application::s_Instance = nullptr;

	static GLenum ShaderDataTypeToOpenGlBaseType(Gart::ShaderDataType type)
	{

		switch (type)
		{
			case Gart::ShaderDataType::Float:   return GL_FLOAT;
			case Gart::ShaderDataType::Float2:  return GL_FLOAT;
			case Gart::ShaderDataType::Float3:  return GL_FLOAT;
			case Gart::ShaderDataType::Float4:  return GL_FLOAT;
			case Gart::ShaderDataType::Mat3:    return GL_FLOAT;
			case Gart::ShaderDataType::Mat4:    return GL_FLOAT;
			case Gart::ShaderDataType::Int:     return GL_INT;
			case Gart::ShaderDataType::Int2:    return GL_INT;
			case Gart::ShaderDataType::Int3:    return GL_INT;
			case Gart::ShaderDataType::Int4:    return GL_INT;
			case Gart::ShaderDataType::Bool:    return GL_INT;
		}
		return 0;
	}

	Application::Application()
	{
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BSS_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		
	}

	Application::~Application()
	{

	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BSS_EVENT_FN(OnWindowClose));
		BSS_CORE_INFO("{0}", e);
		
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.m_Handled)
				break;
		}
	}
	void Application::Run()
	{
		
		while (m_Running)
		{
			
			float l_time = (float)glfwGetTime();
			Gart::TimeStep timestep = l_time - m_LastFrameTime;
			m_LastFrameTime = l_time;
			glClear(GL_COLOR_BUFFER_BIT);


			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(timestep);

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();
			m_Window->OnUpdate();
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

}
