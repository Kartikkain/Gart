#pragma once
#include "Core/Layer.h"
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
		virtual void OnEvent(Event& event) override;
		void Begin();
		void End();
		void BlockEvent(bool block) { m_blockEvent = block; }
		uint32_t ImguiActiveWidget();
		void SetDarkThemeColors();
	private:
		float m_Time;
		bool m_blockEvent = true;
	};
}