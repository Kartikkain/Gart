#pragma once
#include "Core/Core.h"
#include "Scene.h"

namespace Gart
{
	class SceneSerialization
	{
		public:
			SceneSerialization(Ref<Scene> scene);

			void Serialize(const std::string& filePath);
			void SerializeRuntime(const std::string& filePath);

			bool DeSerialize(const std::string& filePath);
			bool DeSerializeRuntime(const std::string& filePath);

		private:
			
			Ref<Scene> m_Scene;
	};
}
