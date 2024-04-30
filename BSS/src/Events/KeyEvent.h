#pragma once

#include "Event.h"


namespace BSS
{
	class BSS_API KeyEvent :public Event
	{
	public:
		KeyEvent(int keycode)
			:m_KeyCode(keycode){}
		inline int GetKeyCode() const { return m_KeyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard |EventCategoryKeyboard)
	protected:
		int m_KeyCode;
	};

	class BSS_API KeyPressedEvent :public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode,int repeatCount)
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
		
		KeyRelesedEvent(int keycode)
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

		KeyTypedEvent(int keycode)
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