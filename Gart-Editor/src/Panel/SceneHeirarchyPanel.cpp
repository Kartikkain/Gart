#include "bsspch.h"
#include "SceneHeirarchyPanel.h"
#include  "imgui.h"
namespace Gart
{
	SceneHeirarchyPanel::SceneHeirarchyPanel(Ref<Scene> context)
	{

	}

	void SceneHeirarchyPanel::SetContext(Ref<Scene> context)
	{
		m_Context = context;
	}

	void SceneHeirarchyPanel::OnGUIRender()
	{
		ImGui::Begin("Heirarchy");


		ImGui::End();
	}
}