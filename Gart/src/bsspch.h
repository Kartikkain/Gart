#pragma once
#include<iostream>
#include<memory>
#include<utility>
#include<functional>
#include<algorithm>

#include<string.h>
#include<vector>
#include<array>
#include<sstream>
#include<unordered_map>
#include<unordered_set>

#include "Debug/Instrumentor.h"
#include "Core/Buffer.h"

#define NOMINMAX

#ifdef BSS_PLATFORM_WINDOW
	#include<Windows.h>
#endif // BSS_PLATFORM_WINDOW
