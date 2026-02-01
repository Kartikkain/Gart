#include "bsspch.h"
#include "SceneImpoter.h"
#include "Project/Project.h"
#include "Scene/SceneSerialization.h"

namespace Gart
{
	Ref<Scene> SceneImpoter::ImportScene(AssetHandle handle, const AssetMetaData& metaData)
	{
		GART_PROFILE_FUNCTION();
		return LoadScene(Project::GetActiveProject()->GetAssetDirectory() / metaData.filePath);
	}

	Ref<Scene> SceneImpoter::LoadScene(const std::filesystem::path& filepath)
	{
		Ref<Scene> scene = std::make_shared<Scene>();
		SceneSerialization sceneSerializer(scene);

		sceneSerializer.DeSerialize(filepath);

		return scene;
	}

	void SceneImpoter::SaveScene(Ref<Scene> scene, const std::filesystem::path& filepath)
	{
		SceneSerialization serializer(scene);
		serializer.Serialize(Project::GetActiveProject()->GetAssetDirectory() / filepath);
	}

}