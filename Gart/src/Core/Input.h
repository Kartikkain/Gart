#pragma once

#include "Core.h"
#include "KeyCodes.h"
#include "MouseButtonCodes.h"

namespace BSS
{
	class BSS_API Input
	{
	public:

		 inline  static bool IsKeyPressed(Gart::KeyCode keycode) { return s_Instance->IsKeyPressedImpl(keycode); }
		 inline  static bool IsMouseButtonPressed(Gart::MouseCode button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		 inline  static std::pair<float,float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		 inline  static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		 inline  static float GetMouseY() { return s_Instance->GetMouseYImpl(); }
	protected:

		virtual bool IsKeyPressedImpl(Gart::KeyCode keycode) = 0;
		virtual bool IsMouseButtonPressedImpl(Gart::MouseCode button) = 0;
		virtual std::pair<float,float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;

	private:

		static Input* s_Instance;
	};
}
