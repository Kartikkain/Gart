#include "bsspch.h"
#include "AssetHandler.h"
#include "Core/Log.h"

namespace Gart
{
	std::filesystem::path AssetHandler::m_RenameTarget;
	std::filesystem::path AssetHandler::m_DelteTarget;
	bool AssetHandler::m_Renaming = false;
	char AssetHandler::m_RenameBuffer[256] = {};
	bool AssetHandler::m_Deleting = false;
	void AssetHandler::CreateFolder(std::filesystem::path& folderPath)
	{
		std::filesystem::create_directory(folderPath / "New Folder");
	}

	void AssetHandler::DeleteItem()
	{
		if(m_DelteTarget.empty())
		{
			BSS_CORE_WARN("Delete Target is empty!");
			return;
		}
		BSS_CORE_INFO("Delete Item Path: {0}", m_DelteTarget);
		std::filesystem::remove_all(m_DelteTarget);
		m_DelteTarget.clear();
	}

	void AssetHandler::CopyRenameItemPath(std::filesystem::path folderPath)
	{
		BSS_CORE_INFO("Rename Item Path: {0}", folderPath.filename());
		m_RenameTarget = folderPath;
		std::string filename = folderPath.filename().string();
		memset(m_RenameBuffer, 0, sizeof(m_RenameBuffer));
		strcpy(m_RenameBuffer, filename.c_str());
		BSS_CORE_INFO("Buffer Size : {0}", sizeof(m_RenameBuffer));
	}

	void AssetHandler::RenameItem(std::filesystem::path parentPath)
	{
		
		std::filesystem::path newPath = parentPath / m_RenameBuffer;
		std::filesystem::rename(m_RenameTarget, newPath);
		m_RenameTarget.clear();
		SetRenaming(false);
	}
}