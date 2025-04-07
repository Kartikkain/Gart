#pragma once
#include "Scene/Scene.h"
#include "Core/Core.h"
#include "Scene/Entity.h"
namespace Gart
{
	class SceneHeirarchyPanel
	{
	public:

		SceneHeirarchyPanel() = default;
		SceneHeirarchyPanel(const Ref<Scene>& context);
		void SetContext(const Ref<Scene>& context);

		void OnGUIRender();

	private:
		Ref<Scene> m_Context = nullptr;
	};
}