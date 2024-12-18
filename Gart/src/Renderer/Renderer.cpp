#include "bsspch.h"
#include "Renderer.h"
#include "Platform/OpenGl/OpenGLShader.h"
#include "Renderer2D.h"

namespace Gart
{
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		GART_PROFILE_FUNCTION();

		RenderCommand::Init();
		Renderer2D::init();
	}

	void Renderer::WindowResize(uint32_t Width, uint32_t Height)
	{
		RenderCommand::SetViewPort(0, 0, Width, Height);
	}

	void Renderer::BeginScene(OrthoGraphicCamera& camera)
	{
		GART_PROFILE_FUNCTION();

		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
		GART_PROFILE_FUNCTION();

	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexarray, const std::shared_ptr<Shader>& shader, const glm::mat4& transform)
	{
		GART_PROFILE_FUNCTION();

		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjectionMatrix", m_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);
		vertexarray->Bind();
		RenderCommand::DrawIndexed(vertexarray);
	}

}