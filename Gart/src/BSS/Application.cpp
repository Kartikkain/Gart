#include "bsspch.h"
#include "Application.h"
#include "Log.h"
#include <glad/glad.h>
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

		glGenVertexArrays(1, &m_vertexarray);
		glBindVertexArray(m_vertexarray);


		float vertex[3 * 7] =
		{
			-0.5f,-0.5f,0.0f,  1.0f,0.0f,1.0f,1.0f,
			 0.5f,-0.5f,0.0f,  0.0f,1.0f,1.0f,1.0f,
			 0.0f, 0.5f,0.0f,  0.0f,0.0f,1.0f,1.0f
		};
		
		m_vertexBuffer.reset(Gart::VertexBuffer::Create(vertex, sizeof(vertex)));

		{

			Gart::BufferLayout layout = {
				{Gart::ShaderDataType::Float3, "a_Position"},
				{Gart::ShaderDataType::Float4, "a_Color"}
			};

			m_vertexBuffer->SetLayout(layout);
		}

		Gart::BufferLayout m_layout = m_vertexBuffer->GetLayout();

		uint32_t index = 0;
		for (const auto& element : m_layout)
		{

			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, 
				element.GetElementCount(),
				ShaderDataTypeToOpenGlBaseType(element.Type), 
				element.Normalize ? GL_TRUE :GL_FALSE, 
				m_layout.GetStride(),
				(const void*) element.Offset);
			index++;
		}
		


		unsigned int indicies[3] = { 0,1,2 };
		m_IndexBuffer.reset(Gart::IndexBuffer::Create(indicies, sizeof(indicies) / sizeof(uint32_t)));


		const std::string vertexsrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec4 v_Color;
			
			void main()
			{
				v_Color = a_Color;
				gl_Position = vec4(a_Position,1.0);
			}
		)";


		const std::string fragsrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 fragColor;
			
			in vec4 v_Color;
				
			void main()
			{
				fragColor = v_Color;
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
