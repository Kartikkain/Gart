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
}

void Sandbox2D::OnDitach()
{
	GART_PROFILE_FUNCTION();

}

void Sandbox2D::OnUpdate(Gart::TimeStep ts)
{
	GART_PROFILE_FUNCTION();


	m_OrthoCamera.OnUpdate(ts);

	Gart::RenderCommand::SetClearColor({ 0.1f,0.1f,0.1f,1 });
	Gart::RenderCommand::Clear();

	Gart::Renderer2D::BeginScene(m_OrthoCamera.GetCamera());

	//Gart::Renderer2D::DrawQuad({ 0.0f,0.0f }, { 10.0f,10.0f }, m_Texture,10.0f);
	Gart::Renderer2D::DrawRotateQuad({ 0.0f,0.0f }, 45 ,{ 10.0f,10.0f }, m_Texture,10.0f,{1.0f,0.0f,0.0f,1.0f});
	Gart::Renderer2D::DrawQuad({ 2.0f,1.0f }, { 0.5f,0.8f }, { 1.0f,1.0f,1.0f,1.0f });

	Gart::Renderer2D::EndScene();

	
}

void Sandbox2D::OnImGuiRender()
{
	GART_PROFILE_FUNCTION();

	ImGui::Begin("Setting");
	ImGui::ColorEdit3("Triangle Color", glm::value_ptr(m_TriangleColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(BSS::Event& e)
{
	GART_PROFILE_FUNCTION();

	m_OrthoCamera.OnEvent(e);
}
