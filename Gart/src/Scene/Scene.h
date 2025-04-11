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
			void DestroyEntity(Entity entity);
			/*entt::registry& GetReg() { return m_Registery; }*/
			void OnUpdate(TimeStep ts);
			void OnViewportResize(uint32_t width, uint32_t height);
	
		private:
			template<typename T>
			void OnComponentAdded(Entity entity, T& component);
		private:
			entt::registry m_Registery;
			uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
			friend class Entity;
			friend class SceneHeirarchyPanel;
	};
}
