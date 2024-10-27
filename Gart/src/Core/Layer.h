#pragma once
#include "Core/Core.h"
#include "Events/Event.h"
#include "Core/TimeStep.h"

namespace BSS
{
	class BSS_API Layer
	{
	public:

		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDitach() {}
		virtual void OnUpdate(Gart::TimeStep ts) {}
		virtual void OnEvent(Event& event) {}
		virtual void OnImGuiRender() {}
		inline const std::string& GetName() const { return m_DebugName; }

	protected:

		std::string m_DebugName;
	};

}