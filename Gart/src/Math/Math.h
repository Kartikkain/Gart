#pragma once
#include <glm/glm.hpp>
namespace Gart {
	namespace Math
	{
		bool DecomposeTransform(const glm::mat4& transform, glm::vec3& OutTranslate, glm::vec3& OutRotation, glm::vec3& OutScale);
	}
}