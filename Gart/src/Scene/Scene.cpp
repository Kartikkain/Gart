#include "bsspch.h"
#include "Scene.h"
#include<glm/gtc/matrix_transform.hpp>

namespace Gart
{
	static void DoMath(glm::mat4& p_transform)
	{

	}
	Scene::Scene()
	{
		struct MeshComponent{};
		struct  TransformComponent
		{
			glm::mat4 Transform;

			TransformComponent() = default;
			TransformComponent(const TransformComponent&) = default;
			TransformComponent(const glm::mat4& transform)
				:Transform(transform){ }

			operator glm::mat4& () { return Transform; }
			operator const glm::mat4& const () { return Transform; }
		};

		TransformComponent l_transformComponent;
		DoMath(l_transformComponent);

		entt::entity l_entity = m_Registery.create();
		auto& l_transform = m_Registery.emplace<TransformComponent>(l_entity, glm::mat4(1.0f));

		if (m_Registery.has<TransformComponent>(l_entity))
		{
			TransformComponent& transform = m_Registery.get<TransformComponent>(l_entity);
		}

		auto view = m_Registery.view<TransformComponent>();
		for (auto entity : view)
		{
			TransformComponent& transform = view.get<TransformComponent>(entity);
		}

		auto group = m_Registery.group<TransformComponent>(entt::get<MeshComponent>);
		for (auto entity : group)
		{
			auto& [transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);
		}


	}
	Scene::~Scene()
	{
	}
}