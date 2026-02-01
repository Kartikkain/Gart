#pragma once

#include "Project/Project.h"
#include "Asset/AssetManagerBase.h"

namespace Gart
{
	class AssetManager
	{
	public:
		template<typename T>
		static Ref<T> GetAsset(AssetHandle handle)
		{
			Ref<Asset> asset = Project::GetActiveProject()->GetAssetManager()->GetAsset(handle);
			return std::static_pointer_cast<T>(asset);
		}

		static bool IsValidHandle(AssetHandle handle)
		{
			return Project::GetActiveProject()->GetAssetManager()->IsValidHandle(handle);
		}
		static bool IsAssetLoaded(AssetHandle handle)
		{
			return Project::GetActiveProject()->GetAssetManager()->IsAssetLoaded(handle);
		}
		static AssetType GetAssetType(AssetHandle handle)
		{
			return Project::GetActiveProject()->GetAssetManager()->GetAssetType(handle);
		}
	};
}
