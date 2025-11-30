#include "bsspch.h"
#include "ProjectSerializer.h"

#include <yaml-cpp/yaml.h>

namespace Gart
{
	ProjectSerializer::ProjectSerializer(Ref<Project> project)
		:m_Project(project)
	{
	}

	bool ProjectSerializer::Serialize(const std::filesystem::path& filePath)
	{
		const auto& config = m_Project->GetConfig();

		YAML::Emitter out;
		out << YAML::BeginMap;
		{
			out << YAML::Key << "Project"<<YAML::Value;
			{
				out << YAML::BeginMap;
				out << YAML::Key << "ProjectName" << YAML::Value << config.ProjectName;
				out << YAML::Key << "StartScene" << YAML::Value << config.StartScene.string();
				out << YAML::Key << "AssetDirectory" << YAML::Value << config.AssetDirectory.string();
				out << YAML::Key << "ScriptDirectory" << YAML::Value << config.ScriptDirectory.string();
				out << YAML::EndMap;
			}
		}

		out << YAML::EndMap;

		std::ofstream fout(filePath);
		fout << out.c_str();

		return true;
	}

	bool ProjectSerializer::DeSerialize(const std::filesystem::path& filePath)
	{
		std::ifstream stream(filePath);
		std::stringstream strstream;
		strstream << stream.rdbuf();

		YAML::Node data = YAML::Load(strstream);
		auto& projectData = data["Project"];

		if (!data)
		{
			BSS_CORE_ERROR("No project file found at {0}", filePath);
			return false;
		}

		auto& config = m_Project->GetConfig();

		config.ProjectName = projectData["ProjectName"].as<std::string>();
		config.StartScene = projectData["StartScene"].as<std::string>();
		config.AssetDirectory = projectData["AssetDirectory"].as<std::string>();
		config.ScriptDirectory = projectData["ScriptDirectory"].as<std::string>();

		return true;
	}

}