#include "bsspch.h"
#include "Entity.h"

namespace Gart 
{
	Entity::Entity(entt::entity handler, Scene* scene)
		:m_EntityHandler(handler),m_Scene(scene)
	{

	}
}