#pragma once

#include "Buffer.h"

#include <filesystem>

namespace Gart
{
	class FileSystem
	{
	public:
		static Buffer ReadFileBinary(const std::filesystem::path& filepath);
	};
}
