#pragma once
#include "BSS/Core.h"
#include "Events/Event.h"

namespace BSS
{
	class BSS_API Layer
	{
	public:

		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDitach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }

	protected:

		std::string m_DebugName;
	};

}