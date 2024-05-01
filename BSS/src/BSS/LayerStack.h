#pragma once
#include "Layer.h"
#include <vector>

namespace BSS
{
	class BSS_API LayerStack
	{
	public:

		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PullLayer(Layer* layer);
		void PullOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }

	private:
		std::vector<Layer*> m_Layers;
		unsigned int m_LayerInsertIndex;

	};
}