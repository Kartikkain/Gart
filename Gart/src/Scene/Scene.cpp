#include "bsspch.h"
#include "Scene.h"
#include "Components.h"
#include "Renderer/Renderer2D.h"
#include "Entity.h"
#include "ScriptableEntity.h"
#include "Scripting/ScriptEngine.h"
#include "box2d/b2_world.h"
#include "box2d/b2_body.h"
#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_circle_shape.h"
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
		return CreateEntityWithUUID(UUID(), name);
	}

	Entity Scene::CreateEntityWithUUID(UUID uuid, const std::string& name)
	{
		Entity entity = { m_Registery.create(), this };
		entity.AddComponent<IDComponent>(uuid);
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.m_Tag = name.empty() ? "Entity" : name;
		EntityMap[uuid] = entity;
 		return entity;
	}
	
	Entity Scene::GetEntityWithUUID(UUID uuid)
	{
		return EntityMap[uuid];
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registery.destroy(entity);
		EntityMap.erase(entity.GetUUID());
	}

	template<typename... component>
	static void CopyComponent(entt::registry& src, entt::registry& dst, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		([&]()
			{
				auto view = src.view<component>();

				for (auto e : view)
				{
					UUID uuid = src.get<IDComponent>(e).m_ID;

					entt::entity dstEntity = enttMap.at(uuid);

					auto& Component = src.get<component>(e);
					dst.emplace_or_replace<component>(dstEntity, Component);
				}

			}(),...);
		
	}

	template<typename... component>
	static void CopyComponent(ComponentGroup<component...>,entt::registry& src, entt::registry& dst, const std::unordered_map<UUID, entt::entity>& enttMap)
	{
		CopyComponent<component...>(src, dst, enttMap);
	}

	template<typename... component>
	static void CopyComponentIfExist(Entity src, Entity dst)
	{
		([&]()
			{
				if (src.HasComponent<component>())
					dst.AddOrReplaceComponent<component>(src.GetComponent<component>());
			}(),...);
		
	}

	template<typename... component>
	static void CopyComponentIfExist(ComponentGroup<component...>,Entity src, Entity dst)
	{
		CopyComponentIfExist<component...>(src, dst);
	}

	Ref<Scene> Scene::Copy(Ref<Scene> other)
	{
		Ref<Scene> newScene = std::make_shared<Scene>();

		newScene->m_ViewportWidth = other->m_ViewportWidth;
		newScene->m_ViewportHeight = other->m_ViewportHeight;

		std::unordered_map<UUID, entt::entity> enttMap;

		auto& srcSceneRegistery = other->m_Registery;
		auto& dstSceneRegistery = newScene->m_Registery;
		auto view = srcSceneRegistery.view<IDComponent>();

		for (auto e : view)
		{
			UUID uuid = srcSceneRegistery.get<IDComponent>(e).m_ID;
			std::string name = srcSceneRegistery.get<TagComponent>(e).m_Tag;

			Entity l_entity = newScene->CreateEntityWithUUID(uuid, name);
			enttMap[uuid] = (entt::entity)l_entity;
		}

		CopyComponent(AllComponent{}, srcSceneRegistery, dstSceneRegistery, enttMap);

		return newScene;
	}

	void Scene::OnRuntimeStart()
	{
		OnPhysicsStart();
		OnScriptStart();
	}

	void Scene::OnRuntimeStop()
	{
		OnPhysicsStop();
		OnScriptStop();
	}

	void Scene::OnSimulationStart()
	{
		OnPhysicsStart();
	}

	void Scene::OnSimulationStop()
	{
		OnPhysicsStop();
	}

	void Scene::OnPhysicsStart()
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
				boxShape.SetAsBox(bc2d.Size.x * transform.Scale.x, bc2d.Size.y * transform.Scale.y,b2Vec2(bc2d.Offset.x,bc2d.Offset.y),0.0);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = bc2d.Density;
				fixtureDef.friction = bc2d.Friction;
				fixtureDef.restitution = bc2d.Restitution;
				fixtureDef.restitutionThreshold = bc2d.RestitutionThreshHold;
				body->CreateFixture(&fixtureDef);
			}

			if (entity.HasComponent<CircleCollider2DComponent>())
			{
				auto& cc2d = entity.GetComponent<CircleCollider2DComponent>();

				b2CircleShape CircleShape;
				CircleShape.m_p.Set(cc2d.Offset.x, cc2d.Offset.y);
				CircleShape.m_radius = transform.Scale.x * cc2d.Radius;

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &CircleShape;
				fixtureDef.density = cc2d.Density;
				fixtureDef.friction = cc2d.Friction;
				fixtureDef.restitution = cc2d.Restitution;
				fixtureDef.restitutionThreshold = cc2d.RestitutionThreshHold;
				body->CreateFixture(&fixtureDef);
			}

		}
	}

	void Scene::OnPhysicsStop()
	{
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
	}


	void Scene::OnScriptStart()
	{
		ScriptEngine::OnRuntimeStart(this);

		auto view = m_Registery.view<ScriptComponent>();
		for (auto e : view)
		{
			Entity entity = { e,this };
			ScriptEngine::CreateEntity(entity);
		}
	}

	void Scene::OnScriptStop()
	{
		ScriptEngine::OnRuntimeStop();
	}

	void Scene::OnUpdateSimulation(TimeStep ts, const EditorCamera& camera)
	{
		{
			const int32_t velocityIterations = 6;
			const int32_t positionIterations = 2;

			m_PhysicsWorld->Step(ts, velocityIterations, positionIterations);

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
		RenderScene(camera);
	}

	void Scene::OnUpdateRuntime(TimeStep ts)
	{
		// Scripts

		{
			auto  view = m_Registery.view<ScriptComponent>();
			for (auto e : view)
			{
				Entity entity = { e,this };
				ScriptEngine::OnUpdateEntity(entity, ts);
			}

		}

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
				Renderer2D::DrawSprite(transform.GetTransform(), sprite);
			}

			auto view = m_Registery.view<TransformComponent, CircleRendererComponent>();
			for (auto entity : view)
			{
				auto [transform, circle] = view.get<TransformComponent, CircleRendererComponent>(entity);
				Renderer2D::DrawCircle(transform.GetTransform(), circle.Color, circle.Thickness, circle.Fade);
			}
			Renderer2D::EndScene();
		}
	}

	void Scene::OnUpdateEditor(TimeStep ts, const EditorCamera& camera)
	{
		RenderScene(camera);
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

	void Scene::RenderScene(const EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);
		auto group = m_Registery.group<TransformComponent>(entt::get<SpriteRenderer>);
		for (auto entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRenderer>(entity);
			Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
			
		}

		auto view = m_Registery.view<TransformComponent, CircleRendererComponent>();
		for (auto entity : view)
		{
			auto [transform, circle] = view.get<TransformComponent, CircleRendererComponent>(entity);
			Renderer2D::DrawCircle(transform.GetTransform(), circle.Color, circle.Thickness, circle.Fade, (int)entity);
		}

		

		Renderer2D::EndScene();
	}

	void Scene::DuplicateEntity(Entity entity)
	{
		std::string entityName = entity.GetName();
		Entity newEntity = CreateEntity(entityName);

		CopyComponentIfExist(AllComponent{}, entity, newEntity);
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
	void Scene::OnComponentAdded<IDComponent>(Entity entity, IDComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<ScriptComponent>(Entity entity, ScriptComponent& component)
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

	template<>
	void Scene::OnComponentAdded<CircleRendererComponent>(Entity entity, CircleRendererComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<CircleCollider2DComponent>(Entity entity, CircleCollider2DComponent& component)
	{

	}
}