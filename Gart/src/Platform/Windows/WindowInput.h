#pragma once

#include "Core/Input.h"

namespace BSS
{
	class WindowInput :public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(Gart::KeyCode keycode) override;

		virtual bool IsMouseButtonPressedImpl(Gart::MouseCode button) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};
}
