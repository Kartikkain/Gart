#pragma once

#include "Core/Core.h"
#include "Asset/Asset.h"

#include <map>

namespace Gart
{
	using AssetMap = std::map<AssetHandle, Ref<Asset>>;
	class AssetManagerBase
	{
	public:
		virtual Ref<Asset> GetAsset(AssetHandle handle) = 0;
		virtual bool IsValidHandle(AssetHandle handle) const = 0;
		virtual bool IsAssetLoaded(AssetHandle handle) const = 0;
		virtual AssetType GetAssetType(AssetHandle handle) const = 0;
	};
}
