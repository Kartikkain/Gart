#include "bsspch.h"
#include "ThumbnailCache.h"
#include "Asset/TextureImpoter.h"

namespace Gart
{
	ThumbnailCache::ThumbnailCache()
	{
		m_CachedFilePath = Project::GetActiveProject()->GetAssetDirectory() / "Thumbnail.cache";
		BSS_CORE_INFO("Cached file path : {0}", m_CachedFilePath);
	}

	Ref<Texture2D> ThumbnailCache::GetorCreateCachedImage(std::filesystem::path& assetpath)
	{

		auto absolutepath = Project::GetActiveProject()->GetAssetDirectory() / assetpath;
		std::filesystem::file_time_type lastWriteTime = std::filesystem::last_write_time(absolutepath);
		uint64_t timestamp = std::chrono::duration_cast<std::chrono::seconds>(lastWriteTime.time_since_epoch()).count();

		if (m_CachedImages.find(assetpath) != m_CachedImages.end())
		{
			if (m_CachedImages.at(assetpath).TimeStep == timestamp)
			{
				return m_CachedImages.at(assetpath).Image;
			}
		}

		if (assetpath.extension() != ".png")
			return nullptr;

		Ref<Texture2D> m_tex = TextureImpoter::LoadTexture2D(absolutepath);

		if (!m_tex)
			return nullptr;

		BSS_CORE_INFO("Thumabnail Craeted");

		auto& l_cachedImage = m_CachedImages[assetpath];
		l_cachedImage.TimeStep = timestamp;
		l_cachedImage.Image = m_tex;
		return l_cachedImage.Image;
	}
}