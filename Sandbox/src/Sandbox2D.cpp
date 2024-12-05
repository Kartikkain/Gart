#include "Sandbox2D.h"
#include "imgui.h"
#include <glm/gtc/type_ptr.hpp>
#include <chrono>

template<typename Fn>
class Timer
{
public:
	Timer(const char* name, Fn&& func)
		:m_Name(name), m_Func(func), m_Stopped(false)
	{
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}

	void Stop()
	{
		auto endTimepoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		m_Stopped = true;
		
		float duration = (end - start) * 0.001f;
		m_Func({ m_Name, duration });
	}

	~Timer()
	{
		if (!m_Stopped) Stop();
	}

private:

	const char* m_Name;
	Fn m_Func;
	std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
	bool m_Stopped;
};

#define PROFILE_SCOPE(name) Timer timer##__Line__(name,[&](ProfileResult profileResult) {m_Propf.push_back(profileResult); })
Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"), m_OrthoCamera(1280.0f / 720.0f)
{

}

void Sandbox2D::OnAttach()
{
	
	m_Texture = Gart::Texture2D::Create("assets/textures/smile.png");
}

void Sandbox2D::OnDitach()
{
}

void Sandbox2D::OnUpdate(Gart::TimeStep ts)
{
	PROFILE_SCOPE("Sandbox2D::OnUpdate");

	m_OrthoCamera.OnUpdate(ts);

	Gart::RenderCommand::SetClearColor({ 0.1f,0.1f,0.1f,1 });
	Gart::RenderCommand::Clear();

	Gart::Renderer2D::BeginScene(m_OrthoCamera.GetCamera());

	Gart::Renderer2D::DrawQuad({ 0.0f,0.0f }, { 10.0f,10.0f }, m_Texture);
	Gart::Renderer2D::DrawQuad({ 2.0f,1.0f }, { 0.2f,0.5f }, { 1.0f,0.5f,0.3f,1.0f });

	Gart::Renderer2D::EndScene();

	
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Setting");
	ImGui::ColorEdit3("Triangle Color", glm::value_ptr(m_TriangleColor));
	for (auto& result : m_Propf)
	{
		char label[50];
		strcpy(label, "%.3fms  ");
		strcat(label, result.Name);
		ImGui::Text(label, result.Time);
	}
	m_Propf.clear();
	ImGui::End();
}

void Sandbox2D::OnEvent(BSS::Event& e)
{
	m_OrthoCamera.OnEvent(e);
}
