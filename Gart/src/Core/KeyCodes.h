#pragma once

namespace Gart
{
	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}
}

#define BSS_KEY_SPACE              ::Gart::Key::Space
#define BSS_KEY_APOSTROPHE         ::Gart::Key::Apostrophe
#define BSS_KEY_COMMA              ::Gart::Key::Comma
#define BSS_KEY_MINUS              ::Gart::Key::Minus/* - */
#define BSS_KEY_PERIOD             ::Gart::Key::Period  /* . */
#define BSS_KEY_SLASH              ::Gart::Key::Slash  /* / */
#define BSS_KEY_0                  ::Gart::Key::D0
#define BSS_KEY_1                  ::Gart::Key::D1
#define BSS_KEY_2                  ::Gart::Key::D2
#define BSS_KEY_3                  ::Gart::Key::D3
#define BSS_KEY_4                  ::Gart::Key::D4
#define BSS_KEY_5                  ::Gart::Key::D5
#define BSS_KEY_6                  ::Gart::Key::D6
#define BSS_KEY_7                  ::Gart::Key::D7
#define BSS_KEY_8                  ::Gart::Key::D8
#define BSS_KEY_9                  ::Gart::Key::D9
#define BSS_KEY_SEMICOLON          ::Gart::Key::Semicolon
#define BSS_KEY_EQUAL              ::Gart::Key::Equal  /* = */
#define BSS_KEY_A                  ::Gart::Key::A
#define BSS_KEY_B                  ::Gart::Key::B
#define BSS_KEY_C                  ::Gart::Key::C
#define BSS_KEY_D                  ::Gart::Key::D
#define BSS_KEY_E                  ::Gart::Key::E
#define BSS_KEY_F                  ::Gart::Key::F
#define BSS_KEY_G                  ::Gart::Key::G
#define BSS_KEY_H                  ::Gart::Key::H
#define BSS_KEY_I                  ::Gart::Key::I
#define BSS_KEY_J                  ::Gart::Key::J
#define BSS_KEY_K                  ::Gart::Key::K
#define BSS_KEY_L                  ::Gart::Key::L
#define BSS_KEY_M                  ::Gart::Key::M
#define BSS_KEY_N                  ::Gart::Key::N
#define BSS_KEY_O                  ::Gart::Key::O
#define BSS_KEY_P                  ::Gart::Key::P
#define BSS_KEY_Q                  ::Gart::Key::Q
#define BSS_KEY_R                  ::Gart::Key::R
#define BSS_KEY_S                  ::Gart::Key::S
#define BSS_KEY_T                  ::Gart::Key::T
#define BSS_KEY_U                  ::Gart::Key::U
#define BSS_KEY_V                  ::Gart::Key::V
#define BSS_KEY_W                  ::Gart::Key::W
#define BSS_KEY_X                  ::Gart::Key::X
#define BSS_KEY_Y                  ::Gart::Key::Y
#define BSS_KEY_Z                  ::Gart::Key::Z
#define BSS_KEY_LEFT_BRACKET       ::Gart::Key::LeftBracket  /* [ */
#define BSS_KEY_BACKSLASH          ::Gart::Key::Backslash  /* \ */
#define BSS_KEY_RIGHT_BRACKET      ::Gart::Key::RightBracket /* ] */
#define BSS_KEY_GRAVE_ACCENT       ::Gart::Key::GraveAccent  /* ` */
#define BSS_KEY_WORLD_1            ::Gart::Key::World1 /* non-US #1 */
#define BSS_KEY_WORLD_2            ::Gart::Key::World2 /* non-US #2 */

