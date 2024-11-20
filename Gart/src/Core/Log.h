#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
namespace BSS {

	class BSS_API Log
	{
	public:
		static void init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

	};
}

#define BSS_CORE_ERROR(...) ::BSS::Log::GetCoreLogger()->error(__VA_ARGS__)
#define BSS_CORE_INFO(...)  ::BSS::Log::GetCoreLogger()->info(__VA_ARGS__)
#define BSS_CORE_TRACE(...) ::BSS::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define BSS_CORE_WARN(...)  ::BSS::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define BSS_CORE_FATAL(...) ::BSS::Log::GetCoreLogger()->fatal(__VA_ARGS__)

#define BSS_CLIENT_ERROR(...) ::BSS::Log::GetClientLogger()->error(__VA_ARGS__)
#define BSS_CLIENT_INFO(...)  ::BSS::Log::GetClientLogger()->info(__VA_ARGS__)
#define BSS_CLIENT_TRACE(...) ::BSS::Log::GetClientLogger()->trace(__VA_ARGS__)
#define BSS_CLIENT_WARN(...)  ::BSS::Log::GetClientLogger()->warn(__VA_ARGS__)
#define BSS_CLIENT_FATAL(...) ::BSS::Log::GetClientLogger()->fatal(__VA_ARGS__)

