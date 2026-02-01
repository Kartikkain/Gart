#include "bsspch.h"
#include "Core/Log.h"
#include "EditorAssetManager.h"
#include "Asset/AssetImpoter.h"
#include "Project/Project.h"

#include<fstream>
#include<yaml-cpp/yaml.h>

namespace Gart
{
	static std::map<std::filesystem::path, AssetType> m_AssetExtentionMap =
	{
		{".gart",AssetType::Scene},
		{".png",AssetType::Texture2D},
		{".jpg",AssetType::Texture2D},
		{".jpeg",AssetType::Texture2D}
	};

	static AssetType GetAssetTypeFromExtention(std::filesystem::path& extention)
	{
		if (m_AssetExtentionMap.find(extention) == m_AssetExtentionMap.end())
		{
			BSS_CORE_ASSERT(false, "No Such ExtentionType Register");
			return AssetType::None;
		}

		return m_AssetExtentionMap.at(extention);
	}

	Ref<Asset> EditorAssetManager::GetAsset(AssetHandle handle)
	{
		//check for vailid handle
		if (!IsValidHandle(handle))
			return nullptr;

		Ref<Asset> asset;

		// check if the asset is loaded
		if (IsAssetLoaded(handle))
		{
			asset = m_LoadedAsset.at(handle);
		}
		else
		{
			const AssetMetaData& metaData = GetMetaData(handle);
			asset = AssetImpoter::ImportAsset(handle, metaData);
			// Import Asset

			if (!asset)
			{
				BSS_CORE_ERROR("EditorAssetManager::GetAsset :- asset import failed");
			}

			m_LoadedAsset[handle] = asset;
		}

		// return the asset

		return asset;
	}

	bool EditorAssetManager::IsValidHandle(AssetHandle handle) const
	{
		return m_Registery.find(handle) != m_Registery.end();
	}

	bool EditorAssetManager::IsAssetLoaded(AssetHandle handle) const
	{
		return m_LoadedAsset.find(handle) != m_LoadedAsset.end();
	}

	AssetType EditorAssetManager::GetAssetType(AssetHandle handle) const
	{
		if (!IsValidHandle(handle))
			return AssetType::None;

		return m_Registery.at(handle).Type;
	}

	const AssetMetaData& EditorAssetManager::GetMetaData(AssetHandle handle) const
	{
		static AssetMetaData NullMetaData;
		auto it = m_Registery.find(handle);
		if (it == m_Registery.end())
			return NullMetaData;
		return it->second;
	}

	const std::filesystem::path& EditorAssetManager::GetFilePath(AssetHandle handle) const
	{
		return GetMetaData(handle).filePath;
	}

	void EditorAssetManager::ImportAsset(std::filesystem::path filePath)
	{
		AssetHandle handle;
		AssetMetaData metadata;
		metadata.filePath = filePath;
		metadata.Type = GetAssetTypeFromExtention(filePath.extension());

		Ref<Asset> l_asset = AssetImpoter::ImportAsset(handle, metadata);

		if (l_asset)
		{
			m_LoadedAsset[handle] = l_asset;
			m_Registery[handle] = metadata;

			SeralizeAssetRegistery();
		}
	}

	void EditorAssetManager::SeralizeAssetRegistery()
	{
		auto path = Project::GetActiveProject()->GetAssetRegistery();

		YAML::Emitter out;
		{
			out << YAML::BeginMap;
			out << YAML::Key << "AssetRegistery" << YAML::Value;

			out << YAML::BeginSeq;
			for (auto [handle, metadata] : m_Registery)
			{
				out << YAML::BeginMap;
				out << YAML::Key << "Handle" << YAML::Value << (uint64_t)handle;
				
				std::string filepathStr = metadata.filePath.generic_string();
				out << YAML::Key << "FilePath" << YAML::Value << filepathStr;
				
				out << YAML::Key << "Type" << YAML::Value << AssetTypeToString(metadata.Type);

				out << YAML::EndMap;
			}
			out << YAML::EndSeq;

			out << YAML::EndMap;
		}

		std::ofstream fout(path);

		fout << out.c_str();
	}

	bool EditorAssetManager::DeserializeAssetRegistery()
	{
		auto path = Project::GetAssetRegistery();
		YAML::Node data;
		try
		{
			data = YAML::LoadFile(path.string());
		}
		catch (YAML::ParserException e)
		{
			BSS_CORE_ERROR("Failed To load the project file {0}", path);
			return false;
		}

		auto rootNode = data["AssetRegistery"];
		
		if (!rootNode)
			return false;

		for (const auto& node : rootNode)
		{
			AssetHandle handle = node["Handle"].as<uint64_t>();
			AssetMetaData& metaData = m_Registery[handle];
			metaData.filePath = node["FilePath"].as<std::string>();
			metaData.Type = StringToAssetType(node["Type"].as<std::string>());
		}

		return true;
	}

}