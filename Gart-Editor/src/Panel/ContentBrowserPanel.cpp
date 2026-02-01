#include "bsspch.h"
#include "ContentBrowserPanel.h"
#include "Project/Project.h"
#include "AssetHandler/AssetHandler.h"
#include "Asset/TextureImpoter.h"
#include "imgui.h"


namespace Gart
{

	ContentBrowserPanel::ContentBrowserPanel()
		:m_BaseDirectory(Project::GetAssetDirectory()), m_currentDirectory(Project::GetAssetDirectory())
	{
		m_TreeNodes.push_back(TreeNode(".",0));
		m_DirectoryIcon = TextureImpoter::LoadTexture2D("Resources/Icons/ContentBrowser/DirectoryIcon.png");
		m_FileIcon = TextureImpoter::LoadTexture2D("Resources/Icons/ContentBrowser/FileIcon.png");
		
		RefreshAssetTree();

		m_Mode = Mode::Asset;
	}

	void ContentBrowserPanel::OnimGuiRender()
	{
		// Begin Content Browser
		ImGui::Begin("Content Browser");
		const char* label = m_Mode == Mode::FileSystem ? "File" : "Asset";
		if (ImGui::Button(label))
		{
			m_Mode = m_Mode == Mode::FileSystem ? Mode::Asset : Mode::FileSystem;
		}

		if (m_currentDirectory != std::filesystem::path(m_BaseDirectory))
		{	
			ImGui::SameLine();
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

		if (m_Mode == Mode::Asset)
		{
			TreeNode* node = &m_TreeNodes[0];
			std::filesystem::path currDirec = std::filesystem::relative(m_currentDirectory, Project::GetActiveProject()->GetAssetDirectory());

			for (const auto& p : currDirec)
			{
				if (node->m_Path == currDirec)
					break;

				if (node->ChildrenNode.find(p) != node->ChildrenNode.end())
				{
					node = &m_TreeNodes[node->ChildrenNode[p]];
					continue;
				}
				else
				{
					BSS_CORE_ASSERT(false, "No such path");
				}
			}

			for (const auto& [item, treeNodeIndex] : node->ChildrenNode)
			{
				// Show Folders
				ImGui::Columns(columnCount, 0, false);

				bool IsDirectory = std::filesystem::is_directory(Project::GetActiveProject()->GetAssetDirectory() / item);
				std::string itemStr = item.generic_string();

				ImGui::PushID(itemStr.c_str());
				Ref<Texture2D> icon;
				icon = IsDirectory? m_DirectoryIcon : m_FileIcon;
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
				ImGui::ImageButton((ImTextureID)icon->GetRenderID(), { thumbnailSize,thumbnailSize }, { 0,1 }, { 1,0 });

				if (ImGui::BeginDragDropSource())
				{
					
					AssetHandle l_handle = m_TreeNodes[treeNodeIndex].m_handle;
					ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM",&l_handle,sizeof(AssetHandle));
					ImGui::EndDragDropSource();
				}

				ImGui::PopStyleColor();

				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					if (IsDirectory)
					{
						m_currentDirectory /= item.filename();
					}
				}

				ImGui::TextWrapped(itemStr.c_str());
				ImGui::NextColumn();
				ImGui::PopID();
			}
		}
		else
		{


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

				/*if (path.extension() == ".jpg" || path.extension() == ".png")
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
				}*/

				icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
				ImGui::ImageButton((ImTextureID)icon->GetRenderID(), { thumbnailSize,thumbnailSize }, { 0,1 }, { 1,0 });
				// --> End Show Thumbnails <-- //
				

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

					if (ImGui::MenuItem("Import Asset"))
					{
						std::filesystem::path relPath = std::filesystem::relative(path, Project::GetActiveProject()->GetAssetDirectory());
						Project::GetActiveProject()->GetEditorAssetManager()->ImportAsset(relPath);
						RefreshAssetTree();
					}

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
					if (IsEntered)
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

	void ContentBrowserPanel::RefreshAssetTree()
	{
		const auto& registery = Project::GetActiveProject()->GetEditorAssetManager()->GetAssetRegistery();
		
		for (const auto& [handle, metaData] : registery)
		{
			uint32_t currentNodeIndex = 0;
			for (const auto& p : metaData.filePath)
			{
				auto it = m_TreeNodes[currentNodeIndex].ChildrenNode.find(p.generic_string());
				if (it != m_TreeNodes[currentNodeIndex].ChildrenNode.end())
				{
					currentNodeIndex = it->second;
				}
				else
				{
					
					TreeNode node(p,handle);
					node.ParentNode = currentNodeIndex;
					m_TreeNodes.push_back(node);

					m_TreeNodes[currentNodeIndex].ChildrenNode[p] = m_TreeNodes.size() - 1;
					currentNodeIndex = m_TreeNodes.size() - 1;
				}
			}
		}
		BSS_CORE_INFO("Finish");
	}
}