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

#ifdef BSS_DEBUG
	#define BSS_ENABLE_ASSERTS
#endif

#ifdef BSS_ENABLE_ASSERTS
	#define BSS_ASSERT(x,...) {if(!(x)){BSS_CLIENT_ERROR("Assertion Failed: {0}",__VA_ARGS__);__debugbreak();}}
	#define BSS_CORE_ASSERT(x,...) {if(!(x)){BSS_CORE_ERROR("Assertion Failed: {0}",__VA_ARGS__);__debugbreak();}}

#else
	#define BSS_ASSERT(x,...)	
	#define BSS_CORE_ASSERT(x,...)
#endif // BSS_ENABLE_ASSERTS


#define BIT(x) (1 << x)

#define BSS_EVENT_BIND_FN(fn) std::bind(&fn,this,std::placeholders::_1)


namespace Gart
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;
}