#pragma once

#include "RenderCommand.h"
#include "Renderer/OrthoGraphicCamera.h"
#include "Renderer/Shader.h"
namespace Gart
{		
	class Renderer
	{
	public :

		static void BeginScene(OrthoGraphicCamera& camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vertexarray, const std::shared_ptr<Shader>& shader);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};
}
