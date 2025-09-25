#include "bsspch.h"
#include "Core/Log.h"
#include "ScriptGlue.h"
#include "Scripting/ScriptEngine.h"
#include "Core/UUID.h"
#include "mono/metadata/object.h"

namespace Gart
{
#define GART_INTERANAL_CALL(Name) mono_add_internal_call("Gart.InternalCalls::" #Name, Name);

	static void Entity_GetTranslation(UUID id,glm::vec3* OutResult)
	{
		Scene* scene = ScriptEngine::GetContext();
		Entity entity = scene->GetEntityWithUUID(id);
		*OutResult = entity.GetComponent<TransformComponent>().Translate;
	}

	static void Entity_SetTranslation(UUID id,glm::vec3* Parameter)
	{
		Scene* scene = ScriptEngine::GetContext();
		Entity entity = scene->GetEntityWithUUID(id);
		entity.GetComponent<TransformComponent>().Translate = *Parameter;
	}

	void ScriptGlue::RegisterFunction()
	{
		GART_INTERANAL_CALL(Entity_GetTranslation)
		GART_INTERANAL_CALL(Entity_SetTranslation)
	}
}