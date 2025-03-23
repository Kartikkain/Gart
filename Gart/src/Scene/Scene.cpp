#include "bsspch.h"
#include "Scene.h"
#include "Components.h"
#include "Renderer/Renderer2D.h"
#include "Entity.h"
#include<glm/gtc/matrix_transform.hpp>

namespace Gart
{
	
	Scene::Scene()
	{
		
	}
	Scene::~Scene()
	{
	}
	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registery.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.m_Tag = name.empty() ? "Entity" : name;
		return entity;
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