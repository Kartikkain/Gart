#pragma once

#include "Event.h"
#include "Core/Input.h"


namespace BSS
{
	class BSS_API KeyEvent :public Event
	{
	public:
		KeyEvent(Gart::KeyCode keycode)
			:m_KeyCode(keycode){}
		inline Gart::KeyCode GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard |EventCategoryKeyboard)
	protected:
		Gart::KeyCode m_KeyCode;
	};

	class BSS_API KeyPressedEvent :public KeyEvent
	{
	public:
		KeyPressedEvent(Gart::KeyCode keycode,int repeatCount)
		:KeyEvent(keycode),m_RepeatCount(repeatCount) {}

		inline int GetRepeatCount() const { return m_RepeatCount; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << m_KeyCode << "( RepeatCount: " << m_RepeatCount << " )";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)

	private:

		int m_RepeatCount;
	};

	class BSS_API KeyRelesedEvent :public KeyEvent
	{
	public :
		
		KeyRelesedEvent(Gart::KeyCode keycode)
			:KeyEvent(keycode){}

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)

	};

	class BSS_API KeyTypedEvent :public KeyEvent
	{
	public:

		KeyTypedEvent(Gart::KeyCode keycode)
			:KeyEvent(keycode) {}

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)

	};
}