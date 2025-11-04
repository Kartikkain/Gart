#include "bsspch.h"
#include "Core/Log.h"
#include "ScriptEngine.h"
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/tabledefs.h"
#include "ScriptGlue.h";
#include "Filewatcher.h"
#include "Core/Application.h"

#include<filesystem>

namespace Gart
{
	static std::unordered_map<std::string, ScriptFieldType> m_FieldTypes =
	{
		{"System.Int32",ScriptFieldType::Int},
		{"System.Single",ScriptFieldType::Float},
		{"System.Boolean",ScriptFieldType::Bool},
		{"System.Char",ScriptFieldType::Char},
		{"System.Int64",ScriptFieldType::Long},
		{"System.Int16",ScriptFieldType::Short},
		{"System.Byte",ScriptFieldType::Byte},
		{"System.Double",ScriptFieldType::Double},
		{"System.UInt32",ScriptFieldType::Uint},
		{"System.UInt64",ScriptFieldType::Ulong},
		{"System.UInt16",ScriptFieldType::Ushort},
		{"Gart.Vector2",ScriptFieldType::Vector2},
		{"Gart.Vector3",ScriptFieldType::Vector3},
		{"Gart.Vector4",ScriptFieldType::Vector4},
		{"Gart.Entity",ScriptFieldType::GEntity},
	};
	namespace Utils
	{
		char* ReadBytes(const std::filesystem::path& filepath, uint32_t* outSize)
		{
			std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

			BSS_CORE_ASSERT(stream, "Failed to open the file");

			std::streampos end = stream.tellg();
			stream.seekg(0, std::ios::beg);
			uint32_t size = end - stream.tellg();


			if (size == 0)
			{
				BSS_CORE_ERROR("File is empty");
				return nullptr;
			}

			char* buffer = new char[size];
			stream.read((char*)buffer, size);
			stream.close();

			*outSize = size;
			return buffer;
		}

		MonoAssembly* LoadMonoAssembly(const std::filesystem::path& assemblyPath)
		{
			uint32_t fileSize = 0;
			char* fileData = ReadBytes(assemblyPath, &fileSize);

			// NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
			MonoImageOpenStatus status;
			MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

			if (status != MONO_IMAGE_OK)
			{
				const char* errorMessage = mono_image_strerror(status);
				// Log some error message using the errorMessage data
				return nullptr;
			}

			std::string assemblypathstr = assemblyPath.string();
			MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblypathstr.c_str(), &status, 0);
			mono_image_close(image);

			// Don't forget to free the file data
			delete[] fileData;

			return assembly;
		}
	
		void PrintAssemblyTypes(MonoAssembly* assembly)
		{
			MonoImage* image = mono_assembly_get_image(assembly);
			const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
			int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

			for (int32_t i = 0; i < numTypes; i++)
			{
				uint32_t cols[MONO_TYPEDEF_SIZE];
				mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

				const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
				const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

				printf("%s.%s\n", nameSpace, name);
			}
		}
		
		ScriptFieldType MonoTypeToGartType(const char* monotype)
		{
			auto it = m_FieldTypes.find(monotype);
			if (it == m_FieldTypes.end()) return ScriptFieldType::none;

			return it->second;
		}

		const char* GartTypeToString(ScriptFieldType type)
		{
			switch (type)
			{
			case Gart::Int: return "int";
			case Gart::Float: return "float";
			case Gart::Char: return "char";
			case Gart::Bool: return "bool";
			case Gart::Byte: return "byte";
			case Gart::Long: return "long";
			case Gart::Short: return "short";
			case Gart::Double: return "double";
			case Gart::Uint: return "uint";
			case Gart::Ulong: return "ulong";
			case Gart::Ushort: return "ushort";
			case Gart::Vector2: return "vector2";
			case Gart::Vector3: return "vector3";
			case Gart::Vector4: return "vector4";
			case Gart::GEntity: return "Entity";
			}

			return "<Inavalid>";
		}

	}

	struct ScriptEngineData
	{
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;

		MonoAssembly* CoreAssembly = nullptr;
		MonoImage* CoreAssemblyImage = nullptr;

		MonoAssembly* AppAssembly = nullptr;
		MonoImage* AppAssemblyImage = nullptr;

		std::filesystem::path CoreAssemblyPath;
		std::filesystem::path AppAssemblyPath;

		Ref<filewatch::FileWatch<std::string>> AppAssemblyFilewatcher;

		bool AppAssemblyLoadPending = false;
		ScriptClass EntityClass;
		std::unordered_map<std::string, Ref<ScriptClass>> EntityClasses;
		std::unordered_map<UUID, Ref<ScriptInstance>> EntityInstances;
		std::unordered_map<UUID, ScriptFieldMap> EntityFieldMap;

		//Runtime

		Scene* SceneContext = nullptr;

	};

	static ScriptEngineData* s_Data = nullptr;


