#pragma once
#include<glm/glm.hpp>
#include "Renderer/Camera.h"
#include "SceneCamera.h"
#include "ScriptableEntity.h"
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
		glm::mat4 Transform = glm::mat4(1.0f);

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform)
			:Transform(transform) { }

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

		std::function<void()>  InstanceFunction;
		std::function<void()>  DeleteInstanceFunction;

		std::function<void(ScriptableEntity*)> OnCreateFunction;
		std::function<void(ScriptableEntity* ,TimeStep)> OnUpdateFunction;
		std::function<void(ScriptableEntity*)> OnDestroyFunction;


		template<typename T>
		void Bind()
		{
			InstanceFunction = [&]() {Instance = new T(); };
			DeleteInstanceFunction = [&]() {delete (T*)Instance; };
				
			OnCreateFunction = [](ScriptableEntity* instance) { ((T*)instance)->OnCreate(); };
			OnUpdateFunction = [](ScriptableEntity* instance, TimeStep ts) { ((T*)instance)->OnUpdate(ts); };
			OnDestroyFunction = [](ScriptableEntity* instance) { ((T*)instance)->OnDestroy(); };
		}
		
	};

}
