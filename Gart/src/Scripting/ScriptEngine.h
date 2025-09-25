#pragma once
#include "Scene/Scene.h"
#include "Scene/Entity.h"
extern "C"
{
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoAssembly MonoAssembly;
}

namespace Gart
{
	

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

	class ScriptInstance
	{
	public:
		ScriptInstance(Ref<ScriptClass> scriptClass, Entity entity);

		void InvokeOnCreate();
		void InvokeOnUpdate(float ts);

	private:
		Ref<ScriptClass> m_scriptClass;

		MonoObject* instance = nullptr;
		MonoMethod* m_Constructor = nullptr;
		MonoMethod* m_Create = nullptr;
		MonoMethod* m_Update = nullptr;
	};


	class ScriptEngine
	{
	public:
		static void Init();
		static void Shutdown();
		
		static void LoadAssembly(const std::filesystem::path& filepath);
		static std::unordered_map<std::string, Ref<ScriptClass>> GetClasses();

		static void OnRuntimeStart(Scene* scene);
		static void OnRuntimeStop();

		static bool ClassExist(std::string& classname);
		static void CreateEntity(Entity entity);
		static void OnUpdateEntity(Entity entity, TimeStep ts);
		static Scene* GetContext();
	private:
		static void InitMono();
		static void ShutDownMono();

		static MonoObject* InstantiateClass(MonoClass* monoClass);
		static void LoadAssemblyClasses(MonoAssembly* assembly);

		friend class ScriptClass;

	};

	
}