#include "bsspch.h"
#include "Core/Log.h"
#include "ScriptEngine.h"
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"

namespace Gart
{
	struct ScriptEngineData
	{
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;

		MonoAssembly* CoreAssembly = nullptr;
	};

	static ScriptEngineData* s_Data = nullptr;

	void ScriptEngine::Init()
	{
		s_Data = new ScriptEngineData();
		InitMono();
	}

	void ScriptEngine::Shutdown()
	{
		ShutDownMono();
		delete s_Data;
	}

	char* ReadBytes(const std::string& filepath, uint32_t* outSize)
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

	MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath)
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

		MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
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

	static void NativeLog(glm::vec3* Parameter, glm::vec3* OutResult)
	{
		BSS_CORE_WARN("Value {0},{1},{2}", Parameter->x,Parameter->y,Parameter->z);

		*OutResult = glm::normalize(*Parameter);
	}

	static float NativeLogFloat(glm::vec3* Parameter)
	{
		return glm::dot(*Parameter, *Parameter);
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
		
		s_Data->AppDomain = mono_domain_create_appdomain("GartScriptRuntime", nullptr);
		mono_domain_set(s_Data->AppDomain, true);

		mono_add_internal_call("Gart.Main::NativeVector", NativeLog);
		mono_add_internal_call("Gart.Main::NativeVectorFloat", NativeLogFloat);

		s_Data->CoreAssembly = LoadCSharpAssembly("Resources/Scripts/Gart-ScriptCore.dll");
		PrintAssemblyTypes(s_Data->CoreAssembly);

		// Create An Object That Call's Constructor

		MonoImage* assemblyImage = mono_assembly_get_image(s_Data->CoreAssembly);
		MonoClass* monoClass = mono_class_from_name(assemblyImage, "Gart", "Main");
		MonoObject* Instance = mono_object_new(s_Data->AppDomain, monoClass);
		mono_runtime_object_init(Instance);


		// Call Function
		MonoMethod* printFunction = mono_class_get_method_from_name(monoClass, "PrintMessage", 0);
		mono_runtime_invoke(printFunction, Instance, nullptr, nullptr);

		// Call Function With Parameter

		MonoMethod* printFunctionWithParameter = mono_class_get_method_from_name(monoClass, "PrintInt", 1);
		int str = 5;
		void* Params = &str;
		mono_runtime_invoke(printFunctionWithParameter, Instance,&Params, nullptr);

		// Call Function With String Parameter

		MonoString* monostring = mono_string_new(s_Data->AppDomain, "C++ Is Reading C# Functions");
		MonoMethod* printFunctionWithStringParameter = mono_class_get_method_from_name(monoClass, "PrintCustomMessage", 1);
		void* stringParam = monostring;
		mono_runtime_invoke(printFunctionWithStringParameter, Instance, &stringParam, nullptr);
	}

	void ScriptEngine::ShutDownMono()
	{
		//mono_domain_unload(s_Data->AppDomain);
		s_Data->AppDomain = nullptr;
		//mono_jit_cleanup(s_Data->RootDomain);
		s_Data->RootDomain = nullptr;

	}
}