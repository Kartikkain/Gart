#include "bsspch.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Renderer/RenderCommand.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Gart
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TillingFactor;
	};

	struct Renderer2DStorage
	{
		const uint32_t MaxQuads = 10000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTexureSlots = 32;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t QuadindexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTexureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;

		glm::vec4 QuadVertexPosition[4];
	};

	
	static Renderer2DStorage s_Data;

	void Renderer2D::init()
	{
		GART_PROFILE_FUNCTION();

		s_Data.QuadVertexArray = VertexArray::Create();


		float vertex[5 * 4] =
		{
			-0.5f,-0.5f,0.0f,  0.0f,0.0f,
			 0.5f,-0.5f,0.0f,  1.0f,1.0f,
			 0.5f, 0.5f,0.0f,  1.0f,0.0f,
			-0.5f, 0.5f,0.0f,  0.0f,1.0f
		};


		
		s_Data.QuadVertexBuffer.reset(VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex)));


		Gart::BufferLayout layout = {
			{Gart::ShaderDataType::Float3, "a_Position"},
			{Gart::ShaderDataType::Float4, "a_Color"},
			{Gart::ShaderDataType::Float2, "a_Texture"},
			{Gart::ShaderDataType::Float, "a_TexIndex"},
			{Gart::ShaderDataType::Float, "a_TillingFactor"}
		};

		s_Data.QuadVertexBuffer->SetLayout(layout);

		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		uint32_t* quadIndicies = new uint32_t[s_Data.MaxIndices];

		uint32_t Offset = 0;
		
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			quadIndicies[i + 0] = Offset + 0;
			quadIndicies[i + 1] = Offset + 1;
			quadIndicies[i + 2] = Offset + 2;

			quadIndicies[i + 3] = Offset + 2;
			quadIndicies[i + 4] = Offset + 3;
			quadIndicies[i + 5] = Offset + 0;

			Offset += 4;
		}
		unsigned int indicies[6] = { 0,1,2,2,3,0 };
		Ref<IndexBuffer> m_IndexBuffer;
		m_IndexBuffer.reset(IndexBuffer::Create(quadIndicies, s_Data.MaxIndices ));
		s_Data.QuadVertexArray->SetIndexBuffer(m_IndexBuffer);
		delete[] quadIndicies;
		//auto m_Shader = m_shadeLibrary.Load("assets/shaders/Texture.glsl");

		int32_t samplers[s_Data.MaxTexureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTexureSlots; i++)
			samplers[i] = i;
		s_Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetIntArray("u_Texture",samplers,s_Data.MaxTexureSlots);

		s_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whitetexturedata = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whitetexturedata, sizeof(uint32_t));

		
		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		s_Data.QuadVertexPosition[0] = { -0.5f,-0.5f,0.0f,1.0f };
		s_Data.QuadVertexPosition[1] = {  0.5f,-0.5f,0.0f,1.0f };
		s_Data.QuadVertexPosition[2] = {  0.5f, 0.5f,0.0f,1.0f };
		s_Data.QuadVertexPosition[3] = { -0.5f, 0.5f,0.0f,1.0f };
	}
	void Renderer2D::Shutdown()
	{
		GART_PROFILE_FUNCTION();
	}
	void Renderer2D::BeginScene(const OrthoGraphicCamera& camera)
	{
		GART_PROFILE_FUNCTION();

		/*s_Data.FlatColorShader->Bind();
		s_Data.FlatColorShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());
		*/

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());

		s_Data.QuadindexCount = 0;
		s_Data.TextureSlotIndex = 1;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

	}
	void Renderer2D::EndScene()
	{
		GART_PROFILE_FUNCTION();
		uint32_t datasize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, datasize);
		Flush();
	}

	void Renderer2D::Flush()
	{ 
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
		{
			s_Data.TextureSlots[i]->Bind(i);
		}
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadindexCount);
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		GART_PROFILE_FUNCTION();

		DrawQuad({ position.x,position.y,0.0f }, size, color);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		GART_PROFILE_FUNCTION();

		float textureIndex = 0.0f;
		float tilling = 1.0f;
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[0];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = {0.0f,0.0f};
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TillingFactor = tilling;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[1];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f,0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TillingFactor = tilling;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[2];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f,1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TillingFactor = tilling;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[3];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f,1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TillingFactor = tilling;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadindexCount += 6;

	}

	void Renderer2D::DrawQuad(const glm::vec2 & position, const glm::vec2 & size, const Ref<Texture2D> texture , float tilling, glm::vec4 tintcolor)
	{
		GART_PROFILE_FUNCTION();

		DrawQuad({ position.x,position.y,0.0f }, size, texture,tilling,tintcolor);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture , float tilling, glm::vec4 tintcolor)
	{
		GART_PROFILE_FUNCTION();

		constexpr glm::vec4 color = { 1.0f,1.0f,1.0f,1.0f };
		
		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = float(i);
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[textureIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[0];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f,0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TillingFactor = tilling;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[1];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f,0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TillingFactor = tilling;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[2];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f,1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TillingFactor = tilling;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[3];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f,1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TillingFactor = tilling;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadindexCount += 6;

	}
	void Renderer2D::DrawRotateQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color)
	{
		DrawRotateQuad({ position.x,position.y,0.0f }, rotation, size, color);
	}
	void Renderer2D::DrawRotateQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color)
	{
		GART_PROFILE_FUNCTION();

		float textureIndex = 0.0f;
		float tilling = 1.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3({ 0.0f,0.0f,1.0f })) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[0];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f,0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TillingFactor = tilling;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[1];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f,0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TillingFactor = tilling;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[2];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f,1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TillingFactor = tilling;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[3];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f,1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TillingFactor = tilling;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadindexCount += 6;

#if OLD_PATH
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetFloat4("u_Color", glm::vec4(color));
		s_Data.TextureShader->SetFloat("u_Tilling", 1.0f);

		// Bind White Texture.

		s_Data.WhiteTexture->Bind();

		// POV : Transform matrix caluclation should be like translate * rotation * scaling.

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));
		s_Data.TextureShader->SetMat4("u_Transform", transform);
		s_Data.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray);
#endif
	}
	void Renderer2D::DrawRotateQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<Texture2D> texture, float tilling, glm::vec4 tintcolor)
	{
		DrawRotateQuad({ position.x,position.y,0.0f }, rotation, size, texture, tilling,tintcolor);
	}
	void Renderer2D::DrawRotateQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<Texture2D> texture, float tilling, glm::vec4 tintcolor)
	{
		GART_PROFILE_FUNCTION();

		constexpr glm::vec4 color = { 1.0f,1.0f,1.0f,1.0f };

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = float(i);
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[textureIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3({ 0.0f,0.0f,1.0f })) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[0];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f,0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TillingFactor = tilling;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[1];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f,0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TillingFactor = tilling;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[2];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f,1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TillingFactor = tilling;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[3];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f,1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TillingFactor = tilling;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadindexCount += 6;
	}
}