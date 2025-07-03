#include "bsspch.h"
#include "ContentBrowserPanel.h"
#include "imgui.h"


namespace Gart
{
	static const std::filesystem::path s_AssetPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
		:m_currentDirectory(s_AssetPath)
	{
		m_DirectoryIcon = Texture2D::Create("Resources/Icons/ContentBrowser/DirectoryIcon.png");
		m_FileIcon = Texture2D::Create("Resources/Icons/ContentBrowser/FileIcon.png");
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

		static float padding = 16.0f;
		static float thumbnailSize = 128.0f;
		float cellSize = padding + thumbnailSize;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		float columnCount = (int)(panelWidth / cellSize);

		if (columnCount < 1) columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_currentDirectory))
		{
			const auto& path = directoryEntry.path();
			auto relPath = std::filesystem::relative(path,s_AssetPath);
			std::string filename = relPath.filename().string();

			Ref<Texture2D> icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton((ImTextureID)icon->GetRenderID(), { thumbnailSize,thumbnailSize }, { 0,1 }, { 1,0 });

			ImGui::PopStyleColor();


			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory())
				{
					m_currentDirectory /= path.filename();
				}
			}
			
			ImGui::TextWrapped(filename.c_str());
			ImGui::NextColumn();
		}

		ImGui::Columns(1);

		ImVec2 windowPos = ImGui::GetWindowPos();
		ImVec2 windowSize = ImGui::GetWindowSize();

		float footerHeight = ImGui::GetFrameHeightWithSpacing() * 2.5f;
		ImGui::Dummy(ImVec2(0.0f, footerHeight));

		ImGui::SetCursorScreenPos(ImVec2(windowPos.x + ImGui::GetStyle().WindowPadding.x,
			windowPos.y + windowSize.y - footerHeight));

		ImGui::Separator();

		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
		ImGui::SliderFloat("Padding", &padding, 0, 32);

		ImGui::End();
	}
}