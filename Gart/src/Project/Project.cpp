#include "bsspch.h"
#include "Project.h"
#include "ProjectSerializer.h"

namespace Gart
{
	Ref<Project> Project::New()
	{
		m_ActiveProject = std::make_shared<Project>();
		return m_ActiveProject;
	}
	Ref<Project> Project::Load(const std::filesystem::path& filepath)
	{
		Ref<Project> project = std::make_shared<Project>();
		ProjectSerializer Serializer(project);
		if (Serializer.DeSerialize(filepath))
		{
			project->m_ProjectDirctory = filepath.parent_path();
			m_ActiveProject = project;
			return m_ActiveProject;
		}

		return nullptr;
	}
	bool Project::Save(const std::filesystem::path& filePath)
	{
		ProjectSerializer serializer(m_ActiveProject);
		if (serializer.Serialize(filePath))
		{
			m_ActiveProject->m_ProjectDirctory = filePath.parent_path();
			return true;
		}
		return false;
	}
}