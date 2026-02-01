#pragma once
#include "Core/Core.h"
#include "Scene.h"

namespace Gart
{
	class SceneSerialization
	{
		public:
			SceneSerialization(Ref<Scene> scene);

			void Serialize(const std::filesystem::path& filePath);
			void SerializeRuntime(const std::filesystem::path& filePath);

			bool DeSerialize(const std::filesystem::path& filePath);
			bool DeSerializeRuntime(const std::filesystem::path& filePath);

		private:
			
			Ref<Scene> m_Scene;
	};
}
