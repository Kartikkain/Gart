#include "bsspch.h"
#include "BSS/Core.h"
#include "Events/Event.h"

namespace BSS
{
	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(std::string title = "BSS", unsigned int width=1920, unsigned int height=1080)
			:Title(title), Width(width), Height(height){}
	};

	// Interface representing desktop base system
	class BSS_API Window
	{
	public:
		using EventCallBackFn = std::function<void(Event&)>;

		virtual ~Window() {}
		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallBackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}
