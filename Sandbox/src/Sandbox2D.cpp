#include "Sandbox2D.h"
#include "Platform/OpenGl/OpenGLShader.h"
#include "imgui.h"
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"), m_OrthoCamera(1280.0f / 720.0f)
{

}

void Sandbox2D::OnAttach()
{
	m_VertexArray = Gart::VertexArray::Create();


	float vertex[4 * 7] =
	{
		-0.5f,-0.5f,0.0f,  1.0f,0.0f,1.0f,1.0f, 
		 0.5f,-0.5f,0.0f,  0.0f,1.0f,1.0f,1.0f, 
		 0.5f, 0.5f,0.0f,  0.0f,0.0f,1.0f,1.0f,
		-0.5f, 0.5f,0.0f,  0.0f,0.0f,1.0f,1.0f,
	};


	Gart::Ref<Gart::VertexBuffer> m_vertexBuffer;
	m_vertexBuffer.reset(Gart::VertexBuffer::Create(vertex, sizeof(vertex)));


	Gart::BufferLayout layout = {
		{Gart::ShaderDataType::Float3, "a_Position"},
		{Gart::ShaderDataType::Float4, "a_Color"},
	};

	m_vertexBuffer->SetLayout(layout);

	m_VertexArray->AddVertexBuffer(m_vertexBuffer);

	unsigned int indicies[6] = { 0,1,2,2,3,0 };
	Gart::Ref<Gart::IndexBuffer> m_IndexBuffer;
	m_IndexBuffer.reset(Gart::IndexBuffer::Create(indicies, sizeof(indicies) / sizeof(uint32_t)));
	m_VertexArray->SetIndexBuffer(m_IndexBuffer);

	//auto m_Shader = m_shadeLibrary.Load("assets/shaders/Texture.glsl");
	m_Shader = Gart::Shader::Create("assets/shaders/FlatColor.glsl");
	//m_Texture = Gart::Texture2D::Create("assets/textures/smile.png");
	m_Shader->Bind();
}

void Sandbox2D::OnDitach()
{
}

void Sandbox2D::OnUpdate(Gart::TimeStep ts)
{
	m_OrthoCamera.OnUpdate(ts);

	Gart::RenderCommand::SetClearColor({ 0.1f,0.1f,0.1f,1 });
	Gart::RenderCommand::Clear();

	Gart::Renderer2D::BeginScene(m_OrthoCamera.GetCamera());
	m_Shader->Bind();

	//Gart::Renderer::Submit(m_VertexArray, m_Shader);
	Gart::Renderer2D::DrawQuad({ 0.0f,0.0f }, { 1.0f,1.0f }, { 0.8f,0.2f,0.3f,1.0f });
	Gart::Renderer2D::EndScene();

	/*std::dynamic_pointer_cast<Gart::OpenGLShader>(m_Shader)->UploadUniformMat4("u_ViewProjectionMatrix", m_OrthoCamera.GetCamera().GetViewProjectionMatrix());
	std::dynamic_pointer_cast<Gart::OpenGLShader>(m_Shader)->UploadUniformFloat3("u_Color", m_TriangleColor);*/

}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Setting");
	ImGui::ColorEdit3("Triangle Color", glm::value_ptr(m_TriangleColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(BSS::Event& e)
{
	m_OrthoCamera.OnEvent(e);
}
