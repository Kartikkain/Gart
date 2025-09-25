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

			Entity GetEntityWithUUID(UUID uuid);
			void DestroyEntity(Entity entity);
			/*entt::registry& GetReg() { return m_Registery; }*/
			static Ref<Scene> Copy(Ref<Scene> other);
			void OnRuntimeStart();
			void OnRuntimeStop();

			void OnSimulationStart();

			void OnSimulationStop();

			void OnPhysicsStart();

			void OnPhysicsStop();

			void OnScriptStart();

			void OnScriptStop();

			void OnUpdateEditor(TimeStep ts,const EditorCamera& camera);
			void OnUpdateSimulation(TimeStep ts,const EditorCamera& camera);
			void OnUpdateRuntime(TimeStep ts);
			void OnViewportResize(uint32_t width, uint32_t height);
			
			void RenderScene(const EditorCamera& camera);
			void DuplicateEntity(Entity entity);

			template<typename... Components>
			auto GetAllEntityWith()
			{
				return m_Registery.view<Components...>();
			}

			Entity GetPrimaryCamera();
	
		private:
			template<typename T>
			void OnComponentAdded(Entity entity, T& component);
			b2World* m_PhysicsWorld = nullptr;
		private:
			entt::registry m_Registery;
			uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
			std::unordered_map<UUID, Entity> EntityMap;
			friend class Entity;
			friend class SceneHeirarchyPanel;
			friend class SceneSerialization;
	};
}
