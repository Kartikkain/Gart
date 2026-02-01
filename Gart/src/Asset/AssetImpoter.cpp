#include "bsspch.h"
#include "AssetImpoter.h"
#include "Asset/TextureImpoter.h"
#include "Asset/SceneImpoter.h"
#include "Core/Log.h"
#include <functional>
#include <map>
namespace Gart
{

	using AssetImpoterFunction = std::function<Ref<Asset>(AssetHandle , const AssetMetaData& )>;
	static std::map<AssetType, AssetImpoterFunction> m_AssetImpoterFunctions = {
		{AssetType::Texture2D, TextureImpoter::ImportTexture2D },
		{AssetType::Scene, SceneImpoter::ImportScene }
	};

	Ref<Asset> AssetImpoter::ImportAsset(AssetHandle handle, const AssetMetaData& metaData)
	{
		if (m_AssetImpoterFunctions.find(metaData.Type) == m_AssetImpoterFunctions.end())
		{
			BSS_CORE_ERROR("No asset impoter for type {}", (uint16_t) metaData.Type);
			return nullptr;
		}
		return m_AssetImpoterFunctions.at(metaData.Type)(handle, metaData);
	}

}