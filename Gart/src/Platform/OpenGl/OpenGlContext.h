#include "Renderer/GraphicContext.h"

struct GLFWwindow;

namespace BSS
{
	class OpenGlContext : public GraphicContext
	{
	public:

		OpenGlContext(GLFWwindow* windowhandle);
		virtual void init() override;
		virtual void swapbuffers() override;


	private:

		GLFWwindow* m_windowhandle;

	};
}
