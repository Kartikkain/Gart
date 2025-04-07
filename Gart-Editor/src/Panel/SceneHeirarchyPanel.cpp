#include "bsspch.h"
#include "SceneHeirarchyPanel.h"
#include "Scene/Components.h"
#include  "imgui.h"
namespace Gart
{
	SceneHeirarchyPanel::SceneHeirarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHeirarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
	}

	void SceneHeirarchyPanel::OnGUIRender()
	{
		ImGui::Begin("Heirarchy");
		
		m_Context->m_Registery.each([&](auto entityID)
		{
			Entity entity{ entityID, m_Context.get() };
			auto& tc = entity.GetComponent<TagComponent>();
			ImGui::Text("%s", tc.m_Tag.c_str());
		});
		
		ImGui::End();
	}
}