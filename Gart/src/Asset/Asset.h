#pragma once

#include "Core/UUID.h"

namespace Gart
{
	using AssetHandle = UUID;

	enum class AssetType:uint16_t
	{
		None = 0,
		Scene,
		Texture2D
	};

	std::string AssetTypeToString(AssetType type);
	AssetType StringToAssetType(std::string_view assetType);

	class Asset
	{
	public:

		

	private:

		AssetHandle Handle = 0;

		virtual AssetType GetType() const = 0;
	};
}
