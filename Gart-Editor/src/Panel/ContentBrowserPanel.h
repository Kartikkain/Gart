#pragma once

#include <filesystem>
#include "Renderer/Texture.h"

namespace Gart
{
	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void OnimGuiRender();
	private:
		std::filesystem::path m_BaseDirectory;
		std::filesystem::path m_currentDirectory;
		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;
		std::unordered_map<std::string, Ref<Texture2D>> m_Thumbnails;
		std::filesystem::path m_RenameTarget;
		char m_RenameBuffer[256] = {};
		bool m_Renaming = false;
	};


}