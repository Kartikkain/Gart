#include "bsspch.h"
#include "ContentBrowserPanel.h"
#include "Project/Project.h"
#include "AssetHandler/AssetHandler.h"
#include "imgui.h"


namespace Gart
{

	ContentBrowserPanel::ContentBrowserPanel()
		:m_BaseDirectory(Project::GetAssetDirectory()), m_currentDirectory(Project::GetAssetDirectory())
	{
		m_DirectoryIcon = Texture2D::Create("Resources/Icons/ContentBrowser/DirectoryIcon.png");
		m_FileIcon = Texture2D::Create("Resources/Icons/ContentBrowser/FileIcon.png");
	}

	void ContentBrowserPanel::OnimGuiRender()
	{
		// Begin Content Browser
		ImGui::Begin("Content Browser");

		if (m_currentDirectory != std::filesystem::path(m_BaseDirectory))
		{	
			// Back Button
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

		if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::MenuItem("Create Folder"))
			{
				AssetHandler::CreateFolder(m_currentDirectory);
			}

			ImGui::EndPopup();
		}


		// Show Folders
		ImGui::Columns(columnCount, 0, false);

		Ref<Texture2D> icon;

		for (auto& directoryEntry : std::filesystem::directory_iterator(m_currentDirectory))
		{
			const auto& path = directoryEntry.path();
			std::string filename = path.filename().string();

			ImGui::PushID(filename.c_str());
			// <-- Show Thumbnails --> // TODO: Cache Thumbnails
			if (path.extension() == ".jpg" || path.extension() == ".png")
			{
				
				if(m_Thumbnails.find(path.string()) == m_Thumbnails.end())
				{
					BSS_CORE_INFO("Image File Found: {0}", path.string());
					Ref<Texture2D> thumbnail = Texture2D::Create(path.string());
					m_Thumbnails[path.string()] = thumbnail;
				}
				
				icon = m_Thumbnails[path.string()];
			}
			else
			{
				icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;
			}
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton((ImTextureID)icon->GetRenderID(), { thumbnailSize,thumbnailSize }, { 0,1 }, { 1,0 });
			// --> End Show Thumbnails <-- //
			if (ImGui::BeginDragDropSource())
			{
				std::filesystem::path relPath(path);
				const wchar_t* itemPath = relPath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t), ImGuiCond_Once);
				ImGui::EndDragDropSource();
			}

			ImGui::PopStyleColor();

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (directoryEntry.is_directory())
				{
					m_currentDirectory /= path.filename();
				}
			}

			

			if (ImGui::BeginPopupContextItem())
			{
				
				if (ImGui::MenuItem("Rename Item"))
				{
					AssetHandler::CopyRenameItemPath(path);
				}

				if (ImGui::MenuItem("Delete Item"))
				{
					AssetHandler::SetDeleteTarget(path);
					AssetHandler::SetDeleting(true);
					
				}

				ImGui::EndPopup();
			}

			if (AssetHandler::GetRenameTarget() == path)
			{
				BSS_CORE_INFO("RenamingStarted: {0}", AssetHandler::GetRenameTarget());
				ImGui::SetNextItemWidth(thumbnailSize);
				if (!AssetHandler::IsRenaming())
				{
					ImGui::SetKeyboardFocusHere();
					AssetHandler::SetRenaming(true);
				}
				size_t buff = AssetHandler::GetRenameBufferSize();
				bool Submitted = ImGui::InputText("##Rename", AssetHandler::GetRenameBuffer(), AssetHandler::GetRenameBufferSize());
				bool IsEntered = ImGui::IsKeyPressed(ImGuiKey_Enter);
				BSS_CORE_INFO("IsEntered: {0}", IsEntered);
				if(IsEntered )
				{
					BSS_CORE_INFO("Enter Pressed");
					AssetHandler::RenameItem(path.parent_path());
				}
			}
			else
			{
				ImGui::TextWrapped(filename.c_str());
				
			}
			
			ImGui::NextColumn();
			ImGui::PopID();
		}

		ImGui::Columns(1);

		//<--------- Delete Confirmation Popup --------->//
			//Always center this window when appearing

		if(AssetHandler::IsDeleting()) ImGui::OpenPopup("Confirm Delete");

		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		if (ImGui::BeginPopupModal("Confirm Delete", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			BSS_CORE_INFO("Delete Popup Opened");
			ImGui::Text("Are you sure you want to delete the file.\nThis action cannot be undone!\n\n");
			ImGui::Separator();

			if (ImGui::Button("Yes", ImVec2(120, 0))) {
				AssetHandler::DeleteItem();
				AssetHandler::SetDeleting(false);
				ImGui::CloseCurrentPopup(); 
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("No", ImVec2(120, 0))) { 
				AssetHandler::SetDeleting(false);
				ImGui::CloseCurrentPopup(); 
			}
			ImGui::EndPopup();
		}
		//<--------- End Delete Confirmation Popup --------->//

		// Show Content brwoser Setting

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