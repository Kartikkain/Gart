#pragma once
#include "Core/Core.h"
#include "Core/Log.h"
#include "Asset/EditorAssetManager.h"
#include "Asset/RuntimeAssetManager.h"

#include<string>
#include<filesystem>

namespace Gart
{
	
	struct ProjectConfig
	{
		std::string ProjectName = "Untitled";

		AssetHandle StartScene;

		std::filesystem::path AssetDirectory;
		std::filesystem::path AssetRegistery;
		std::filesystem::path ScriptDirectory;
	};

	class Project
	{
	public:
		
		ProjectConfig& GetConfig()
		{
			return m_Config;
		}

		static std::filesystem::path GetAssetDirectory()
		{
			BSS_CORE_ASSERT(m_ActiveProject, "No Project Found");
			return GetProjectDirectory() / m_ActiveProject->m_Config.AssetDirectory;
		}

		static std::filesystem::path& GetProjectDirectory()
		{
			BSS_CORE_ASSERT(m_ActiveProject, "No Project Found");
			return m_ActiveProject->m_ProjectDirctory;
		}

		static std::filesystem::path GetAssetRegistery()
		{
			BSS_CORE_ASSERT(m_ActiveProject, "No Project Found");
			return GetAssetDirectory() / m_ActiveProject->m_Config.AssetRegistery;
		}

		static std::filesystem::path GetAssetFileSystemPath(const std::filesystem::path& filepath)
		{
			BSS_CORE_ASSERT(m_ActiveProject, "No Project Found");
			std::filesystem::path finalPath = GetAssetDirectory() / filepath;
			BSS_CORE_INFO("test : {0}", finalPath.generic_string());
			return finalPath;
		}

		static Ref<Project> GetActiveProject() { return m_ActiveProject; }
		Ref<AssetManagerBase> GetAssetManager() { return m_Assetmanager; }
		Ref<EditorAssetManager> GetEditorAssetManager() { return std::static_pointer_cast<EditorAssetManager>(m_Assetmanager); }
		Ref<RuntimeAssetManager> GetRuntimeAssetManager() { return std::static_pointer_cast<RuntimeAssetManager>(m_Assetmanager); }

		static Ref<Project> New();
		static Ref<Project> Load(const std::filesystem::path& filepath);
		static bool Save(const std::filesystem::path& filePath);
	private:

		static inline Ref<Project> m_ActiveProject;
		std::filesystem::path m_ProjectDirctory;
		ProjectConfig m_Config;
		Ref<AssetManagerBase> m_Assetmanager;

	};
}