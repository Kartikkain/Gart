#include "bsspch.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Renderer/RenderCommand.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Gart
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::init()
	{
		s_Data = new Renderer2DStorage();
		s_Data->QuadVertexArray = VertexArray::Create();


		float vertex[5 * 4] =
		{
			-0.5f,-0.5f,0.0f,  0.0f,0.0f,
			 0.5f,-0.5f,0.0f,  1.0f,1.0f,
			 0.5f, 0.5f,0.0f,  1.0f,0.0f,
			-0.5f, 0.5f,0.0f,  0.0f,1.0f
		};


		Ref<VertexBuffer> m_vertexBuffer;
		m_vertexBuffer.reset(VertexBuffer::Create(vertex, sizeof(vertex)));


		Gart::BufferLayout layout = {
			{Gart::ShaderDataType::Float3, "a_Position"},
			{Gart::ShaderDataType::Float2, "a_Texture"}
		};

		m_vertexBuffer->SetLayout(layout);

		s_Data->QuadVertexArray->AddVertexBuffer(m_vertexBuffer);

		unsigned int indicies[6] = { 0,1,2,2,3,0 };
		Ref<IndexBuffer> m_IndexBuffer;
		m_IndexBuffer.reset(IndexBuffer::Create(indicies, sizeof(indicies) / sizeof(uint32_t)));
		s_Data->QuadVertexArray->SetIndexBuffer(m_IndexBuffer);

		//auto m_Shader = m_shadeLibrary.Load("assets/shaders/Texture.glsl");
		s_Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetInt("u_Texture", 0);

		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whitetexturedata = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whitetexturedata, sizeof(uint32_t));
	}
	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}
	void Renderer2D::BeginScene(const OrthoGraphicCamera& camera)
	{
		/*s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());
		*/

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());

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
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetFloat4("u_Color", glm::vec4(color));

		// Bind White Texture.

		s_Data->WhiteTexture->Bind();

		// POV : Transform matrix caluclation should be like translate * rotation * scaling.

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));
		s_Data->TextureShader->SetMat4("u_Transform", transform);
		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2 & position, const glm::vec2 & size, const Ref<Texture2D> texture)
	{
		DrawQuad({ position.x,position.y,0.0f }, size, texture);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture)
	{
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetFloat4("u_Color", glm::vec4(1.0f));

		// POV : Transform matrix caluclation should be like translate * rotation * scaling.

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));
		s_Data->TextureShader->SetMat4("u_Transform", transform);
		texture->Bind();
		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}
}