#pragma once

#include "OrthoGraphicCamera.h"
#include "Core/TimeStep.h"
#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"

namespace Gart
{
	class OrthoGraphicCameraController 
	{
	public :
		OrthoGraphicCameraController(float aspectRatio);
		void OnUpdate(TimeStep ts);
		void OnEvent(BSS::Event& e);
		OrthoGraphicCamera& GetCamera() { return m_Camera; }
		const OrthoGraphicCamera& GetCamera() const { return m_Camera; }
	private:
		bool OnMouseScrolled(BSS::MouseScrollEvent& e);
		bool OnWindowResized(BSS::WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthoGraphicCamera m_Camera;
		glm::vec3 m_CameraPosition = { 0.0f,0.0f, 0.0f};

		float m_CameraTranslationSpeed = 1.0f;
	};
}
