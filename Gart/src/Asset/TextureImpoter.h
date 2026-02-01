#pragma once

#include "Renderer/Texture.h"
#include "Asset/Asset.h"
#include "Asset/AssetMetaData.h"

namespace Gart
{
	class TextureImpoter
	{
	public:
		static Ref<Texture2D> ImportTexture2D(AssetHandle handle, const AssetMetaData& metaData);
		static Ref<Texture2D> LoadTexture2D(const std::filesystem::path& filepath);
	};
}