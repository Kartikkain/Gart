#pragma once
#include "Project.h"

namespace Gart
{
	class ProjectSerializer
	{
	public:

		ProjectSerializer(Ref<Project> project);

		bool Serialize(const std::filesystem::path& filePath);
		bool DeSerialize(const std::filesystem::path& filePath);

	private:
		Ref<Project> m_Project;
	};
}