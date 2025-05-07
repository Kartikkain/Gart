#include "bsspch.h"
#include "Math.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
namespace Gart::Math
{
	bool DecomposeTransform(const glm::mat4& transform, glm::vec3& OutTranslate, glm::vec3& OutRotation, glm::vec3& OutScale)
	{

		// From glm::decompose in matrix_decompose.inl

		using namespace glm;
		using T = float;

		mat4 LocalMatrix(transform);

		//normalize the matrix
		if (epsilonEqual(LocalMatrix[3][3], static_cast<float>(0), epsilon<T>())) return false;

		if (
			epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
			epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
			epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>())
			)
		{
			LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
			LocalMatrix[3][3] = static_cast<T>(1);
		}

		OutTranslate = vec3(LocalMatrix[3]);
		LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

		vec3 Row[3], Pdum3;

		for (length_t i = 0; i < 3; ++i)
		{
			for (length_t j = 0; j < 3; ++j)
				Row[i][j] = LocalMatrix[i][j];
		}

		OutScale.x = length(Row[0]);
		Row[0] = detail::scale(Row[0], static_cast<T>(1));
		OutScale.y = length(Row[1]);
		Row[1] = detail::scale(Row[1], static_cast<T>(1));
		OutScale.z = length(Row[2]);
		Row[2] = detail::scale(Row[2], static_cast<T>(1));

#if 0

		Pdum3 = cross(Row[1], Row[2]);
		if (dot(Row[0], Pdum3) < 0)
		{
			for (length_t i = 0; i < 3; i++)
			{
				OutScale[i] *= static_cast<T>(-1);
				Row[i] *= static_cast<T>(-1);

			}
		}

#endif

		OutRotation.y = asin(-Row[0][2]);
		if (cos(OutRotation.y) != 0)
		{
			OutRotation.x = atan2(Row[1][2], Row[2][2]);
			OutRotation.z = atan2(Row[0][1], Row[0][0]);
		}
		else
		{
			OutRotation.x = atan2(-Row[2][0], Row[1][1]);
			OutRotation.z = 0;
		}
		return true;
	}
}