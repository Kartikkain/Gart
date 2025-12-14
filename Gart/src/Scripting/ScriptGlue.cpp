#include "bsspch.h"
#include "Core/Log.h"
#include "ScriptGlue.h"
#include "Scripting/ScriptEngine.h"
#include "Physics/Physics2D.h"
#include "Core/UUID.h"
#include "mono/metadata/object.h"
#include "mono/metadata/reflection.h"
#include <box2d/b2_body.h>

namespace Gart
{

#define GART_INTERANAL_CALL(Name) mono_add_internal_call("Gart.InternalCalls::" #Name, Name);

	static std::unordered_map<MonoType*, std::function<bool(Entity)>> m_EntitHasComponentFun;

	static void Transform_GetTranslation(UUID id,glm::vec3* OutResult)
	{
		Scene* scene = ScriptEngine::GetContext();
		Entity entity = scene->GetEntityWithUUID(id);
		*OutResult = entity.GetComponent<TransformComponent>().Translate;
	}

	static void Transform_SetTranslation(UUID id,glm::vec3* Parameter)
	{
		Scene* scene = ScriptEngine::GetContext();
		Entity entity = scene->GetEntityWithUUID(id);
		entity.GetComponent<TransformComponent>().Translate = *Parameter;
	}

	static bool Input_GetKeyDown(KeyCode keycode)
	{
		return BSS::Input::IsKeyPressed(keycode);
	}

	static bool Entity_Has_Component(UUID id, MonoReflectionType* componentType)
	{

		Scene* scene = ScriptEngine::GetContext();
		BSS_CORE_ASSERT(scene, "No Scene Context");

		Entity entity = scene->GetEntityWithUUID(id);

		BSS_CORE_ASSERT(entity, "No entity found");

		MonoType* manageType = mono_reflection_type_get_type(componentType);

		BSS_CORE_ASSERT(manageType, "mono manage type not found.");

		return m_EntitHasComponentFun.at(manageType)(entity);
	}

	static uint64_t Find_Entity_By_Name(MonoString* entityname)
	{
		char* nameCstr = mono_string_to_utf8(entityname);
		std::string str(nameCstr);

		Scene* scene = ScriptEngine::GetContext();
		BSS_CORE_ASSERT(scene, "No Scene Context");

		Entity entity = scene->FindEntityByName(str);
		mono_free(nameCstr);

		if (!entity)
			return 0;

		return entity.GetUUID();

	}

	static MonoObject* GetScriptInstance(UUID entityID)
	{
		return ScriptEngine::GetManagedInstance(entityID);
	}

	static void RigidBody2DComponent_ApplyImpulse(UUID id,glm::vec3* impulse, glm::vec3* worldposition, bool wake)
	{
		Scene* scene = ScriptEngine::GetContext();
		BSS_CORE_ASSERT(scene, "No Scene Context");

		Entity entity = scene->GetEntityWithUUID(id);

		BSS_CORE_ASSERT(entity, "No entity found");

		RigidBody2DComponent rb2d = entity.GetComponent<RigidBody2DComponent>();
		b2Body* body =(b2Body*)rb2d.RuntimeBody;
		body->ApplyLinearImpulse(b2Vec2(impulse->x, impulse->y), b2Vec2(worldposition->x, worldposition->y),wake);

	}

	static void RigidBody2DComponent_ApplyImpulseToCenter(UUID id, glm::vec3* impulse, bool wake)
	{
		Scene* scene = ScriptEngine::GetContext();
		BSS_CORE_ASSERT(scene, "No Scene Context");

		Entity entity = scene->GetEntityWithUUID(id);

		BSS_CORE_ASSERT(entity, "No entity found");

		RigidBody2DComponent rb2d = entity.GetComponent<RigidBody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->ApplyLinearImpulseToCenter(b2Vec2(impulse->x, impulse->y), wake);

	}

	static void RigidBody2DComponent_GetLinearVelocity(UUID id, glm::vec2* OutLinearVelocity)
	{
		Scene* scene = ScriptEngine::GetContext();
		BSS_CORE_ASSERT(scene, "No Scene Context");

		Entity entity = scene->GetEntityWithUUID(id);

		BSS_CORE_ASSERT(entity, "No entity found");

		RigidBody2DComponent rb2d = entity.GetComponent<RigidBody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		const b2Vec2& linearvelocity = body->GetLinearVelocity();

		*OutLinearVelocity = glm::vec2(linearvelocity.x, linearvelocity.y);
	}

	static void RigidBody2DComponent_SetBodyType(UUID id, RigidBody2DComponent::BodyType bodytype)
	{
		Scene* scene = ScriptEngine::GetContext();
		BSS_CORE_ASSERT(scene, "No Scene Context");

		Entity entity = scene->GetEntityWithUUID(id);

		BSS_CORE_ASSERT(entity, "No entity found");

		RigidBody2DComponent rb2d = entity.GetComponent<RigidBody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->SetType(GartBodyTypeToBox2DBodyType(bodytype));
	}

	static Gart::RigidBody2DComponent::BodyType RigidBody2DComponent_GetBodyType(UUID id)
	{
		Scene* scene = ScriptEngine::GetContext();
		BSS_CORE_ASSERT(scene, "No Scene Context");

		Entity entity = scene->GetEntityWithUUID(id);

		BSS_CORE_ASSERT(entity, "No entity found");

		RigidBody2DComponent rb2d = entity.GetComponent<RigidBody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		return GetBox2DToGartBodyType(body->GetType());
	}

	void ScriptGlue::RegisterFunction()
	{
		GART_INTERANAL_CALL(Transform_GetTranslation)
		GART_INTERANAL_CALL(Transform_SetTranslation)
		GART_INTERANAL_CALL(Entity_Has_Component)
		GART_INTERANAL_CALL(Find_Entity_By_Name)
		GART_INTERANAL_CALL(GetScriptInstance)
		GART_INTERANAL_CALL(RigidBody2DComponent_ApplyImpulse)
		GART_INTERANAL_CALL(RigidBody2DComponent_ApplyImpulseToCenter)
		GART_INTERANAL_CALL(RigidBody2DComponent_GetLinearVelocity)
		GART_INTERANAL_CALL(RigidBody2DComponent_GetBodyType)
		GART_INTERANAL_CALL(RigidBody2DComponent_SetBodyType)
		GART_INTERANAL_CALL(Input_GetKeyDown)
	}
	
	template<typename... component>
	static void RegisterComponent()
	{
		([]()
			{
				std::string_view name = typeid(component).name();
				int pos = name.find_last_of(':');
				std::string_view componentName = name.substr(pos + 1);
				std::string manageTypeName = fmt::format("Gart.{}", componentName);
				MonoType* manageType = mono_reflection_type_from_name(manageTypeName.data(), ScriptEngine::GetCoreAssemblyImage());
				if (!manageType)
				{
					BSS_CORE_ERROR("Monotype {0} not found", manageTypeName);
				}
				m_EntitHasComponentFun[manageType] = [](Entity entity) {return entity.HasComponent<component>(); };
			}(), ...);
		
	}

	template<typename... component>
	static void RegisterComponent(ComponentGroup<component...>)
	{
		RegisterComponent<component...>();
	}

	void ScriptGlue::RegisterComponents()
	{
		m_EntitHasComponentFun.clear();
		RegisterComponent(AllComponent{});
	}
}