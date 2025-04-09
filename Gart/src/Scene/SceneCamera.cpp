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
		m_projectionType = ProjectionType::Orthographic;
		OrthoGraphicSize = size;
		OrthoGaphicNearClip = nearclip;
		OrthoGaphicFarClip = farclip;

		RecalculateProjection();
	}

	void SceneCamera::SetPrespective(float fov, float nearclip, float farclip)
	{
		m_projectionType = ProjectionType::Prespective;
		m_PrespectiveFOV = fov;
		m_PrespectiveNearClip = nearclip;
		m_PrespectiveFarClip = farclip;

		RecalculateProjection();
	}
	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		AspectRatio = (float)width / (float)height;

		RecalculateProjection();

	}

	void SceneCamera::RecalculateProjection()
	{

		if (m_projectionType == ProjectionType::Orthographic)
		{
			float OrthoGraphicLeft = -OrthoGraphicSize * AspectRatio * 0.5f;
			float OrthoGraphicRight = OrthoGraphicSize * AspectRatio * 0.5f;
			float OrthoGraphicTop = OrthoGraphicSize * 0.5f;
			float OrthoGraphicBottom = -OrthoGraphicSize * 0.5f;

			m_Projection = glm::ortho(OrthoGraphicLeft, OrthoGraphicRight, OrthoGraphicBottom, OrthoGraphicTop, OrthoGaphicNearClip, OrthoGaphicFarClip);
		}
		else if (m_projectionType == ProjectionType::Prespective)
		{
			m_Projection = glm::perspective(m_PrespectiveFOV, AspectRatio, m_PrespectiveNearClip, m_PrespectiveFarClip);
		}


		
	}
	

}