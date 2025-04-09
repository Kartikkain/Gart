#pragma once
#include "Renderer/Camera.h"

namespace Gart
{
	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType
		{
			Prespective = 0,
			Orthographic
		};
		
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;
		void SetOrthographic(float size, float nearclip, float farclip);
		void SetPrespective(float fov, float nearclip, float farclip);
		void SetViewportSize(uint32_t width, uint32_t height);
		void RecalculateProjection();

		float GetPrespectiveFOV() const { return m_PrespectiveFOV; }
		void SetPrespectiveFOV(float fov) { m_PrespectiveFOV = fov; RecalculateProjection(); }


		float GetPrespectiveNearClip() const { return  m_PrespectiveNearClip; }
		void SetPrespectiveNearClip(float nearclip) { m_PrespectiveNearClip = nearclip; RecalculateProjection(); }

		float GetPrespectiveFarClip() const { return  m_PrespectiveFarClip; }
		void SetPrespectiveFarClip(float farclip) { m_PrespectiveFarClip = farclip; RecalculateProjection(); }

		const float GetOrthographicSize() const { return OrthoGraphicSize; }

		float GetOrthographicNearClip() const { return  OrthoGaphicNearClip; }
		void SetOrthographicNearClip(float nearclip) { OrthoGaphicNearClip = nearclip; RecalculateProjection(); }

		float GetOrthographicFarClip() const { return  OrthoGaphicFarClip; }
		void SetOrthographicFarClip(float farclip) { OrthoGaphicFarClip = farclip; RecalculateProjection(); }
		void SetOrthographicSize(float Size) { OrthoGraphicSize = Size; RecalculateProjection(); }

		ProjectionType GetProjectionType() const { return m_projectionType; }
		void SetProjectionType(ProjectionType ptype) { m_projectionType = ptype; }

	

	private:
		ProjectionType m_projectionType = ProjectionType::Orthographic;

		float m_PrespectiveFOV = glm::radians(45.0f);
		float m_PrespectiveNearClip = 0.01f;
		float m_PrespectiveFarClip = 1000.0f;

		float OrthoGraphicSize = 10;
		float OrthoGaphicNearClip = -1.0f;
		float OrthoGaphicFarClip = 1.0f;
		float AspectRatio = 0;

		

	};
}
