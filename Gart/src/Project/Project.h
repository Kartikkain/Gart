#pragma once
#include "Core/Core.h"
#include "Core/Log.h"

#include<string>
#include<filesystem>

namespace Gart
{
	
	struct ProjectConfig
	{
		std::string ProjectName = "Untitled";

		std::filesystem::path StartScene;

		std::filesystem::path AssetDirectory;
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

		static std::filesystem::path GetAssetFileSystemPath(const std::filesystem::path& filepath)
		{
			BSS_CORE_ASSERT(m_ActiveProject, "No Project Found");
			std::filesystem::path finalPath = GetAssetDirectory() / filepath;
			BSS_CORE_INFO("test : {0}", finalPath.generic_string());
			return finalPath;
		}

		static Ref<Project> GetActiveProject() { return m_ActiveProject; }

		static Ref<Project> New();
		static Ref<Project> Load(const std::filesystem::path& filepath);
		static bool Save(const std::filesystem::path& filePath);
	private:

		static inline Ref<Project> m_ActiveProject;
		std::filesystem::path m_ProjectDirctory;
		ProjectConfig m_Config;

	};
}