#pragma region Filewatch


	static void OnAppAssemblyFileSystemEvent(const std::string& path, const filewatch::Event change_type)
	{
		if (change_type == filewatch::Event::modified && !s_Data->AppAssemblyLoadPending)
		{
			s_Data->AppAssemblyLoadPending = true;
			
			std::cout << path << "-" << (int)change_type << std::endl;

			BSS::Application::Get().SubmitToMainThreadQueue([]() { 
				s_Data->AppAssemblyFilewatcher.reset();
				ScriptEngine::ReloadAssemblies(); 
				});
		}
	}

#pragma endregion


#pragma region ScriptEngine
	void ScriptEngine::Init()
	{
		s_Data = new ScriptEngineData();


		InitMono();
		ScriptGlue::RegisterFunction();

		LoadAssembly("Resources/Scripts/Gart-ScriptCore.dll");
		LoadAppAssembly("SandboxProject/Assets/Scripts/Binaries/Sandbox.dll");

		ScriptGlue::RegisterComponents();

		LoadAssemblyClasses();
		auto& classes = s_Data->EntityClasses;

		// Create An Object That Call's Constructor

		s_Data->EntityClass = ScriptClass::ScriptClass("Gart", "Entity",true);

		
		MonoObject* Instance = s_Data->EntityClass.Instantiate();
		
#if 0

		// Call Function
		MonoMethod* printFunction = s_Data->EntityClass.GetMethod("PrintMessage", 0);
		s_Data->EntityClass.InvokeMethod(printFunction, Instance, nullptr);

		// Call Function With Parameter

		MonoMethod* printFunctionWithParameter = s_Data->EntityClass.GetMethod("PrintInt", 1);
		int str = 5;
		void* Params = &str;
		s_Data->EntityClass.InvokeMethod(printFunctionWithParameter, Instance, &Params);

		// Call Function With String Parameter

		MonoString* monostring = mono_string_new(s_Data->AppDomain, "C++ Is Reading C# Functions");
		MonoMethod* printFunctionWithStringParameter = s_Data->EntityClass.GetMethod("PrintCustomMessage", 1);
		void* stringParam = monostring;
		s_Data->EntityClass.InvokeMethod(printFunctionWithStringParameter, Instance, &stringParam);
#endif

		
	}


	void ScriptEngine::Shutdown()
	{
		ShutDownMono();
		delete s_Data;
	}

	void ScriptEngine::LoadAssembly(const std::filesystem::path& filepath)
	{
		s_Data->CoreAssemblyPath = filepath;
		s_Data->AppDomain = mono_domain_create_appdomain("GartScriptRuntime", nullptr);
		mono_domain_set(s_Data->AppDomain, true);

		s_Data->CoreAssembly = Utils::LoadMonoAssembly(filepath);
		Utils::PrintAssemblyTypes(s_Data->CoreAssembly);

		s_Data->CoreAssemblyImage = mono_assembly_get_image(s_Data->CoreAssembly);

		
	}
	void ScriptEngine::LoadAppAssembly(const std::filesystem::path& filepath)
	{
		s_Data->AppAssemblyPath = filepath;
		s_Data->AppAssembly = Utils::LoadMonoAssembly(filepath);
		Utils::PrintAssemblyTypes(s_Data->AppAssembly);

		s_Data->AppAssemblyImage = mono_assembly_get_image(s_Data->AppAssembly);


		s_Data->AppAssemblyFilewatcher = std::make_shared<filewatch::FileWatch<std::string>>(filepath.string(), OnAppAssemblyFileSystemEvent );
		s_Data->AppAssemblyLoadPending = false;
	}
	void ScriptEngine::InitMono()
	{
		mono_set_assemblies_path("mono/lib");

		MonoDomain* RootDomain = mono_jit_init("GartJITRuntime");

		if (RootDomain == nullptr)
		{
			BSS_CORE_ERROR("Failed To initialize JIT");
			return;
		}

		s_Data->RootDomain = RootDomain;

	}

	void ScriptEngine::ShutDownMono()
	{
		mono_domain_set(mono_get_root_domain(), false);
		mono_domain_unload(s_Data->AppDomain);
		s_Data->AppDomain = nullptr;
		mono_jit_cleanup(s_Data->RootDomain);
		s_Data->RootDomain = nullptr;

	}

	MonoObject* ScriptEngine::InstantiateClass(MonoClass* monoClass)
	{
		 MonoObject* Instance = mono_object_new(s_Data->AppDomain, monoClass);
		 mono_runtime_object_init(Instance);
		 return Instance;
	}

	void  ScriptEngine::LoadAssemblyClasses()
	{
		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(s_Data->AppAssemblyImage, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

		MonoClass* entityClass = mono_class_from_name(s_Data->CoreAssemblyImage, "Gart", "Entity");

		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(s_Data->AppAssemblyImage, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(s_Data->AppAssemblyImage, cols[MONO_TYPEDEF_NAME]);

			std::string fullname;
			if (strlen(nameSpace) != 0)
				fullname = fmt::format("{}.{}", nameSpace, name);
			else
				fullname = name;

			MonoClass* monoClass = mono_class_from_name(s_Data->AppAssemblyImage, nameSpace, name);

			if (monoClass == entityClass)
				continue;

			bool IsEntity = mono_class_is_subclass_of(monoClass, entityClass, false);
			
			if (!IsEntity)
				continue;

			Ref<ScriptClass> scriptClass = std::make_shared<ScriptClass>(nameSpace, name);

			s_Data->EntityClasses[fullname] = scriptClass;

			void* Iterator = nullptr;

			while (MonoClassField* fields = mono_class_get_fields(monoClass, &Iterator))
			{
				const char* fieldName = mono_field_get_name(fields);
				uint32_t fieldFlag = mono_field_get_flags(fields);
				MonoType* fieldType = mono_field_get_type(fields);
				
				
				if (fieldFlag & FIELD_ATTRIBUTE_PUBLIC)
				{
					const char* monotypename = mono_type_get_name(fieldType);
					ScriptFieldType l_types = Utils::MonoTypeToGartType(monotypename);
					BSS_CORE_WARN("{0} - {1}", fieldName,Utils::GartTypeToString(l_types));
					scriptClass->m_FiledBuffer[fieldName] = { l_types,fieldName, fields };
				}
			}

		}

		auto& EntityClass = s_Data->EntityClasses;
	}

	void  ScriptEngine::ReloadAssemblies()
	{
		mono_domain_set(mono_get_root_domain(), false);

		mono_domain_unload(s_Data->AppDomain);

		LoadAssembly(s_Data->CoreAssemblyPath);
		LoadAppAssembly(s_Data->AppAssemblyPath);

		LoadAssemblyClasses();

		ScriptGlue::RegisterComponents();

		s_Data->EntityClass = ScriptClass::ScriptClass("Gart", "Entity", true);

	}

	std::unordered_map<std::string, Ref<ScriptClass>> ScriptEngine::GetClasses()
	{
		return s_Data->EntityClasses;
	}

	Ref<ScriptClass> ScriptEngine::GetClass(std::string& name)
	{
		return s_Data->EntityClasses[name];
	}

	void ScriptEngine::OnRuntimeStart(Scene* scene)
	{
		s_Data->SceneContext = scene;
	}


	void ScriptEngine::OnRuntimeStop()
	{
		s_Data->SceneContext = nullptr;
		s_Data->EntityInstances.clear();
	}


	bool ScriptEngine::ClassExist(std::string& classname)
	{
		return s_Data->EntityClasses.find(classname) != s_Data->EntityClasses.end();
	}

	void ScriptEngine::CreateEntity(Entity entity)
	{
		ScriptComponent& script = entity.GetComponent<ScriptComponent>();
		if (ClassExist(script.Name))
		{
			UUID id = entity.GetUUID();
			Ref<ScriptInstance> instance = std::make_shared<ScriptInstance>(s_Data->EntityClasses[script.Name],entity);
			s_Data->EntityInstances[id] = instance;
			if (s_Data->EntityFieldMap.find(id) != s_Data->EntityFieldMap.end())
			{
				const ScriptFieldMap& fieldMap = s_Data->EntityFieldMap.at(id);
				for (const auto& [name, fieldInstance] : fieldMap)
				{
					instance->SetFieldValueInternal(name, fieldInstance.m_Buffer);
				}
			}
			instance->InvokeOnCreate();
		}
	}

	void ScriptEngine::OnUpdateEntity(Entity entity, TimeStep ts)
	{
		Ref<ScriptInstance> instance = s_Data->EntityInstances[entity.GetUUID()];
		instance->InvokeOnUpdate((float)ts);
	}

	Scene* ScriptEngine::GetContext()
	{
		return s_Data->SceneContext;
	}

	MonoImage* ScriptEngine::GetCoreAssemblyImage()
	{
		return s_Data->CoreAssemblyImage;
	}

	Ref<ScriptInstance> ScriptEngine::GetEntityScriptInstance(UUID id)
	{
		auto it = s_Data->EntityInstances.find(id);
		if (it == s_Data->EntityInstances.end()) return nullptr;
		return it->second;
	}

	MonoObject* ScriptEngine::GetManagedInstance(UUID id)
	{
		
		BSS_CORE_ASSERT(s_Data->EntityInstances.find(id) != s_Data->EntityInstances.end(), "Not able to find entity.");
		return s_Data->EntityInstances.at(id)->GetManagedObject();
	}

	ScriptFieldMap& ScriptEngine::GetFieldMap(Entity entity)
	{
		BSS_CORE_ASSERT(entity, "There is no such entity");
		UUID id = entity.GetUUID();
		return s_Data->EntityFieldMap[id];
	}
#pragma endregion


#pragma region ScriptClass
	ScriptClass::ScriptClass(const std::string& nameSpace, const std::string& className, bool IsCore)
		:m_ClassNamespace(nameSpace), m_ClassName(className)
	{
		m_MonoClass = mono_class_from_name(IsCore ? s_Data->CoreAssemblyImage : s_Data->AppAssemblyImage, nameSpace.c_str(), className.c_str());
	}

	MonoObject* ScriptClass::Instantiate()
	{
		return ScriptEngine::InstantiateClass(m_MonoClass);
	}

	MonoMethod* ScriptClass::GetMethod(const char* name, int parameter)
	{
		 return mono_class_get_method_from_name(m_MonoClass, name, parameter);
	}

	MonoObject* ScriptClass::InvokeMethod(MonoMethod* method, MonoObject* instance, void** params)
	{
		return mono_runtime_invoke(method, instance, params, nullptr);
	}

#pragma endregion
	
	
#pragma region ScriptInstance

	ScriptInstance::ScriptInstance(Ref<ScriptClass> scriptClass, Entity entity)
		:m_scriptClass(scriptClass)
	{
		instance = m_scriptClass->Instantiate();

		m_Constructor = s_Data->EntityClass.GetMethod(".ctor", 1);
		m_Create = m_scriptClass->GetMethod("OnCreate",0);
		m_Update = m_scriptClass->GetMethod("OnUpdate",1);

		UUID id = entity.GetUUID();
		void* params = &id;
		m_scriptClass->InvokeMethod(m_Constructor, instance, &params);
	}

	void ScriptInstance::InvokeOnCreate()
	{
		m_scriptClass->InvokeMethod(m_Create, instance, nullptr);

	}

	void ScriptInstance::InvokeOnUpdate(float ts)
	{
		void* params = &ts;
		m_scriptClass->InvokeMethod(m_Update, instance, &params);
	}

	bool ScriptInstance::GetFieldValueInternal(const std::string& name, void* buffer)
	{
		
		const auto& fields = m_scriptClass->GetFields();
		auto it = fields.find(name);

		if (it == fields.end())
			return false;

		mono_field_get_value(instance, it->second.m_fields, buffer);
		return true;
	}

	bool ScriptInstance::SetFieldValueInternal(const std::string& name, const void* value)
	{
		const auto& fields = m_scriptClass->GetFields();

		auto it = fields.find(name);

		if (it == fields.end())
			return false;


		mono_field_set_value(instance, it->second.m_fields, (void*)value);
		return true;
	}

#pragma endregion







}