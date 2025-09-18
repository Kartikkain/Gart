#pragma once

namespace Gart
{
	class ScriptEngine
	{
	public:
		static void Init();
		static void Shutdown();
	
	private:
		static void InitMono();
		static void ShutDownMono();

	};
}