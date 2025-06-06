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
	
	void Scene::DestroyEntity(Entity entity)
	{
		m_Registery.destroy(entity);
	}
	void Scene::OnUpdateRuntime(TimeStep ts)
	{
		m_Registery.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
		{

				if (!nsc.Instance)
				{
					nsc.Instance = nsc.InstanciateScript();
					nsc.Instance->m_entity = Entity{ entity,this };
					nsc.Instance->OnCreate();
				}

				nsc.Instance->OnUpdate(ts);

		});

		Camera* maincamera = nullptr;
		glm::mat4* mainCameraTransform = nullptr;
		
		auto l_group = m_Registery.view<TransformComponent, CameraComponent>();
		for (auto entity : l_group)
		{
			auto [transform, camera] = l_group.get<TransformComponent, CameraComponent>(entity);
			
			if (camera.Primary)
			{
				maincamera = &camera.camera;
				mainCameraTransform = &transform.GetTransform();
				break;
			}
			
		}


		if (maincamera )
		{
			Renderer2D::BeginScene(maincamera->GetProjection(), *mainCameraTransform);
			auto group = m_Registery.group<TransformComponent>(entt::get<SpriteRenderer>);
			for (auto entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRenderer>(entity);
				Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
			}
			Renderer2D::EndScene();
		}
	}

	void Scene::OnUpdateEditor(TimeStep ts, const EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);
		auto group = m_Registery.group<TransformComponent>(entt::get<SpriteRenderer>);
		for (auto entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRenderer>(entity);
			Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
		}
		Renderer2D::EndScene();
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

	Entity Scene::GetPrimaryCamera()
	{
		auto view = m_Registery.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (cameraComponent.Primary)
			{
				return Entity{ entity,this };
			}
		}
		return {};
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		if (m_ViewportWidth > 0 && m_ViewportHeight > 0)
			component.camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<SpriteRenderer>(Entity entity, SpriteRenderer& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{

	}
}