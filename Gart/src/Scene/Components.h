#pragma once
#include<glm/glm.hpp>
#include "Renderer/Camera.h"
#include "SceneCamera.h"
#include "Renderer/Texture.h"
#include "Core/UUID.h"
#include <glm/gtc/matrix_transform.hpp>
namespace Gart
{

	struct IDComponent
	{
		UUID m_ID;

		IDComponent() = default;

		IDComponent(const IDComponent&) = default;
	};

	struct TagComponent
	{
		std::string m_Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			:m_Tag(tag) {}
	};

	struct TransformComponent
	{
		glm::vec3 Translate = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f,0.0f,0.0f };

		glm::vec3 Scale = { 1.01f,1.0f,1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translate)
			:Translate(translate) { }

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), Rotation.x, { 1.0f,0.0f,0.0f })
				* glm::rotate(glm::mat4(1.0f), Rotation.y, { 0.0f,1.0f,0.0f })
				* glm::rotate(glm::mat4(1.0f), Rotation.z, { 0.0f,0.0f,1.0f });

			return glm::translate(glm::mat4(1.0f), Translate) *
				rotate * glm::scale(glm::mat4(1.0f), Scale);
		}

	};

	struct SpriteRenderer
	{
		glm::vec4 Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		Ref<Texture2D> Texture;
		float TillingFactor = 1.0f;

		SpriteRenderer() = default;
		SpriteRenderer(const SpriteRenderer&) = default;
		SpriteRenderer(const glm::vec4& color)
			:Color(color){ }
	};

	struct CameraComponent
	{
		SceneCamera camera;
		bool Primary = true; // To do after for the scene.
		bool FixedAspectRatio = false;
		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		
	};

	struct ScriptComponent
	{
		std::string Name;

		ScriptComponent() = default;
		ScriptComponent(const ScriptComponent&) = default;
	};

	class ScriptableEntity;

	struct NativeScriptComponent {

		ScriptableEntity* Instance = nullptr;

		ScriptableEntity* (*InstanciateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		


		template<typename T>
		void Bind()
		{
			InstanciateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) {delete nsc->Instance; nsc->Instance = nullptr; };
		}
		
	};

	struct RigidBody2DComponent
	{
		enum class BodyType {Static = 0, Dynamic, Kinematic};
		
		BodyType Type = BodyType::Static;
		bool FixedRotation = false;

		void* RuntimeBody = nullptr;

		RigidBody2DComponent() = default;
		RigidBody2DComponent(const RigidBody2DComponent&) = default;

	};

	struct CircleRendererComponent
	{
		glm::vec4 Color{ 1,1,1,1 };
		float Radius = 0.5f;
		float Fade = 0.005f;
		float Thickness = 1.0f;

		CircleRendererComponent() = default;
		CircleRendererComponent(const CircleRendererComponent&) = default;
	};

	struct BoxCollider2DComponent
	{
		glm::vec2 Offset = { 0.0f,0.0f };
		glm::vec2 Size = { 0.5f,0.5f };

		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshHold = 0.5f;

		void* RuntimeFixture = nullptr;

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;

	};

	struct CircleCollider2DComponent
	{
		glm::vec2 Offset = { 0.0f,0.0f };
		float Radius = 0.5f;

		float Density = 1.0f;
		float Friction = 0.5f;
		float Restitution = 0.0f;
		float RestitutionThreshHold = 0.5f;

		void* RuntimeFixture = nullptr;

		CircleCollider2DComponent() = default;
		CircleCollider2DComponent(const CircleCollider2DComponent&) = default;

	};

	template<typename... component>
	struct ComponentGroup
	{

	};

	using AllComponent = ComponentGroup<TransformComponent,SpriteRenderer,CameraComponent,CircleRendererComponent
		,CircleCollider2DComponent,BoxCollider2DComponent,RigidBody2DComponent,ScriptComponent>;

}
