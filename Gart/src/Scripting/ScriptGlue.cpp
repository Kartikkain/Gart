#include "bsspch.h"
#include "Core/Log.h"
#include "ScriptGlue.h"
#include "mono/metadata/object.h"

namespace Gart
{
#define GART_INTERANAL_CALL(Name) mono_add_internal_call("Gart.InternalCalls::" #Name, Name);

	static void NativeVector(glm::vec3* Parameter, glm::vec3* OutResult)
	{
		BSS_CORE_WARN("Value {0},{1},{2}", Parameter->x, Parameter->y, Parameter->z);

		*OutResult = glm::normalize(*Parameter);
	}

	static float NativeVectorFloat(glm::vec3* Parameter)
	{
		return glm::dot(*Parameter, *Parameter);
	}

	void ScriptGlue::RegisterFunction()
	{
		GART_INTERANAL_CALL(NativeVector)
		GART_INTERANAL_CALL(NativeVectorFloat)
	}
}