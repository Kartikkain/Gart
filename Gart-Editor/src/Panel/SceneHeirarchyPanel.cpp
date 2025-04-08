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
			DrawEntityNode(entity);
		});
		
		ImGui::End();
	}

	void SceneHeirarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().m_Tag;
		ImGuiTreeNodeFlags flags = (m_SelectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool open = ImGui::TreeNodeEx((void*)(uint32_t)entity, flags, tag.c_str());

		if (ImGui::IsItemClicked())
		{
			m_SelectedEntity = entity;
		}


		if (open)
		{
			ImGui::TreePop();
		}
	}
}