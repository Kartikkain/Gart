#pragma once

#include "Entity.h"

namespace Gart {

	class ScriptableEntity
	{
	public:
		template<typename T>
		T& GetComponent()
		{
			return m_entity.GetComponent<T>();
		}

	private:
		Entity m_entity;
		friend class Scene;
	};
}
