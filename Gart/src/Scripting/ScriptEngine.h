#pragma once

extern "C"
{
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
}

namespace Gart
{
	

	class ScriptEngine
	{
	public:
		static void Init();
		static void Shutdown();
		
		static void LoadAssembly(const std::filesystem::path& filepath);
	private:
		static void InitMono();
		static void ShutDownMono();

		static MonoObject* InstantiateClass(MonoClass* monoClass);

		friend class ScriptClass;

	};

	class ScriptClass
	{
	public:
		ScriptClass() = default;
		ScriptClass(const std::string& nameSpace, const std::string& className);

		MonoObject* Instantiate();
		MonoMethod* GetMethod(const char* name, int parameter);

		MonoObject* InvokeMethod(MonoMethod* method, MonoObject* instance, void** params);

	private:
		
		std::string m_ClassNamespace;
		std::string m_ClassName;
		MonoClass* m_MonoClass = nullptr;
	};
}