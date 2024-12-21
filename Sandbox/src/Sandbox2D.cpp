#include "Sandbox2D.h"
#include "imgui.h"
#include <glm/gtc/type_ptr.hpp>
#include <chrono>


Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"), m_OrthoCamera(1280.0f / 720.0f)
{

}

void Sandbox2D::OnAttach()
{
	GART_PROFILE_FUNCTION();

	m_Texture = Gart::Texture2D::Create("assets/textures/smile.png");

	// Init here
	m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	m_Particle.LifeTime = 1.0f;
	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 3.0f, 1.0f };
	m_Particle.Position = { 0.0f, 0.0f };
}

void Sandbox2D::OnDitach()
{
	GART_PROFILE_FUNCTION();

}

void Sandbox2D::OnUpdate(Gart::TimeStep ts)
{
	GART_PROFILE_FUNCTION();

	Gart::Renderer2D::ResetStats();

	m_OrthoCamera.OnUpdate(ts);

	Gart::RenderCommand::SetClearColor({ 0.1f,0.1f,0.1f,1 });
	Gart::RenderCommand::Clear();

	Gart::Renderer2D::BeginScene(m_OrthoCamera.GetCamera());

	Gart::Renderer2D::DrawQuad({ 0.0f,0.0f }, { 2.0f,2.0f }, m_Texture,10.0f);
	//Gart::Renderer2D::DrawQuad({ -1.0f,0.0f }, { 1.0f,1.0f }, m_Texture,10.0f);
	Gart::Renderer2D::DrawRotateQuad({ -0.5f,0.0f }, glm::radians(- 45.0f), {1.0f,1.0f}, m_Texture, 10.0f);
	Gart::Renderer2D::DrawRotateQuad({ -0.5f,0.0f },glm::radians(45.0f),{ 0.3f,0.3f }, {0.0f,0.0f,1.0f,1.0f});
	//Gart::Renderer2D::DrawRotateQuad({ 0.0f,0.0f }, 45 ,{ 10.0f,10.0f }, m_Texture,10.0f,{1.0f,0.0f,0.0f,1.0f});
	Gart::Renderer2D::DrawQuad({ 0.0f,-1.0f }, { 0.5f,0.8f }, { 1.0f,1.0f,1.0f,1.0f });
	Gart::Renderer2D::DrawQuad({ -0.5f,0.4f }, { 0.2f,0.2f }, { 1.0f,0.0f,0.0f,1.0f });
	

	Gart::Renderer2D::EndScene();


	if (BSS::Input::IsMouseButtonPressed(BSS_MOUSE_BUTTON_LEFT))
	{
		auto [x, y] = BSS::Input::GetMousePosition();
		auto width = BSS::Application::Get().GetWindow().GetWidth();
		auto height = BSS::Application::Get().GetWindow().GetHeight();

		auto bounds = m_OrthoCamera.GetBounds();
		auto pos = m_OrthoCamera.GetCamera().GetPosition();
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		m_Particle.Position = { x + pos.x, y + pos.y };
		for (int i = 0; i < 5; i++)
			m_ParticleSystem.Emit(m_Particle);
	}

	m_ParticleSystem.OnUpdate(ts);
	m_ParticleSystem.OnRender(m_OrthoCamera.GetCamera());

	
}

void Sandbox2D::OnImGuiRender()
{
	GART_PROFILE_FUNCTION();

	ImGui::Begin("Setting");
	
	auto l_stats = Gart::Renderer2D::GetStats();

	ImGui::Text("Renderer2D stats:");
	ImGui::Text("Draw Calls: %d", l_stats.DrawCalls);
	ImGui::Text("Quad Counts: %d", l_stats.QuadCounts);
	ImGui::Text("Number Of Vertices: %d", l_stats.GetNumbersOfVertices());
	ImGui::Text("Number Of Indicies: %d", l_stats.GetNumbersOfIndices());

	ImGui::ColorEdit3("Triangle Color", glm::value_ptr(m_TriangleColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(BSS::Event& e)
{
	GART_PROFILE_FUNCTION();

	m_OrthoCamera.OnEvent(e);
}