/* Function keys */
#define BSS_KEY_ESCAPE             ::Gart::Key::Escape
#define BSS_KEY_ENTER              ::Gart::Key::Enter
#define BSS_KEY_TAB                ::Gart::Key::Tab
#define BSS_KEY_BACKSPACE          ::Gart::Key::Backspace
#define BSS_KEY_INSERT             ::Gart::Key::Insert
#define BSS_KEY_DELETE             ::Gart::Key::Delete
#define BSS_KEY_RIGHT              ::Gart::Key::Right
#define BSS_KEY_LEFT               ::Gart::Key::Left
#define BSS_KEY_DOWN               ::Gart::Key::Down
#define BSS_KEY_UP                 ::Gart::Key::Up
#define BSS_KEY_PAGE_UP            ::Gart::Key::PageUp
#define BSS_KEY_PAGE_DOWN          ::Gart::Key::PageDown
#define BSS_KEY_HOME               ::Gart::Key::Home
#define BSS_KEY_END                ::Gart::Key::End
#define BSS_KEY_CAPS_LOCK          ::Gart::Key::CapsLock
#define BSS_KEY_SCROLL_LOCK        ::Gart::Key::ScrollLock
#define BSS_KEY_NUM_LOCK           ::Gart::Key::NumLock
#define BSS_KEY_PRINT_SCREEN       ::Gart::Key::PrintScreen
#define BSS_KEY_PAUSE              ::Gart::Key::Pause
#define BSS_KEY_F1                 ::Gart::Key::F1
#define BSS_KEY_F2                 ::Gart::Key::F2
#define BSS_KEY_F3                 ::Gart::Key::F3
#define BSS_KEY_F4                 ::Gart::Key::F4
#define BSS_KEY_F5                 ::Gart::Key::F5
#define BSS_KEY_F6                 ::Gart::Key::F6
#define BSS_KEY_F7                 ::Gart::Key::F7
#define BSS_KEY_F8                 ::Gart::Key::F8
#define BSS_KEY_F9                 ::Gart::Key::F9
#define BSS_KEY_F10                ::Gart::Key::F10
#define BSS_KEY_F11                ::Gart::Key::F11
#define BSS_KEY_F12                ::Gart::Key::F12
#define BSS_KEY_F13                ::Gart::Key::F13
#define BSS_KEY_F14                ::Gart::Key::F14
#define BSS_KEY_F15                ::Gart::Key::F15
#define BSS_KEY_F16                ::Gart::Key::F16
#define BSS_KEY_F17                ::Gart::Key::F17
#define BSS_KEY_F18                ::Gart::Key::F18
#define BSS_KEY_F19                ::Gart::Key::F19
#define BSS_KEY_F20                ::Gart::Key::F20
#define BSS_KEY_F21                ::Gart::Key::F21
#define BSS_KEY_F22                ::Gart::Key::F22
#define BSS_KEY_F23                ::Gart::Key::F23
#define BSS_KEY_F24                ::Gart::Key::F24
#define BSS_KEY_F25                ::Gart::Key::F25
#define BSS_KEY_KP_0               ::Gart::Key::KP0
#define BSS_KEY_KP_1               ::Gart::Key::KP1
#define BSS_KEY_KP_2               ::Gart::Key::KP2
#define BSS_KEY_KP_3               ::Gart::Key::KP3
#define BSS_KEY_KP_4               ::Gart::Key::KP4
#define BSS_KEY_KP_5               ::Gart::Key::KP5
#define BSS_KEY_KP_6               ::Gart::Key::KP6
#define BSS_KEY_KP_7               ::Gart::Key::KP7
#define BSS_KEY_KP_8               ::Gart::Key::KP8
#define BSS_KEY_KP_9               ::Gart::Key::KP9
#define BSS_KEY_KP_DECIMAL         ::Gart::Key::KPDecimal
#define BSS_KEY_KP_DIVIDE          ::Gart::Key::KPDivide
#define BSS_KEY_KP_MULTIPLY        ::Gart::Key::KPMultiply
#define BSS_KEY_KP_SUBTRACT        ::Gart::Key::KPSubtract
#define BSS_KEY_KP_ADD             ::Gart::Key::KPAdd
#define BSS_KEY_KP_ENTER           ::Gart::Key::KPEnter
#define BSS_KEY_KP_EQUAL           ::Gart::Key::KPEqual
#define BSS_KEY_LEFT_SHIFT         ::Gart::Key::LeftShift
#define BSS_KEY_LEFT_CONTROL       ::Gart::Key::LeftControl
#define BSS_KEY_LEFT_ALT           ::Gart::Key::LeftAlt
#define BSS_KEY_LEFT_SUPER         ::Gart::Key::LeftSuper
#define BSS_KEY_RIGHT_SHIFT        ::Gart::Key::RightShift
#define BSS_KEY_RIGHT_CONTROL      ::Gart::Key::RightControl
#define BSS_KEY_RIGHT_ALT          ::Gart::Key::RightAlt
#define BSS_KEY_RIGHT_SUPER        ::Gart::Key::RightSuper
#define BSS_KEY_MENU               ::Gart::Key::Menu