#pragma once
#include "Scene/Scene.h"
#include "Asset/Asset.h"
#include "Asset/AssetMetaData.h"

namespace Gart
{
	class SceneImpoter
	{
	public:

		static Ref<Scene> ImportScene(AssetHandle handle, const AssetMetaData& metaData);
		static Ref<Scene> LoadScene(const std::filesystem::path& filepath);
		static void SaveScene(Ref<Scene> scene, const std::filesystem::path& filepath);
	};
}
