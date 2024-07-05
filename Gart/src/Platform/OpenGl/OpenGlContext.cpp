#include "bsspch.h"
#include "OpenGlContext.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<gl/GL.h>

namespace BSS
{
	OpenGlContext::OpenGlContext(GLFWwindow* windowhandle)
		:m_windowhandle(windowhandle) { }


	void OpenGlContext::init()
	{
		glfwMakeContextCurrent(m_windowhandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	}


	void OpenGlContext::swapbuffers()
	{
		
		glfwSwapBuffers(m_windowhandle);
	}
}