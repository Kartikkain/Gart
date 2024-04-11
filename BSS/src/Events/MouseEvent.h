#pragma once
#include "Event.h"


namespace BSS
{
	class BSS_API MouseMoveEvent :public Event
	{
	public :
		MouseMoveEvent(float xPos, int yPos)
			:m_MouseX(xPos),m_MouseY(yPos){}

		inline float GetX() const { return m_MouseX; }
		inline float GetY() const { return m_MouseY; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMoveEvent: " << m_MouseX << "," << m_MouseY;
			return ss.str();
		}

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
		EVENT_CLASS_TYPE(MouseMoved)

	private:
		float m_MouseX, m_MouseY;
	};

	class BSS_API MouseScrollEvent :public Event
	{
	public:
		MouseScrollEvent(float xOffset,float yOffset)
			:m_OffsetX(xOffset),m_OffsetY(yOffset){}

		inline float GetOffsetX() const { return m_OffsetX; }
		inline float GetOffsetY() const { return m_OffsetY; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrollEvent: " << m_OffsetX << "," << m_OffsetY;
			return ss.str();
		}
	private:
		float m_OffsetX, m_OffsetY;
	};

	class BSS_API MouseButtonEvent :public Event
	{
	public :
		inline int GetButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryInput |EventCategoryMouse)

	protected:
		MouseButtonEvent(int button)
			:m_Button(button) {}
		int m_Button;
	};

	class BSS_API MouseButtonPressedEvent :public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button)
			:MouseButtonEvent(button){}

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent" << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class BSS_API MouseButtonReleasedEvent :public MouseButtonEvent
	{
		MouseButtonReleasedEvent(int button)
			:MouseButtonEvent(button) {}

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent" << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}
