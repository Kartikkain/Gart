#include "bsspch.h"
#include "OrthoGraphicCameraController.h"
#include "BSS/Input.h"
#include "BSS/KeyCodes.h"

namespace Gart 
{
	OrthoGraphicCameraController::OrthoGraphicCameraController(float aspectRatio)
		:m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)
	{

	}

	void OrthoGraphicCameraController::OnUpdate(TimeStep ts)
	{
		if (BSS::Input::IsKeyPressed(BSS_KEY_A)) { m_CameraPosition.x -= m_CameraTranslationSpeed * ts; }
		else if (BSS::Input::IsKeyPressed(BSS_KEY_D)) { m_CameraPosition.x += m_CameraTranslationSpeed * ts; }
		else if (BSS::Input::IsKeyPressed(BSS_KEY_W)) { m_CameraPosition.y += m_CameraTranslationSpeed * ts; }
		else if (BSS::Input::IsKeyPressed(BSS_KEY_S)) { m_CameraPosition.y -= m_CameraTranslationSpeed * ts; }
		m_Camera.SetPosition(m_CameraPosition);
		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void OrthoGraphicCameraController::OnEvent(BSS::Event& e)
	{
		BSS::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<BSS::MouseScrollEvent>(BSS_EVENT_BIND_FN(OrthoGraphicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<BSS::WindowResizeEvent>(BSS_EVENT_BIND_FN(OrthoGraphicCameraController::OnWindowResized));
	}

	bool OrthoGraphicCameraController::OnMouseScrolled(BSS::MouseScrollEvent& e)
	{
		m_ZoomLevel -= e.GetOffsetY() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthoGraphicCameraController::OnWindowResized(BSS::WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

}