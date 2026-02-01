#pragma once
#include "Asset/Asset.h"

#include <filesystem>

namespace Gart
{
	struct AssetMetaData
	{
		AssetType Type = AssetType::None;
		std::filesystem::path filePath;

		operator bool() const { return Type != AssetType::None; }
	};
}