#pragma once
#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include <map>
extern "C"
{
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoImage MonoImage;
	typedef struct _MonoClassField MonoClassField;
}

namespace Gart
{
	enum ScriptFieldType
	{
		none = 0,
		Int,Float,Char,Bool,Byte,Long,Short,Double,Uint,Ulong,Ushort,Vector2,Vector3,Vector4,GEntity
	};
	
	struct ScriptField
	{
		ScriptFieldType scriptFieldType;
		const char* FieldName;
		MonoClassField* m_fields;
	};

	struct ScriptFieldInstance
	{
		ScriptField field;
		ScriptFieldInstance()
		{
			memset(m_Buffer, 0, sizeof(m_Buffer));
		}

		

		template<typename T>
		T GetValue()
		{
			static_assert(sizeof(T) <= 8, "Type Is Too Large.");
			return *(T*)m_Buffer;
		}

		template<typename T>
		void SetValue(T value)
		{
			static_assert(sizeof(T) <= 8, "Type Is Too Large.");
			memcpy(m_Buffer, &value, sizeof(T));
		}
	private:
		uint8_t m_Buffer[8];

		friend class ScriptEngine;
		friend class ScriptInstance;
		
	};

	using ScriptFieldMap = std::unordered_map<const char*, ScriptFieldInstance>;

	class ScriptClass
	{
	public:
		ScriptClass() = default;
		ScriptClass(const std::string& nameSpace, const std::string& className, bool IsCore = false);

		MonoObject* Instantiate();
		MonoMethod* GetMethod(const char* name, int parameter);

		MonoObject* InvokeMethod(MonoMethod* method, MonoObject* instance, void** params);

		std::map<const char*, ScriptField> GetFields() const { return m_FiledBuffer; }

	private:
		std::map<const char*, ScriptField> m_FiledBuffer;
		std::string m_ClassNamespace;
		std::string m_ClassName;
		MonoClass* m_MonoClass = nullptr;

		friend class ScriptEngine;
	};

	class ScriptInstance
	{
	public:
		ScriptInstance(Ref<ScriptClass> scriptClass, Entity entity);

		void InvokeOnCreate();
		void InvokeOnUpdate(float ts);

		Ref<ScriptClass> GetScriptClass() const { return m_scriptClass; }

		template<typename T>
		T GetFieldValue(const char* name)
		{
			bool success = GetFieldValueInternal(name, s_FieldValueBuffer);

			if (!success)
				return T();
			return *(T*)s_FieldValueBuffer;
		}

		template<typename T>
		void SetFieldValue(const char* name, const T& value)
		{
			SetFieldValueInternal(name, value);
		}

	private:
		Ref<ScriptClass> m_scriptClass;
		const char* s_FieldValueBuffer[8];
		MonoObject* instance = nullptr;
		MonoMethod* m_Constructor = nullptr;
		MonoMethod* m_Create = nullptr;
		MonoMethod* m_Update = nullptr;

	private:

		bool GetFieldValueInternal(const char* name, void* buffer);
		bool SetFieldValueInternal(const char* name, const void* value);

		friend class ScriptEngine;
		friend class ScriptFieldInstance;
	};


	class ScriptEngine
	{
	public:
		static void Init();
		static void Shutdown();
		
		static void LoadAssembly(const std::filesystem::path& filepath);
		static void LoadAppAssembly(const std::filesystem::path& filepath);
		static std::unordered_map<std::string, Ref<ScriptClass>> GetClasses();
		static Ref<ScriptClass> GetClass(std::string& name);
		static void OnRuntimeStart(Scene* scene);
		static void OnRuntimeStop();

		static bool ClassExist(std::string& classname);
		static void CreateEntity(Entity entity);
		static void OnUpdateEntity(Entity entity, TimeStep ts);
		static Scene* GetContext();
		static Ref<ScriptInstance> GetEntityScriptInstance(UUID id);
		static MonoImage* GetCoreAssemblyImage();
		static ScriptFieldMap& GetFieldMap(Entity entity);

	private:
		static void InitMono();
		static void ShutDownMono();

		static MonoObject* InstantiateClass(MonoClass* monoClass);
		static void LoadAssemblyClasses();

		
		friend class ScriptClass;
		friend class ScriptGlue;

	};

	
}