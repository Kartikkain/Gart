#pragma once

#include "Asset/AssetManagerBase.h"
#include "Asset/AssetMetaData.h"
namespace Gart
{
	using AssetRegistery = std::map<AssetHandle, AssetMetaData>;

	class EditorAssetManager: public AssetManagerBase
	{
	public:

		virtual Ref<Asset> GetAsset(AssetHandle handle) override;
		virtual bool IsValidHandle(AssetHandle handle) const override;
		virtual bool IsAssetLoaded(AssetHandle handle) const override;
		virtual AssetType GetAssetType(AssetHandle handle) const override;
		const AssetMetaData& GetMetaData(AssetHandle handle) const;
		const AssetRegistery& GetAssetRegistery() const { return m_Registery; }
		const std::filesystem::path& GetFilePath(AssetHandle handle) const;
		void ImportAsset(std::filesystem::path filePath);
		void SeralizeAssetRegistery();
		bool DeserializeAssetRegistery();

	private:
		AssetRegistery m_Registery;
		AssetMap m_LoadedAsset;

	};

}
