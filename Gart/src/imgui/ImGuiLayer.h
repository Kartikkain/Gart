#pragma once
#include "BSS/Layer.h"
#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"

namespace BSS {
	class BSS_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDitach() override;
		virtual void OnImGuiRender() override;
		void Begin();
		void End();
	private:
		float m_Time;
	};
}