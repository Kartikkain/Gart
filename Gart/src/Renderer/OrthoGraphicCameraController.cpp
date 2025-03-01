#include "bsspch.h"
#include "OrthoGraphicCameraController.h"
#include "Core/Input.h"
#include "Core/KeyCodes.h"

namespace Gart 
{
	OrthoGraphicCameraController::OrthoGraphicCameraController(float aspectRatio)
		:m_AspectRatio(aspectRatio), 
		m_Bounds({ -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel }),
		m_Camera(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top)
	{

	}

	void OrthoGraphicCameraController::OnUpdate(TimeStep ts)
	{
		GART_PROFILE_FUNCTION();

		if (BSS::Input::IsKeyPressed(BSS_KEY_A)) { m_CameraPosition.x -= m_CameraTranslationSpeed * ts; }
		else if (BSS::Input::IsKeyPressed(BSS_KEY_D)) { m_CameraPosition.x += m_CameraTranslationSpeed * ts; }
		else if (BSS::Input::IsKeyPressed(BSS_KEY_W)) { m_CameraPosition.y += m_CameraTranslationSpeed * ts; }
		else if (BSS::Input::IsKeyPressed(BSS_KEY_S)) { m_CameraPosition.y -= m_CameraTranslationSpeed * ts; }
		m_Camera.SetPosition(m_CameraPosition);
		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void OrthoGraphicCameraController::OnEvent(BSS::Event& e)
	{
		GART_PROFILE_FUNCTION();

		BSS::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<BSS::MouseScrollEvent>(BSS_EVENT_BIND_FN(OrthoGraphicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<BSS::WindowResizeEvent>(BSS_EVENT_BIND_FN(OrthoGraphicCameraController::OnWindowResized));
	}

	void OrthoGraphicCameraController::OnResize(float width, float height)
	{
		m_AspectRatio = width/height;
		CalculateView();
	}

	bool OrthoGraphicCameraController::OnMouseScrolled(BSS::MouseScrollEvent& e)
	{
		GART_PROFILE_FUNCTION();

		m_ZoomLevel -= e.GetOffsetY() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		CalculateView();
		return false;
	}

	bool OrthoGraphicCameraController::OnWindowResized(BSS::WindowResizeEvent& e)
	{
		OnResize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}

	void OrthoGraphicCameraController::CalculateView()
	{
		m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
	}

}