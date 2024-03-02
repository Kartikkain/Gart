#include <BSS.h>

class Sandbox : public BSS::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}

};

BSS::Application* BSS::CreateApplication()
{
	return new Sandbox();
}