#pragma once

#include <filesystem>
#include <map>
#include "Renderer/Texture.h"

namespace Gart
{
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void OnimGuiRender();

	private:

		void RefreshAssetTree();

	private:
		std::filesystem::path m_BaseDirectory;
		std::filesystem::path m_currentDirectory;
		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;
		std::unordered_map<std::string, Ref<Texture2D>> m_Thumbnails;
		std::filesystem::path m_RenameTarget;
		char m_RenameBuffer[256] = {};
		bool m_Renaming = false;

		enum class Mode
		{
			Asset = 0, FileSystem
		};

		struct TreeNode
		{
			std::filesystem::path m_Path;
			AssetHandle m_handle;
			uint32_t ParentNode = (uint32_t) - 1;
			std::map<std::filesystem::path,uint32_t> ChildrenNode;

			TreeNode(std::filesystem::path path,AssetHandle handle)
				:m_Path(path), m_handle(handle)
			{

			}
		};


		std::vector<TreeNode> m_TreeNodes;

		Mode m_Mode = Mode::Asset;
	};


}