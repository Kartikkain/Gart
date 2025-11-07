#include "bsspch.h"
#include "FileSystem.h"
#include "Log.h"

namespace Gart
{
	Buffer FileSystem::ReadFileBinary(const std::filesystem::path& filepath)
	{
		std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

		BSS_CORE_ASSERT(stream, "Failed to open the file");

		std::streampos end = stream.tellg();
		stream.seekg(0, std::ios::beg);
		uint32_t size = end - stream.tellg();


		if (size == 0)
		{
			BSS_CORE_ERROR("File is empty");
			return {};
		}

		Buffer buffer(size);
		stream.read(buffer.As<char>(), size);
		stream.close();

		
		return buffer;
	}
}