#pragma once
#include <filesystem>

namespace Gart
{
	class AssetHandler
	{
	public:
		static void CreateFolder(std::filesystem::path& folderPath);
		
		static void SetDeleteTarget(std::filesystem::path deletePath) { m_DelteTarget = deletePath; }
		static void DeleteItem();
		static void CopyRenameItemPath(std::filesystem::path folderPath);
		static void RenameItem(std::filesystem::path parentPath);
		static std::filesystem::path& GetRenameTarget() { return m_RenameTarget; }
		static bool IsRenaming() { return m_Renaming; }
		static void SetRenaming(bool renaming) { m_Renaming = renaming; }
		static char* GetRenameBuffer() { return m_RenameBuffer; }
		static size_t GetRenameBufferSize() { return sizeof(m_RenameBuffer); }
		static void SetDeleting(bool deleting) { m_Deleting = deleting; }
		static bool IsDeleting() { return m_Deleting; }

	private:
		static std::filesystem::path m_RenameTarget;
		static std::filesystem::path m_DelteTarget;
		static char m_RenameBuffer[256];
		static bool m_Renaming;
		static bool m_Deleting;
	};

	
}