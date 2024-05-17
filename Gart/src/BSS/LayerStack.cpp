#include "bsspch.h"
#include "LayerStack.h"

namespace BSS
{
	LayerStack::LayerStack()
	{

	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
			delete layer;
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
	}

	void LayerStack::PushOverlay(Layer* Overlay)
	{
		m_Layers.emplace_back(Overlay);
	}

	void LayerStack::PullLayer(Layer* layer)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end())
		{
			m_Layers.erase(it);
			m_LayerInsertIndex--;
		}
	}

	void LayerStack::PullOverlay(Layer* Overlay)
	{
		auto it = std::find(m_Layers.begin(), m_Layers.end(), Overlay);
		if (it != m_Layers.end())
		{
			m_Layers.erase(it);
			
		}
	}
}