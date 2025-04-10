#pragma once
#include<glm/glm.hpp>
#include "Renderer/Camera.h"
#include "SceneCamera.h"
#include "ScriptableEntity.h"
#include <glm/gtc/matrix_transform.hpp>
namespace Gart
{

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

}
