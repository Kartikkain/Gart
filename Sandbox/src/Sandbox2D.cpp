#include "Sandbox2D.h"
#include "imgui.h"
#include <glm/gtc/type_ptr.hpp>
#include <chrono>

static const uint32_t s_MapWidth = 10;
static const char* s_MapTiles =
"WWWWWWWWWW"
"WWWDDDDWWW"
"WWDDDCDDWW"
"WWWDDDDWWW"
"WWWWWWWWWW"
;

Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"), m_OrthoCamera(1280.0f / 720.0f)
{

}

void Sandbox2D::OnAttach()
{
	GART_PROFILE_FUNCTION();

	m_OrthoCamera.SetZoomLevel(7.0f);

	m_Texture = Gart::Texture2D::Create("assets/textures/smile.png");
	m_SpriteSheet = Gart::Texture2D::Create("assets/game/textures/RPG.png");
	m_Tree = Gart::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 2,1 }, { 128,128 },{1,2});
	m_Stairs = Gart::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 7,6 }, { 128,128 },{1,1});
	m_TileSet['D'] = Gart::SubTexture2D::CreateFromCoords(m_SpriteSheet, {1,11}, {128,128}, {1,1});
	m_TileSet['W'] = Gart::SubTexture2D::CreateFromCoords(m_SpriteSheet, {11,11}, {128,128}, {1,1});

	m_MapWidth = s_MapWidth;
	m_MapHeight = strlen(s_MapTiles) / s_MapWidth;

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

	/*Gart::Renderer2D::DrawQuad({ 0.0f,0.0f }, { 2.0f,2.0f }, m_Texture,10.0f);
	Gart::Renderer2D::DrawRotateQuad({ -0.5f,0.0f }, glm::radians(- 45.0f), {1.0f,1.0f}, m_Texture, 10.0f);
	Gart::Renderer2D::DrawRotateQuad({ -0.5f,0.0f },glm::radians(45.0f),{ 0.3f,0.3f }, {0.0f,0.0f,1.0f,1.0f});
	Gart::Renderer2D::DrawQuad({ 0.0f,-1.0f }, { 0.5f,0.8f }, { 1.0f,1.0f,1.0f,1.0f });
	Gart::Renderer2D::DrawQuad({ -0.5f,0.4f }, { 0.2f,0.2f }, { 1.0f,0.0f,0.0f,1.0f });*/

	for (uint32_t y = 0; y < m_MapHeight; y++)
	{
		for (uint32_t x = 0;x < m_MapWidth;x++)
		{
			char l_tileName = s_MapTiles[x + y * m_MapWidth];
			Gart::Ref<Gart::SubTexture2D> l_texture;
			if (m_TileSet.find(l_tileName) != m_TileSet.end())
				l_texture = m_TileSet[l_tileName];
			else
				l_texture = m_Tree;

			Gart::Renderer2D::DrawQuad({ x - m_MapWidth / 2.0f,y - m_MapHeight / 2.0f }, { 1.0f,1.0f }, l_texture);

		}
	}


	//Gart::Renderer2D::DrawQuad({ 0.0f,0.0f }, { 1.0f,2.0f }, m_Tree);
	//Gart::Renderer2D::DrawQuad({ 1.5f,0.0f }, { -1.0f,1.0f }, m_Stairs);
	

	Gart::Renderer2D::EndScene();


	/*if (BSS::Input::IsMouseButtonPressed(BSS_MOUSE_BUTTON_LEFT))
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
	m_ParticleSystem.OnRender(m_OrthoCamera.GetCamera());*/

	
}

void Sandbox2D::OnImGuiRender()
{
	GART_PROFILE_FUNCTION();

	



	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static bool dockspaceOpen = true;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
	if (!opt_padding)
		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// Submit the DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}
	
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Options"))
		{
			if (ImGui::MenuItem("Close")) BSS::Application::Get().Close();
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	static bool show = true;

	ImGui::ShowDemoWindow(&show);

	ImGui::Begin("Setting");

	auto l_stats = Gart::Renderer2D::GetStats();

	ImGui::Text("Renderer2D stats:");
	ImGui::Text("Draw Calls: %d", l_stats.DrawCalls);
	ImGui::Text("Quad Counts: %d", l_stats.QuadCounts);
	ImGui::Text("Number Of Vertices: %d", l_stats.GetNumbersOfVertices());
	ImGui::Text("Number Of Indicies: %d", l_stats.GetNumbersOfIndices());

	ImGui::ColorEdit3("Triangle Color", glm::value_ptr(m_TriangleColor));
	uint32_t texture = m_Texture->GetRenderID();
	ImGui::Image((void*)texture, { 64.0f,64.0f });
	ImGui::End();

	ImGui::End();
}

void Sandbox2D::OnEvent(BSS::Event& e)
{
	GART_PROFILE_FUNCTION();

	m_OrthoCamera.OnEvent(e);
}
