#pragma once

#include "Core/Core.h"
#include "Asset/Asset.h"
#include "Asset/AssetMetaData.h"


namespace Gart
{
	class AssetImpoter
	{
	public:
		static Ref<Asset> ImportAsset(AssetHandle handle, const AssetMetaData& metaData);
	};
}