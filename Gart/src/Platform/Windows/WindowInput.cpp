#include "bsspch.h"
#include "WindowInput.h"

#include "Core/Application.h"
#include <GLFW/glfw3.h>

namespace BSS
{
	Input* Input::s_Instance = new WindowInput();

	bool WindowInput::IsKeyPressedImpl(Gart::KeyCode keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window,static_cast<int32_t>(keycode));
		return  state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	bool WindowInput::IsMouseButtonPressedImpl(Gart::MouseCode button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window,static_cast<int32_t>(button));
		return state == GLFW_PRESS;
	}
	std::pair<float, float> WindowInput::GetMousePositionImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		return {(float)xPos,(float)yPos};
	}
	float WindowInput::GetMouseXImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return x;
	}
	float WindowInput::GetMouseYImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return y;
	}
}