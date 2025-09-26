#pragma once

namespace Gart
{
	typedef enum class MouseCode : uint16_t
	{
		// From glfw3.h
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2
	} Mouse;

	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		os << static_cast<int32_t>(mouseCode);
		return os;
	}
}

#define BSS_MOUSE_BUTTON_1         ::Gart::Mouse::Button0
#define BSS_MOUSE_BUTTON_2         ::Gart::Mouse::Button1
#define BSS_MOUSE_BUTTON_3         ::Gart::Mouse::Button2
#define BSS_MOUSE_BUTTON_4         ::Gart::Mouse::Button3
#define BSS_MOUSE_BUTTON_5         ::Gart::Mouse::Button4
#define BSS_MOUSE_BUTTON_6         ::Gart::Mouse::Button5
#define BSS_MOUSE_BUTTON_7         ::Gart::Mouse::Button6
#define BSS_MOUSE_BUTTON_8         ::Gart::Mouse::Button7
#define BSS_MOUSE_BUTTON_LAST      ::Gart::Mouse::ButtonLast
#define BSS_MOUSE_BUTTON_LEFT      ::Gart::Mouse::ButtonLeft
#define BSS_MOUSE_BUTTON_RIGHT     ::Gart::Mouse::ButtonRight
#define BSS_MOUSE_BUTTON_MIDDLE    ::Gart::Mouse::ButtonMiddle