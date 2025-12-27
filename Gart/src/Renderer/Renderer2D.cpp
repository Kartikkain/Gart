#include "bsspch.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "UniformBuffer.h"
#include "Renderer/RenderCommand.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "MSDFData.h"

namespace Gart
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TillingFactor;
		// Editor Only
		int EntityID;
	};

	struct CircleVertex
	{
		glm::vec3 WorldPosition;
		glm::vec3 LocalPosition;
		glm::vec4 Color;
		float Thickness;
		float Fade;
		// Editor Only
		int EntityID;
	};

	struct LineVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		
		// Editor Only
		int EntityID;
	};

	struct TextVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		
		// Editor Only
		int EntityID;
	};

	struct Renderer2DStorage
	{
		static const uint32_t MaxQuads = 1000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTexureSlots = 32;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;

		Ref<VertexArray> CircleVertexArray;
		Ref<VertexBuffer> CircleVertexBuffer;
		Ref<Shader> CircleShader;

		Ref<VertexArray> LineVertexArray;
		Ref<VertexBuffer> LineVertexBuffer;
		Ref<Shader> LineShader;


		Ref<VertexArray> TextVertexArray;
		Ref<VertexBuffer> TextVertexBuffer;
		Ref<Shader> TextShader;

		Ref<Texture2D> WhiteTexture;


		uint32_t QuadindexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		uint32_t CircleindexCount = 0;
		CircleVertex* CircleVertexBufferBase = nullptr;
		CircleVertex* CircleVertexBufferPtr = nullptr;

		uint32_t LineindexCount = 0;
		LineVertex* LineVertexBufferBase = nullptr;
		LineVertex* LineVertexBufferPtr = nullptr;
		float LineWidth = 2.0f;

		uint32_t TextindexCount = 0;
		TextVertex* TextVertexBufferBase = nullptr;
		TextVertex* TextVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTexureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;

		Ref<Texture2D> FontAtlasTexture;

		glm::vec4 QuadVertexPosition[4];
		Renderer2D::Statistics Stats;

		struct CameraData
		{
			glm::mat4 ViewProjection;
		};
		CameraData CameraBuffer;
		Ref<UniformBuffer> CameraUniformBuffer;
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
			{Gart::ShaderDataType::Float, "a_TillingFactor"},
			{Gart::ShaderDataType::Int, "a_EntityID"}
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
		

		//Circle

		s_Data.CircleVertexArray = VertexArray::Create();
		s_Data.CircleVertexBuffer.reset(VertexBuffer::Create(s_Data.MaxVertices * sizeof(CircleVertex)));

		s_Data.CircleVertexBuffer->SetLayout({
			{Gart::ShaderDataType::Float3, "a_WorldPosition"},
			{Gart::ShaderDataType::Float3, "a_LocalPosition"},
			{Gart::ShaderDataType::Float4, "a_Color"},
			{Gart::ShaderDataType::Float, "a_Thickness"},
			{Gart::ShaderDataType::Float, "a_Fade"},
			{Gart::ShaderDataType::Int, "a_EntityID"}
			});

		s_Data.CircleVertexArray->AddVertexBuffer(s_Data.CircleVertexBuffer);
		s_Data.CircleVertexArray->SetIndexBuffer(m_IndexBuffer);
		s_Data.CircleVertexBufferBase = new CircleVertex[s_Data.MaxIndices];

		//Line

		s_Data.LineVertexArray = VertexArray::Create();
		s_Data.LineVertexBuffer.reset(VertexBuffer::Create(s_Data.MaxVertices * sizeof(LineVertex)));

		s_Data.LineVertexBuffer->SetLayout({
			{Gart::ShaderDataType::Float3, "a_Position"},
			{Gart::ShaderDataType::Float4, "a_Color"},
			{Gart::ShaderDataType::Int, "a_EntityID"}
			});

		s_Data.LineVertexArray->AddVertexBuffer(s_Data.LineVertexBuffer);
		s_Data.LineVertexBufferBase = new LineVertex[s_Data.MaxIndices];

		// Text

		s_Data.TextVertexArray = VertexArray::Create();
		s_Data.TextVertexBuffer.reset(VertexBuffer::Create(s_Data.MaxVertices * sizeof(TextVertex)));

		s_Data.TextVertexBuffer->SetLayout({
			{Gart::ShaderDataType::Float3, "a_Position"},
			{Gart::ShaderDataType::Float4, "a_Color"},
			{Gart::ShaderDataType::Float2, "a_Texture"},
			{Gart::ShaderDataType::Int, "a_EntityID"}
			});

		s_Data.TextVertexArray->AddVertexBuffer(s_Data.TextVertexBuffer);
		s_Data.TextVertexArray->SetIndexBuffer(m_IndexBuffer);
		s_Data.TextVertexBufferBase = new TextVertex[s_Data.MaxVertices];


		int32_t samplers[s_Data.MaxTexureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTexureSlots; i++)
			samplers[i] = i;
		s_Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		
		s_Data.CircleShader = Shader::Create("assets/shaders/Circle2D.glsl");

		s_Data.LineShader = Shader::Create("assets/shaders/Line.glsl");

		s_Data.TextShader = Shader::Create("assets/shaders/Text.glsl");

		s_Data.WhiteTexture = Texture2D::Create(TextureSpecifications());
		uint32_t whitetexturedata = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whitetexturedata, sizeof(uint32_t));

		
		s_Data.TextureSlots[0] = s_Data.WhiteTexture;

		s_Data.QuadVertexPosition[0] = { -0.5f,-0.5f,0.0f,1.0f };
		s_Data.QuadVertexPosition[1] = {  0.5f,-0.5f,0.0f,1.0f };
		s_Data.QuadVertexPosition[2] = {  0.5f, 0.5f,0.0f,1.0f };
		s_Data.QuadVertexPosition[3] = { -0.5f, 0.5f,0.0f,1.0f };

		std::cout << "CameraData size = " << sizeof(Renderer2DStorage::CameraData) << std::endl;
		
		s_Data.CameraUniformBuffer = UniformBuffer::Create(sizeof(Renderer2DStorage::CameraData), 0);
	}
	void Renderer2D::Shutdown()
	{
		GART_PROFILE_FUNCTION();
		delete[] s_Data.QuadVertexBufferBase;
	}

	void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform)
	{
		GART_PROFILE_FUNCTION();

		s_Data.CameraBuffer.ViewProjection = camera.GetProjection() * glm::inverse(transform);
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DStorage::CameraData));

		StartBach();
	}

	void Renderer2D::BeginScene(const EditorCamera& camera)
	{

		GART_PROFILE_FUNCTION();

		s_Data.CameraBuffer.ViewProjection = camera.GetViewProjection();
		s_Data.CameraUniformBuffer->SetData(&s_Data.CameraBuffer, sizeof(Renderer2DStorage::CameraData));

		StartBach();
	}

	void Renderer2D::BeginScene(const OrthoGraphicCamera& camera)
	{
		GART_PROFILE_FUNCTION();

		

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());

		StartBach();

	}
	void Renderer2D::EndScene()
	{
		GART_PROFILE_FUNCTION();

		Flush();
	}

	void Renderer2D::Flush()
	{ 
		if (s_Data.QuadindexCount)
		{
			uint32_t datasize = (uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase;
			s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, datasize);

			for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			{
				s_Data.TextureSlots[i]->Bind(i);
			}

			s_Data.TextureShader->Bind();
			RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadindexCount);
			s_Data.Stats.DrawCalls++;
		}

		if (s_Data.CircleindexCount)
		{
			uint32_t datasize = (uint8_t*)s_Data.CircleVertexBufferPtr - (uint8_t*)s_Data.CircleVertexBufferBase;
			s_Data.CircleVertexBuffer->SetData(s_Data.CircleVertexBufferBase, datasize);

			s_Data.CircleShader->Bind();
			RenderCommand::DrawIndexed(s_Data.CircleVertexArray, s_Data.CircleindexCount);
			s_Data.Stats.DrawCalls++;
		}

		if (s_Data.LineindexCount)
		{
			uint32_t datasize = (uint8_t*)s_Data.LineVertexBufferPtr - (uint8_t*)s_Data.LineVertexBufferBase;
			s_Data.LineVertexBuffer->SetData(s_Data.LineVertexBufferBase, datasize);

			s_Data.LineShader->Bind();
			RenderCommand::SetLineThickness(s_Data.LineWidth);
			RenderCommand::DrawLine(s_Data.LineVertexArray, s_Data.LineindexCount);
			s_Data.Stats.DrawCalls++;
		}

		if (s_Data.TextindexCount)
		{
			uint32_t datasize = (uint32_t)((uint8_t*)s_Data.TextVertexBufferPtr - (uint8_t*)s_Data.TextVertexBufferBase);
			s_Data.TextVertexBuffer->SetData(s_Data.TextVertexBufferBase, datasize);

			auto buf = s_Data.TextVertexBufferBase;
			s_Data.FontAtlasTexture->Bind(0);

			s_Data.TextShader->Bind();
			RenderCommand::DrawIndexed(s_Data.TextVertexArray, s_Data.TextindexCount);
			s_Data.Stats.DrawCalls++;
		}
	}
	void Renderer2D::DrawSprite(const glm::mat4& transform, SpriteRenderer& str, int entityID)
	{
		if (str.Texture) DrawQuad(transform, str.Texture, str.TillingFactor, str.Color, entityID);
		else DrawQuad(transform, str.Color, entityID);
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		GART_PROFILE_FUNCTION();

		DrawQuad({ position.x,position.y,0.0f }, size, color);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		GART_PROFILE_FUNCTION();

		if (s_Data.QuadindexCount >= Renderer2DStorage::MaxIndices)
			FlushAndRestart();

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

		s_Data.Stats.QuadCounts++;

	}

	void Renderer2D::DrawQuad(const glm::vec2 & position, const glm::vec2 & size, const Ref<Texture2D> texture , float tilling, glm::vec4 tintcolor)
	{
		GART_PROFILE_FUNCTION();

		DrawQuad({ position.x,position.y,0.0f }, size, texture,tilling,tintcolor);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture , float tilling, glm::vec4 tintcolor)
	{
		GART_PROFILE_FUNCTION();

		if (s_Data.QuadindexCount >= Renderer2DStorage::MaxIndices)
			FlushAndRestart();

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

		s_Data.Stats.QuadCounts++;

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D> subTexture, float tilling, glm::vec4 tintcolor)
	{
		GART_PROFILE_FUNCTION();

		DrawQuad({ position.x,position.y,0.0f }, size, subTexture, tilling, tintcolor);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D> subTexture, float tilling, glm::vec4 tintcolor)
	{
		GART_PROFILE_FUNCTION();

		if (s_Data.QuadindexCount >= Renderer2DStorage::MaxIndices)
			FlushAndRestart();

		constexpr glm::vec4 color = { 1.0f,1.0f,1.0f,1.0f };

		float textureIndex = 0.0f;

		const Ref<Texture2D> texture = subTexture->GetTexture();
		const glm::vec2* texCoords = subTexture->GetTextCoords();

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
		s_Data.QuadVertexBufferPtr->TexCoord = texCoords[0];
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TillingFactor = tilling;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[1];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = texCoords[1];
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TillingFactor = tilling;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[2];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = texCoords[2];
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TillingFactor = tilling;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[3];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = texCoords[3];
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TillingFactor = tilling;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadindexCount += 6;

		s_Data.Stats.QuadCounts++;
	}


	void Renderer2D::DrawRotateQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const glm::vec4& color)
	{
		DrawRotateQuad({ position.x,position.y,0.0f }, rotation, size, color);
	}
	void Renderer2D::DrawRotateQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const glm::vec4& color)
	{
		GART_PROFILE_FUNCTION();

		if (s_Data.QuadindexCount >= Renderer2DStorage::MaxIndices)
			FlushAndRestart();

		float textureIndex = 0.0f;
		float tilling = 1.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation, glm::vec3({ 0.0f,0.0f,1.0f })) *
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

		s_Data.Stats.QuadCounts++;

	}

	void Renderer2D::DrawRotateQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<Texture2D> texture, float tilling, glm::vec4 tintcolor)
	{
		DrawRotateQuad({ position.x,position.y,0.0f }, rotation, size, texture, tilling,tintcolor);
	}
	void Renderer2D::DrawRotateQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<Texture2D> texture, float tilling, glm::vec4 tintcolor)
	{
		GART_PROFILE_FUNCTION();

		if (s_Data.QuadindexCount >= Renderer2DStorage::MaxIndices)
			FlushAndRestart();

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
			glm::rotate(glm::mat4(1.0f), rotation, glm::vec3({ 0.0f,0.0f,1.0f })) *
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

		s_Data.Stats.QuadCounts++;
	}
	
	void Renderer2D::DrawRotateQuad(const glm::vec2& position, float rotation, const glm::vec2& size, const Ref<SubTexture2D> subTexture, float tilling, glm::vec4 tintcolor)
	{
		GART_PROFILE_FUNCTION();

		DrawRotateQuad({ position.x,position.y,0.0 }, rotation, size, subTexture, tilling, tintcolor);
	}
	void Renderer2D::DrawRotateQuad(const glm::vec3& position, float rotation, const glm::vec2& size, const Ref<SubTexture2D> subTexture, float tilling, glm::vec4 tintcolor)
	{
		GART_PROFILE_FUNCTION();

		if (s_Data.QuadindexCount >= Renderer2DStorage::MaxIndices)
			FlushAndRestart();

		constexpr glm::vec4 color = { 1.0f,1.0f,1.0f,1.0f };

		float textureIndex = 0.0f;

		const Ref<Texture2D> texture = subTexture->GetTexture();
		const glm::vec2* texCoords = subTexture->GetTextCoords();


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
			glm::rotate(glm::mat4(1.0f), rotation, glm::vec3({ 0.0f,0.0f,1.0f })) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[0];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = texCoords[0];
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TillingFactor = tilling;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[1];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = texCoords[1];
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TillingFactor = tilling;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[2];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = texCoords[2];
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TillingFactor = tilling;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[3];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = texCoords[3];
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TillingFactor = tilling;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadindexCount += 6;

		s_Data.Stats.QuadCounts++;
	}


	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		GART_PROFILE_FUNCTION();

		if (s_Data.QuadindexCount >= Renderer2DStorage::MaxIndices)
			FlushAndRestart();

		float textureIndex = 0.0f;
		float tilling = 1.0f;
		

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[0];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f,0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TillingFactor = tilling;
		s_Data.QuadVertexBufferPtr->EntityID = entityID;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[1];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f,0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TillingFactor = tilling;
		s_Data.QuadVertexBufferPtr->EntityID = entityID;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[2];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f,1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TillingFactor = tilling;
		s_Data.QuadVertexBufferPtr->EntityID = entityID;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[3];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f,1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TillingFactor = tilling;
		s_Data.QuadVertexBufferPtr->EntityID = entityID;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadindexCount += 6;

		s_Data.Stats.QuadCounts++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D> texture, float tilling , glm::vec4 tintcolor, int entityID)
	{
		GART_PROFILE_FUNCTION();

		if (s_Data.QuadindexCount >= Renderer2DStorage::MaxIndices)
			FlushAndRestart();

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

		

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[0];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f,0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TillingFactor = tilling;
		s_Data.QuadVertexBufferPtr->EntityID = entityID;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[1];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f,0.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TillingFactor = tilling;
		s_Data.QuadVertexBufferPtr->EntityID = entityID;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[2];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f,1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TillingFactor = tilling;
		s_Data.QuadVertexBufferPtr->EntityID = entityID;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[3];
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f,1.0f };
		s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_Data.QuadVertexBufferPtr->TillingFactor = tilling;
		s_Data.QuadVertexBufferPtr->EntityID = entityID;
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadindexCount += 6;

		s_Data.Stats.QuadCounts++;
	}

	void Renderer2D::DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness , float fade , int entityID )
	{
		for (size_t i = 0; i < 4; i++)
		{
			s_Data.CircleVertexBufferPtr->WorldPosition = transform * s_Data.QuadVertexPosition[i];
			s_Data.CircleVertexBufferPtr->LocalPosition = s_Data.QuadVertexPosition[i] * 2.0f;
			s_Data.CircleVertexBufferPtr->Color = color;
			s_Data.CircleVertexBufferPtr->Thickness = thickness;
			s_Data.CircleVertexBufferPtr->Fade = fade;
			s_Data.CircleVertexBufferPtr->EntityID = entityID;
			s_Data.CircleVertexBufferPtr++;
		}

		s_Data.CircleindexCount += 6;

		s_Data.Stats.QuadCounts++;
	}


	void Renderer2D::DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, int entityID)
	{
		s_Data.LineVertexBufferPtr->Position = p0;
		s_Data.LineVertexBufferPtr->Color = color;
		s_Data.LineVertexBufferPtr->EntityID = entityID;
		s_Data.LineVertexBufferPtr++;

		s_Data.LineVertexBufferPtr->Position = p1;
		s_Data.LineVertexBufferPtr->Color = color;
		s_Data.LineVertexBufferPtr->EntityID = entityID;
		s_Data.LineVertexBufferPtr++;

		s_Data.LineindexCount += 2;

	}

	void Renderer2D::DrawRect(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{
		glm::vec3 Linevertices[4];

		for (size_t i = 0; i < 4; i++)
			Linevertices[i] = transform * s_Data.QuadVertexPosition[i];
		DrawLine(Linevertices[0], Linevertices[1], color);
		DrawLine(Linevertices[1], Linevertices[2], color);
		DrawLine(Linevertices[2], Linevertices[3], color);
		DrawLine(Linevertices[3], Linevertices[0], color);
	}

	void Renderer2D::DrawString(const std::string& outputString, Ref<Font> font, const glm::mat4& transform, const glm::vec4& color)
	{
		const auto& fontGeometry = font->GetMSDFData()->m_FontGeometry;
		const auto& fontmetric = fontGeometry.getMetrics();
		Ref<Texture2D> fontTex = font->GetFontTexture();

		s_Data.FontAtlasTexture = fontTex;

		double x = 0.0f;
		double fScale = 1 / (fontmetric.ascenderY - fontmetric.descenderY);
		double y = 0.0;
		float lineHeightOffset = 0.0f;
		
		for (int i = 0;i < outputString.size();i++)
		{


			char l_character = outputString[i];

			if (l_character == '\r')
				continue;

			if (l_character == '\n')
			{
				x = 0;
				y -= fScale * fontmetric.lineHeight + lineHeightOffset;
				continue;
			}

			auto glyph = fontGeometry.getGlyph(l_character);
			if (!glyph)
				glyph = fontGeometry.getGlyph('?');
			if (!glyph)
				return;

			if (l_character == '\t')
				glyph = fontGeometry.getGlyph(' ');


			double al, ab, ar, at;
			glyph->getQuadAtlasBounds(al, ab, ar, at);
			glm::vec2 texCoordMin((float)al, (float)ab);
			glm::vec2 texCoordMax((float)ar, (float)at);

			double pl, pb, pr, pt;
			glyph->getQuadPlaneBounds(pl, pb, pr, pt);
			glm::vec2 quadMin((float)pl, (float)pb);
			glm::vec2 quadMax((float)pr, (float)pt);

			quadMin *= fScale;
			quadMax *= fScale;
			quadMin += glm::vec2(x, y);
			quadMax += glm::vec2(x, y);

			float texWidth = 1.0f / fontTex->GetWidth();
			float texHeight = 1.0f / fontTex->GetHeight();


			texCoordMin *= glm::vec2(texWidth, texHeight);
			texCoordMax *= glm::vec2(texWidth, texHeight);


			// Render Text Here

			s_Data.TextVertexBufferPtr->Position = transform * glm::vec4(quadMin, 0.0f, 1.0f);
			s_Data.TextVertexBufferPtr->Color = color;
			s_Data.TextVertexBufferPtr->TexCoord = texCoordMin;
			s_Data.TextVertexBufferPtr->EntityID = 0; // TODO
			s_Data.TextVertexBufferPtr++;

			s_Data.TextVertexBufferPtr->Position = transform * glm::vec4(quadMin.x, quadMax.y, 0.0f, 1.0f);
			s_Data.TextVertexBufferPtr->Color = color;
			s_Data.TextVertexBufferPtr->TexCoord = { texCoordMin.x,texCoordMax.y };
			s_Data.TextVertexBufferPtr->EntityID = 0; // TODO
			s_Data.TextVertexBufferPtr++;

			s_Data.TextVertexBufferPtr->Position = transform * glm::vec4(quadMax, 0.0f, 1.0f);
			s_Data.TextVertexBufferPtr->Color = color;
			s_Data.TextVertexBufferPtr->TexCoord = texCoordMax;
			s_Data.TextVertexBufferPtr->EntityID = 0; // TODO
			s_Data.TextVertexBufferPtr++;

			s_Data.TextVertexBufferPtr->Position = transform * glm::vec4(quadMax.x, quadMin.y, 0.0f, 1.0f);
			s_Data.TextVertexBufferPtr->Color = color;
			s_Data.TextVertexBufferPtr->TexCoord = { texCoordMax.x,texCoordMin.y };
			s_Data.TextVertexBufferPtr->EntityID = 0; // TODO
			s_Data.TextVertexBufferPtr++;

			s_Data.TextindexCount += 6;
			s_Data.Stats.QuadCounts++;

			if (i < outputString.size() - 1)
			{
				double fontAdvance = glyph->getAdvance();
				char l_nextCharacter = outputString[i + 1];
				fontGeometry.getAdvance(fontAdvance, l_character, l_nextCharacter);

				float kerningOffset = 0.0f;
				x += fScale * fontAdvance + kerningOffset;
			}
		}
	}

	float Renderer2D::GetLineWidth()
	{
		return s_Data.LineWidth;
	}

	void Renderer2D::SetLineWidth(float width)
	{
		s_Data.LineWidth = width;
	}
	void Renderer2D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Renderer2D::Statistics));
	}
	
	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_Data.Stats;
	}
	void Renderer2D::FlushAndRestart()
	{
		EndScene();

		s_Data.QuadindexCount = 0;
		s_Data.TextureSlotIndex = 1;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
	}
	void Renderer2D::StartBach()
	{
		s_Data.QuadindexCount = 0;
		s_Data.TextureSlotIndex = 1;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.CircleindexCount = 0;
		s_Data.CircleVertexBufferPtr = s_Data.CircleVertexBufferBase;

		s_Data.LineindexCount = 0;
		s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;

		s_Data.TextindexCount = 0;
		s_Data.TextVertexBufferPtr = s_Data.TextVertexBufferBase;
	}
}