#pragma once

#include<entt.hpp>

namespace Gart
{
	class Scene
	{
		public:
			Scene();
			~Scene();

		private:
			entt::registry m_Registery;
	};
}
