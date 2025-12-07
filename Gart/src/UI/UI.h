#pragma once
#include <imgui.h>

namespace Gart::UI
{
	struct ScopeStyleColor
	{
		ScopeStyleColor(ImGuiCol idx, ImVec4 color, bool predicate = true)
			:m_Set(predicate)
		{
			if(predicate)
				ImGui::PushStyleColor(idx,color);
		}

		ScopeStyleColor(ImGuiCol idx, ImU32 color, bool predicate = true)
			:m_Set(predicate)
		{
			if(predicate)
				ImGui::PushStyleColor(idx, color);
		}

		~ScopeStyleColor()
		{
			if (m_Set)
				ImGui::PopStyleColor();
		}

	private :

		bool m_Set = true;
	};
}