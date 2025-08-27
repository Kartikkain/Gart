#pragma once

#include<entt.hpp>
#include"Core/TimeStep.h"
#include"Core/UUID.h"
#include "Renderer/EditorCamera.h"

class b2World;
namespace Gart
{
	class Entity;

	class Scene
	{
		public:
			Scene();
			~Scene();

			Entity CreateEntity(const std::string& name = std::string());
			Entity CreateEntityWithUUID(UUID uuid,const std::string& name = std::string());
			void DestroyEntity(Entity entity);
			/*entt::registry& GetReg() { return m_Registery; }*/

			void OnRuntimeStart();
			void OnRuntimeStop();
			void OnUpdateEditor(TimeStep ts,const EditorCamera& camera);
			void OnUpdateRuntime(TimeStep ts);
			void OnViewportResize(uint32_t width, uint32_t height);

			Entity GetPrimaryCamera();
	
		private:
			template<typename T>
			void OnComponentAdded(Entity entity, T& component);
			b2World* m_PhysicsWorld = nullptr;
		private:
			entt::registry m_Registery;
			uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
			friend class Entity;
			friend class SceneHeirarchyPanel;
			friend class SceneSerialization;
	};
}
