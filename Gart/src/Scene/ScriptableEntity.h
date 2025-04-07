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

	protected:

		virtual void OnCreate() {}
		virtual void OnUpdate(TimeStep ts){}
		virtual void OnDestroy() {}

	private:
		Entity m_entity;
		friend class Scene;
	};
}
