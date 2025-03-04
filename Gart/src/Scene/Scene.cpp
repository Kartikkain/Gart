#include "bsspch.h"
#include "Scene.h"
#include "Components.h"
#include "Renderer/Renderer2D.h"
#include<glm/gtc/matrix_transform.hpp>

namespace Gart
{
	
	Scene::Scene()
	{
		
	}
	Scene::~Scene()
	{
	}
	entt::entity Scene::CreateEntity()
	{
		return m_Registery.create();
	}
	void Scene::OnUpdate(TimeStep ts)
	{
		auto group = m_Registery.group<TransformComponent>(entt::get<SpriteRenderer>);
		for (auto entity : group)
		{
			auto& [transform,sprite] = group.get<TransformComponent,SpriteRenderer>(entity);
			Renderer2D::DrawQuad(transform.Transform, sprite.Color);
		}
	}
}