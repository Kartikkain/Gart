#include <BSS.h>
#include "Core/EntryPoint.h"
#include "Platform/OpenGl/OpenGLShader.h"
#include "imgui.h"
#include <glm/gtc/type_ptr.hpp>
#include "Sandbox2D.h"
class ExampleLayer :public BSS::Layer
{
public:
	ExampleLayer()
		:Layer("Example"), m_OrthoCamera(1280.0f/720.0f), m_CameraPosition(0.0f),m_ModelTransform(0.0f)
	{
		m_VertexArray = Gart::VertexArray::Create();


		float vertex[4 * 9] =
		{
			-0.5f,-0.5f,0.0f,  1.0f,0.0f,1.0f,1.0f, 0.0f,0.0f,
			 0.5f,-0.5f,0.0f,  0.0f,1.0f,1.0f,1.0f, 1.0f,1.0f,
			 0.5f, 0.5f,0.0f,  0.0f,0.0f,1.0f,1.0f, 1.0f,0.0f,
			-0.5f, 0.5f,0.0f,  0.0f,0.0f,1.0f,1.0f, 0.0f,1.0f
		};

		

		m_vertexBuffer.reset(Gart::VertexBuffer::Create(vertex, sizeof(vertex)));


		Gart::BufferLayout layout = {
			{Gart::ShaderDataType::Float3, "a_Position"},
			{Gart::ShaderDataType::Float4, "a_Color"},
			{Gart::ShaderDataType::Float2, "a_Texture"}
		};

		m_vertexBuffer->SetLayout(layout);

		m_VertexArray->AddVertexBuffer(m_vertexBuffer);

		unsigned int indicies[6] = { 0,1,2,2,3,0 };
		m_IndexBuffer.reset(Gart::IndexBuffer::Create(indicies, sizeof(indicies) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		auto m_Shader = m_shadeLibrary.Load("assets/shaders/Texture.glsl");
		//m_Shader = Gart::Shader::Create("assets/shaders/Texture.glsl");
		m_Texture = Gart::Texture2D::Create("assets/textures/smile.png");
		m_Shader->Bind();
		std::dynamic_pointer_cast<Gart::OpenGLShader>(m_Shader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Gart::TimeStep ts) override
	{
		BSS_CLIENT_INFO("Delta Time :: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliSeconds());

		m_OrthoCamera.OnUpdate(ts);
		

		if (BSS::Input::IsKeyPressed(BSS_KEY_A)) { m_ModelTransform.x -= m_modelspeed * ts; }
		else if (BSS::Input::IsKeyPressed(BSS_KEY_D)) { m_ModelTransform.x += m_modelspeed * ts; }
		else if (BSS::Input::IsKeyPressed(BSS_KEY_W)) { m_ModelTransform.y += m_modelspeed * ts; }
		else if (BSS::Input::IsKeyPressed(BSS_KEY_S)) { m_ModelTransform.y -= m_modelspeed * ts; }
		

		Gart::RenderCommand::SetClearColor({ 0.1f,0.1f,0.1f,1 });
		Gart::RenderCommand::Clear();

		Gart::Renderer::BeginScene(m_OrthoCamera.GetCamera());
		auto m_Shader = m_shadeLibrary.Get("Texture");
		m_Shader->Bind();
		std::dynamic_pointer_cast<Gart::OpenGLShader>(m_Shader)->UploadUniformMat4("u_ViewProjectionMatrix", m_OrthoCamera.GetCamera().GetViewProjectionMatrix());
		std::dynamic_pointer_cast<Gart::OpenGLShader>(m_Shader)->UploadUniformFloat3("u_Color", m_TriangleColor);
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_ModelTransform);

		m_Texture->Bind();
		Gart::Renderer::Submit(m_VertexArray,m_Shader,transform);
		Gart::Renderer::EndScene();
	}

	void OnImGuiRender()
	{
		ImGui::Begin("Setting");
		ImGui::ColorEdit3("Triangle Color", glm::value_ptr(m_TriangleColor));
		ImGui::End();
	}

	void OnEvent(BSS::Event& e)
	{
		//BSS_CLIENT_TRACE("{0}", e);
		m_OrthoCamera.OnEvent(e);
	}

private :
	Gart::OrthoGraphicCameraController m_OrthoCamera;
	Gart::ShaderLibrary m_shadeLibrary;
	//Gart::Ref<Gart::Shader> m_Shader;
	Gart::Ref<Gart::VertexArray> m_VertexArray;
	Gart::Ref<Gart::VertexBuffer> m_vertexBuffer;
	Gart::Ref<Gart::IndexBuffer> m_IndexBuffer;
	Gart::Ref<Gart::Texture2D> m_Texture;

	glm::vec3 m_CameraPosition;
	float m_CameraSpeed = 1.0f;

	glm::vec3 m_ModelTransform;
	float m_modelspeed = 0.5f;
	glm::vec3 m_TriangleColor = { 1.0f,0.0f,0.0f };
};
class Sandbox : public BSS::Application
{
public:
	Sandbox()
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
		
	}

	~Sandbox()
	{

	}

};

BSS::Application* BSS::CreateApplication()
{
	return new Sandbox();
}