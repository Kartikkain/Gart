#pragma once
#include "BSS/Layer.h"
namespace BSS {
	class BSS_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDitach();
		void OnUpdate();
		void OnEvent(Event& event);

	private:
		float m_Time;
	};
}