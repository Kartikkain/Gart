#pragma once
#include "Event.h"

#include <sstream>
namespace BSS
{
	class BSS_API WindowResizeEvent:public Event
	{
	public:

		WindowResizeEvent(unsigned int width,unsigned int height)
			:m_Width(width),m_Height(height){}

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << "," << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:
		unsigned int m_Width, m_Height;
	};

	class BSS_API WindowCloseEvent :public Event
	{
	public:
		WindowCloseEvent(){}
		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowCloseEvent: ";
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class BSS_API AppTickEvent :public Event
	{
	public:
		AppTickEvent() {}
		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "AppTickEvent: ";
			return ss.str();
		}

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class BSS_API AppUpdateEvent :public Event
	{
	public:
		AppUpdateEvent() {}
		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "AppUpdateEvent: ";
			return ss.str();
		}

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class BSS_API AppRenderEvent :public Event
	{
	public:
		AppRenderEvent() {}
		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "AppRenderEvent: ";
			return ss.str();
		}

		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}