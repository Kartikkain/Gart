#include "bsspch.h"
#include "SceneCamera.h"
#include <glm/gtc/matrix_transform.hpp>
namespace Gart 
{
	
	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}



	void SceneCamera::SetOrthographic(float size, float nearclip, float farclip)
	{
		OrthoGraphicSize = size;
		OrthoGaphicNearClip = nearclip;
		OrthoGaphicFarClip = farclip;

		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		AspectRatio = (float)width / (float)height;

		RecalculateProjection();

	}

	void SceneCamera::RecalculateProjection()
	{
		float OrthoGraphicLeft = -OrthoGraphicSize * AspectRatio * 0.5f;
		float OrthoGraphicRight = OrthoGraphicSize * AspectRatio * 0.5f;
		float OrthoGraphicTop = OrthoGraphicSize * 0.5f;
		float OrthoGraphicBottom = -OrthoGraphicSize * 0.5f;

		m_Projection = glm::ortho(OrthoGraphicLeft, OrthoGraphicRight, OrthoGraphicBottom, OrthoGraphicTop, OrthoGaphicNearClip, OrthoGaphicFarClip);
	}
	

}