#include "bsspch.h"
#include "TextureImpoter.h"
#include "Project/Project.h"
#include "stb_image.h"
#include "Core/Log.h"
namespace Gart
{
	Ref<Texture2D> TextureImpoter::ImportTexture2D(AssetHandle handle, const AssetMetaData& metaData)
	{
		GART_PROFILE_FUNCTION();

		return LoadTexture2D(Project::GetActiveProject()->GetAssetDirectory() / metaData.filePath);
		
	}
	Ref<Texture2D> TextureImpoter::LoadTexture2D(const std::filesystem::path& filepath)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);

		Buffer data;
		{
			std::string filepathStr = filepath.string();
			data.Data = stbi_load(filepathStr.c_str(), &width, &height, &channels, 0);
		}

		if (data.Data == nullptr)
		{
			//BSS_CORE_ERROR("Could not able to load the file from path : {0}", metaData.filePath.c_str());
			return nullptr;
		}

		TextureSpecifications specs;

		data.Size = width * height * channels;

		specs.m_width = width;
		specs.m_height = height;

		switch (channels)
		{
		case 3: specs.m_imageFormat = ImageFormat::RGB8;
			break;

		case 4: specs.m_imageFormat = ImageFormat::RGBA8;
			break;
		}

		Ref<Texture2D> texture = Texture2D::Create(specs, data);
		data.Release();
		return texture;
	}
}