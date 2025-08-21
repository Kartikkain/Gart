#include "bsspch.h"
#include "Scene.h"
#include "Components.h"
#include "Renderer/Renderer2D.h"
#include "Entity.h"
#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"
#include<glm/gtc/matrix_transform.hpp>

namespace Gart
{
	static b2BodyType GartBodyTypeToBox2DBodyType(RigidBody2DComponent::BodyType bodytype)
	{
		switch (bodytype)
		{
		case Gart::RigidBody2DComponent::BodyType::Static: return b2_staticBody;
		case Gart::RigidBody2DComponent::BodyType::Dynamic: return b2_dynamicBody;
		case Gart::RigidBody2DComponent::BodyType::Kinematic: return b2_kinematicBody;
		}

		
		return b2_staticBody;
	}
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

	void Scene::OnRuntimeStart()
	{
		m_PhysicsWorld = new b2World({ 0.0f,-9.8f });

		auto view = m_Registery.view<RigidBody2DComponent>();

		for (auto e : view)
		{
			Entity entity = { e,this };
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<RigidBody2DComponent>();

			b2BodyDef bodyDef;
			bodyDef.type = GartBodyTypeToBox2DBodyType(rb2d.Type);
			bodyDef.position.Set(transform.Translate.x, transform.Translate.y);
			bodyDef.angle = transform.Rotation.z;

			b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
			body->SetFixedRotation(rb2d.FixedRotation);
			rb2d.RuntimeBody = body;

			if (entity.HasComponent<BoxCollider2DComponent>())
			{
				auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();

				b2PolygonShape boxShape;
				boxShape.SetAsBox(bc2d.Size.x * transform.Scale.x, bc2d.Size.y * transform.Scale.y);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = bc2d.Density;
				fixtureDef.friction = bc2d.Friction;
				fixtureDef.restitution = bc2d.Restitution;
				fixtureDef.restitutionThreshold = bc2d.RestitutionThreshHold;
				body->CreateFixture(&fixtureDef);
			}
			
		}
	}

	void Scene::OnRuntimeStop()
	{
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
	}
	void Scene::OnUpdateRuntime(TimeStep ts)
	{
		// Scripts
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

		//Physics

		{
			const int32_t velocityIterations = 6;
			const int32_t positionIterations = 2;

			m_PhysicsWorld->Step(ts,velocityIterations, positionIterations);

			auto& view = m_Registery.view<RigidBody2DComponent>();

			for (auto e : view)
			{
				Entity entity = { e,this };
				
				auto& rb2d = entity.GetComponent<RigidBody2DComponent>();
				auto& transform = entity.GetComponent<TransformComponent>();

				b2Body* body = (b2Body*)rb2d.RuntimeBody;

				const auto& position = body->GetPosition();

				transform.Translate.x = position.x;
				transform.Translate.y = position.y;
				transform.Rotation.z = body->GetAngle();

			}

		}

		// Render 2D
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

	template<>
	void Scene::OnComponentAdded<RigidBody2DComponent>(Entity entity, RigidBody2DComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<BoxCollider2DComponent>(Entity entity, BoxCollider2DComponent& component)
	{

	}
}