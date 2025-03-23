#pragma once

#include "Scene.h"

#include "entt.hpp"

namespace Gart
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handler, Scene* scene);
		Entity(const Entity& ) = default;
		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registery.has<T>(m_EntityHandler);
		}

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			return m_Scene->m_Registery.emplace<T>(m_EntityHandler, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			return m_Scene->m_Registery.get<T>(m_EntityHandler);
		}

		template<typename T>
		void RemoveComponent()
		{
			m_Scene->m_Registery.remove<T>(m_EntityHandler);
		}

		//operator bool() const { return m_EntityHandler != 0; }

	private:

		entt::entity m_EntityHandler{ 0 };
		Scene* m_Scene = nullptr;

	};
}
