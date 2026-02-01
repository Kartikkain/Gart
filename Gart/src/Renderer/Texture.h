#pragma once

#include "Core/Core.h"
#include "Asset/Asset.h"
#include <string>

namespace Gart
{
	enum class ImageFormat
	{
		None = 0,
		R8,
		RGB8,
		RGBA8,
		RGBA32F
	};

	struct TextureSpecifications
	{
		uint32_t m_width = 1;
		uint32_t m_height = 1;
		bool GenerateMinMap = true;
		ImageFormat m_imageFormat = ImageFormat::RGBA8;
	};

	class Texture:public Asset
	{
	public:
		virtual ~Texture() = default;
		virtual const TextureSpecifications& GetTextureSpecs() const = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRenderID() const = 0;
		virtual void SetData(Buffer data) = 0;
		virtual const std::string GetPath() const = 0;

		virtual bool operator== (const Texture& other) const = 0;
		virtual void Bind(uint32_t slot = 0) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const TextureSpecifications& specs, Buffer data = Buffer());
		static AssetType GetStaticType() { return AssetType::Texture2D; }
		virtual AssetType GetType() const { return GetStaticType(); }
	};
}
