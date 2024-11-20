#include "bsspch.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Renderer/RenderCommand.h"
#include "Platform/OpenGl/OpenGLShader.h"

namespace Gart
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> FlatColorShader;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::init()
	{
		s_Data = new Renderer2DStorage();
		s_Data->QuadVertexArray = VertexArray::Create();


		float vertex[4 * 7] =
		{
			-0.5f,-0.5f,0.0f,  1.0f,0.0f,1.0f,1.0f,
			 0.5f,-0.5f,0.0f,  0.0f,1.0f,1.0f,1.0f,
			 0.5f, 0.5f,0.0f,  0.0f,0.0f,1.0f,1.0f,
			-0.5f, 0.5f,0.0f,  0.0f,0.0f,1.0f,1.0f,
		};


		Ref<VertexBuffer> m_vertexBuffer;
		m_vertexBuffer.reset(VertexBuffer::Create(vertex, sizeof(vertex)));


		Gart::BufferLayout layout = {
			{Gart::ShaderDataType::Float3, "a_Position"},
			{Gart::ShaderDataType::Float4, "a_Color"},
		};

		m_vertexBuffer->SetLayout(layout);

		s_Data->QuadVertexArray->AddVertexBuffer(m_vertexBuffer);

		unsigned int indicies[6] = { 0,1,2,2,3,0 };
		Ref<IndexBuffer> m_IndexBuffer;
		m_IndexBuffer.reset(IndexBuffer::Create(indicies, sizeof(indicies) / sizeof(uint32_t)));
		s_Data->QuadVertexArray->SetIndexBuffer(m_IndexBuffer);

		//auto m_Shader = m_shadeLibrary.Load("assets/shaders/Texture.glsl");
		s_Data->FlatColorShader = Shader::Create("assets/shaders/FlatColor.glsl");
		//m_Texture = Gart::Texture2D::Create("assets/textures/smile.png");
		s_Data->FlatColorShader->Bind();
	}
	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}
	void Renderer2D::BeginScene(const OrthoGraphicCamera& camera)
	{
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->UploadUniformMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->UploadUniformMat4("u_Transform", glm::mat4(1.0f));
	}
	void Renderer2D::EndScene()
	{
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x,position.y,0.0f }, size, color);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_Data->FlatColorShader)->UploadUniformFloat4("u_Color", glm::vec4(color));
		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}
}