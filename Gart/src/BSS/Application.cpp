#include "bsspch.h"
#include "Application.h"
#include "Log.h"
#include <glad/glad.h>
#include "Input.h"
namespace BSS
{
#define BSS_EVENT_FN(x) std::bind(&Application::x,this,std::placeholders::_1)
	
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BSS_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		glGenVertexArrays(1, &m_vertexarray);
		glBindVertexArray(m_vertexarray);

		/*glGenBuffers(1, &m_vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);*/

		float vertex[3 * 3] =
		{
			-0.5f,-0.5f,0.0f,
			 0.5f,-0.5f,0.0f,
			 0.0f, 0.5f,0.0f,
		};
		
		m_vertexBuffer.reset(Gart::VertexBuffer::Create(vertex, sizeof(vertex)));

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		/*glGenBuffers(1, &m_indexbuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexbuffer);*/

		unsigned int indicies[3] = { 0,1,2 };
		m_IndexBuffer.reset(Gart::IndexBuffer::Create(indicies, sizeof(indicies) / sizeof(uint32_t)));

		/*glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);*/

		const std::string vertexsrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			
			void main()
			{
				gl_Position = vec4(a_Position,1.0);
			}
		)";


		const std::string fragsrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 fragColor;
			
			void main()
			{
				fragColor = vec4(0.3,0.8,0.2,1.0);
			}
		)";

		m_Shader.reset(new Gart::Shader(vertexsrc,fragsrc));
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
			
			glClear(GL_COLOR_BUFFER_BIT);

			m_Shader->Bind();
			glBindVertexArray(m_vertexarray);
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

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
