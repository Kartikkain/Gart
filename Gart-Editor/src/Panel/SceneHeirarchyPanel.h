#pragma once
#include "Scene/Scene.h"
#include "Core/Core.h"
namespace Gart
{
	class SceneHeirarchyPanel
	{
	public:

		SceneHeirarchyPanel() = default;
		SceneHeirarchyPanel(Ref<Scene> context);
		void SetContext(Ref<Scene> context);

		void OnGUIRender();

	private:
		Ref<Scene> m_Context;
		
	};
}