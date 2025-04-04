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

		Camera* maincamera = nullptr;
		glm::mat4* mainCameraTransform = nullptr;
		
		auto l_group = m_Registery.view<TransformComponent, CameraComponent>();
		for (auto entity : l_group)
		{
			auto& [transform, camera] = l_group.get<TransformComponent, CameraComponent>(entity);
			
			if (camera.Primary)
			{
				maincamera = &camera.camera;
				mainCameraTransform = &transform.Transform;
				break;
			}
			
		}


		if (maincamera )
		{
			Renderer2D::BeginScene(maincamera->GetProjection(), *mainCameraTransform);
			auto group = m_Registery.group<TransformComponent>(entt::get<SpriteRenderer>);
			for (auto entity : group)
			{
				auto& [transform, sprite] = group.get<TransformComponent, SpriteRenderer>(entity);
				Renderer2D::DrawQuad(transform.Transform, sprite.Color);
			}
			Renderer2D::EndScene();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;
		auto view = m_Registery.view<CameraComponent>();

		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			
			if (!cameraComponent.FixedAspectRatio)
			{
				cameraComponent.camera.SetViewportSize(width, height);
			}
		}
	}
}