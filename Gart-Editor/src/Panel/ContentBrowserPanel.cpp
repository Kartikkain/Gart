#include "bsspch.h"
#include "ContentBrowserPanel.h"
#include  "imgui.h"

namespace Gart
{
	static const std::filesystem::path s_AssetPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
		:m_currentDirectory(s_AssetPath)
	{

	}

	void ContentBrowserPanel::OnimGuiRender()
	{
		ImGui::Begin("Content Browser");

		if (m_currentDirectory != std::filesystem::path(s_AssetPath))
		{
			if (ImGui::Button("<-"))
			{
				m_currentDirectory = m_currentDirectory.parent_path();
			}
		}

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_currentDirectory))
		{
			const auto& path = directoryEntry.path();
			auto relPath = std::filesystem::relative(path, s_AssetPath);
			std::string filename = relPath.filename().string();

			if (directoryEntry.is_directory())
			{
				if (ImGui::Button(filename.c_str()))
				{
					m_currentDirectory /= path.filename();
				}
			}
			else
			{
				if (ImGui::Button(filename.c_str()))
				{
				}
			}

		}

		ImGui::End();
	}
}