#pragma once
#include "Renderer/Camera.h"

namespace Gart
{
	class SceneCamera : public Camera
	{
	public:

		SceneCamera();
		virtual ~SceneCamera() = default;
		void SetOrthographic(float size, float nearclip, float farclip);
		void SetViewportSize(uint32_t width, uint32_t height);
		void RecalculateProjection();
		const float GetOrthographicSize() const { return OrthoGraphicSize; }
		void SetOrthographicSize(float Size) { OrthoGraphicSize = Size; RecalculateProjection(); }
	private:
		float OrthoGraphicSize = 10;
		float OrthoGaphicNearClip = -1.0f;
		float OrthoGaphicFarClip = 1.0f;
		float AspectRatio = 0;

	};
}
