#pragma once

#include "Project/Project.h"
#include "Renderer/Texture.h"

#include <map>

namespace Gart
{
	struct ThumbnailImage
	{
		uint64_t TimeStep;
		Ref<Texture2D> Image;
	};

	class ThumbnailCache
	{
	public:
		ThumbnailCache();
		Ref<Texture2D> GetorCreateCachedImage(std::filesystem::path& assetpath);
	private:

		std::map<std::filesystem::path, ThumbnailImage> m_CachedImages;

		std::filesystem::path m_CachedFilePath;

	};

}
