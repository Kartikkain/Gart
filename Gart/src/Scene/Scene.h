#pragma once

#include<entt.hpp>
#include"Core/TimeStep.h"

namespace Gart
{
	class Entity;

	class Scene
	{
		public:
			Scene();
			~Scene();

			Entity CreateEntity(const std::string& name = std::string());
			/*entt::registry& GetReg() { return m_Registery; }*/
			void OnUpdate(TimeStep ts);
		private:
			entt::registry m_Registery;

			friend class Entity;
	};
}
