#pragma once

#include<memory>

#ifdef BSS_PLATFORM_WINDOW
#if HZ_DYNAMIC_LINK
	#ifdef BSS_BUILD_DLL
		#define BSS_API _declspec(dllexport)
	#else
		#define BSS_API _declspec(dllimport)
	#endif // BSS_BUILD_DLL
#else 
	#define BSS_API
#endif
#else 
	#error BSS Only Works On Windows!
#endif // BSS_PLATFORM_WINDOW
#define BIT(x) (1 << x)

#define BSS_EVENT_BIND_FN(fn) std::bind(&fn,this,std::placeholders::_1)


namespace Gart
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;
}