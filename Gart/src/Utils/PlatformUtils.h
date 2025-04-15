#pragma once
#include <string>
namespace Gart
{
	class FileDialogs
	{
	public :
		static std::string OpenFile(const char* filters);
		static std::string SaveFile(const char* filters);
	};
}
