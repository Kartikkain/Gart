#pragma once

#include<entt.hpp>
#include"Core/TimeStep.h"

namespace Gart
{
	class Scene
	{
		public:
			Scene();
			~Scene();

			entt::entity CreateEntity();
			entt::registry& GetReg() { return m_Registery; }
			void OnUpdate(TimeStep ts);
		private:
			entt::registry m_Registery;
	};
}
