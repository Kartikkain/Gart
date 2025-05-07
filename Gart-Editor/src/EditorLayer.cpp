#include "EditorLayer.h"
#include "imgui.h"
#include <glm/gtc/type_ptr.hpp>
#include <chrono>
#include "Scene/SceneSerialization.h"
#include "Utils/PlatformUtils.h"
#include "ImGuizmo.h"
#include "Math/Math.h"
static const uint32_t s_MapWidth = 10;
static const char* s_MapTiles =
"WWWWWWWWWW"
"WWWDDDDWWW"
"WWDDDCDDWW"
"WWWDDDDWWW"
"WWWWWWWWWW"
;

namespace Gart 
{

	EditorLayer::EditorLayer()
		:Layer("Sandbox2D"), m_OrthoCamera(1280.0f / 720.0f)
	{

	}

	void EditorLayer::OnAttach()
	{
		GART_PROFILE_FUNCTION();

		m_OrthoCamera.SetZoomLevel(7.0f);

		m_Texture = Gart::Texture2D::Create("assets/textures/smile.png");
		m_SpriteSheet = Gart::Texture2D::Create("assets/game/textures/RPG.png");
		m_Tree = Gart::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 2,1 }, { 128,128 }, { 1,2 });
		m_Stairs = Gart::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 7,6 }, { 128,128 }, { 1,1 });
		m_TileSet['D'] = Gart::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 1,11 }, { 128,128 }, { 1,1 });
		m_TileSet['W'] = Gart::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 11,11 }, { 128,128 }, { 1,1 });

		m_MapWidth = s_MapWidth;
		m_MapHeight = strlen(s_MapTiles) / s_MapWidth;

		Gart::FrameBufferSpecification SceneViewFrameBuffer;

		SceneViewFrameBuffer.Width = 1280;
		SceneViewFrameBuffer.Height = 720;

		m_framebuffer = Gart::FrameBuffer::Create(SceneViewFrameBuffer);

		// Init here
		/*m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
		m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
		m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
		m_Particle.LifeTime = 1.0f;
		m_Particle.Velocity = { 0.0f, 0.0f };
		m_Particle.VelocityVariation = { 3.0f, 1.0f };
		m_Particle.Position = { 0.0f, 0.0f };*/

		m_ActiveScene = std::make_shared<Scene>();
#if 0
		auto Square = m_ActiveScene->CreateEntity("Square");
		auto GreenSquare = m_ActiveScene->CreateEntity("Green Square");


		Square.AddComponent<SpriteRenderer>(glm::vec4{ 1.0f,0.0f,0.0f,1.0f });
		m_SquareEntity = Square;

		GreenSquare.AddComponent<SpriteRenderer>(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		m_SquareEntityGreen = GreenSquare;

		m_CameraComponent = m_ActiveScene->CreateEntity("Main Camera");
		m_CameraComponent.AddComponent<CameraComponent>();
		
		m_SecondaryCameraComponent = m_ActiveScene->CreateEntity("Clipping-Space Camera");
		auto l_SecondayCam = m_SecondaryCameraComponent.AddComponent<CameraComponent>();
		l_SecondayCam.Primary = false;

		if (m_CameraComponent.HasComponent<CameraComponent>())
		{
			BSS_CLIENT_INFO("Has Camera Component");
		}

		class  CameraController : public ScriptableEntity
		{
		public:

			void OnCreate()
			{
				std::cout << "Created Camera Controller" << std::endl;
			}

			void OnUpdate(TimeStep ts)
			{
				auto& translate = GetComponent<TransformComponent>().Translate;
				float Speed = 5.0f;

				if (BSS::Input::IsKeyPressed(BSS_KEY_A))
					translate.x -= Speed * ts;

				if (BSS::Input::IsKeyPressed(BSS_KEY_D))
					translate.x += Speed * ts;

				if (BSS::Input::IsKeyPressed(BSS_KEY_W))
					translate.y += Speed * ts;

				if (BSS::Input::IsKeyPressed(BSS_KEY_S))
					translate.y -= Speed * ts;
			}

			void OnDestroy()
			{

			}
		};

		m_SecondaryCameraComponent.AddComponent<NativeScriptComponent>().Bind<CameraController>();
#endif
		m_HierarchyPanel.SetContext(m_ActiveScene);
		SceneSerialization l_Serializer(m_ActiveScene);
		l_Serializer.DeSerialize("assets/Scenes/Example.gart");
		m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
		//l_Serializer.Serialize("assets/Scenes/Example.gart");
	}

	void EditorLayer::OnDitach()
	{
		GART_PROFILE_FUNCTION();

	}

	void EditorLayer::OnUpdate(Gart::TimeStep ts)
	{
		GART_PROFILE_FUNCTION();

		m_framebuffer->Bind();

		Gart::Renderer2D::ResetStats();

		if(m_ViewPortFocus) m_OrthoCamera.OnUpdate(ts);

		Gart::RenderCommand::SetClearColor({ 0.1f,0.1f,0.1f,1 });
		Gart::RenderCommand::Clear();

		/*Gart::Renderer2D::BeginScene(m_OrthoCamera.GetCamera());

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
		}*/


		m_ActiveScene->OnUpdate(ts);


		//Gart::Renderer2D::EndScene();


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

		m_framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
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
		ImGuiStyle& style = ImGui::GetStyle();
		float widthminsize = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		style.WindowMinSize.x = widthminsize;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N")) NewScene();
				if (ImGui::MenuItem("Open...", "Ctrl+O")) OpenDialog();
				if (ImGui::MenuItem("Save as...", "Ctrl+Shift+S")) SaveScene();
				if (ImGui::MenuItem("Exit")) BSS::Application::Get().Close();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		/*static bool show = true;

		ImGui::ShowDemoWindow(&show);*/

		m_HierarchyPanel.OnGUIRender();

		ImGui::Begin("Setting");

		auto l_stats = Gart::Renderer2D::GetStats();
		ImGui::Text("Renderer2D stats:");
		ImGui::Text("Draw Calls: %d", l_stats.DrawCalls);
		ImGui::Text("Quad Counts: %d", l_stats.QuadCounts);
		ImGui::Text("Number Of Vertices: %d", l_stats.GetNumbersOfVertices());
		ImGui::Text("Number Of Indicies: %d", l_stats.GetNumbersOfIndices());

		ImGui::End();


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Scene View");

		m_ViewPortFocus = ImGui::IsWindowFocused();
		m_ViewPortHover = ImGui::IsWindowHovered();
		BSS::Application::Get().GetImGuiLayer()->BlockEvent(!m_ViewPortFocus || !m_ViewPortHover);
		ImVec2 l_ViewPortSize = ImGui::GetContentRegionAvail();
		if (m_ViewPortSize != *((glm::vec2*)&l_ViewPortSize) && l_ViewPortSize.x > 0 && l_ViewPortSize.y > 0)
		{
			m_framebuffer->Resize((uint32_t)l_ViewPortSize.x,(uint32_t)l_ViewPortSize.y);
			m_ViewPortSize = { l_ViewPortSize.x,l_ViewPortSize.y };
			m_OrthoCamera.OnResize(l_ViewPortSize.x, l_ViewPortSize.y);
			
			m_ActiveScene->OnViewportResize(l_ViewPortSize.x, l_ViewPortSize.y);
		}
		
		if(m_ViewPortFocus) BSS_CORE_INFO("Scene View port Size : ({0}, {1}) Focused", l_ViewPortSize.x, l_ViewPortSize.y);
		else BSS_CORE_INFO("Scene View port Size : ({0}, {1})", l_ViewPortSize.x, l_ViewPortSize.y);
		uint32_t texture = m_framebuffer->GetColorAttachmetID();
		ImGui::Image((void*)texture, { m_ViewPortSize.x,m_ViewPortSize.y },ImVec2(0,1),ImVec2(1,0));

		//Gizmos

		Entity m_SelectedEntity = m_HierarchyPanel.GetSelectedEntity();

		if (m_SelectedEntity && m_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			float windowWidth = (float)ImGui::GetWindowWidth();
			float windowHeight = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			// Camera
			auto cameraEntity = m_ActiveScene->GetPrimaryCamera();
			const auto& camera = cameraEntity.GetComponent<CameraComponent>().camera;
			const glm::mat4& cameraProjection = camera.GetProjection();
			glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());
			
			// Entity transform

			auto& tc = m_SelectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();


			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform));

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translate, rotation, scale;
				Math::DecomposeTransform(transform, translate, rotation, scale);
				glm::vec3 deltaRotation = rotation - tc.Rotation;
				tc.Translate = translate;
				tc.Rotation += deltaRotation; 
				tc.Scale = scale;
			}


		}


		ImGui::End();

		ImGui::PopStyleVar();


		ImGui::End();


	}

	void EditorLayer::OnEvent(BSS::Event& e)
	{
		GART_PROFILE_FUNCTION();

		m_OrthoCamera.OnEvent(e);
		BSS::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<BSS::KeyPressedEvent>(BSS_EVENT_BIND_FN(EditorLayer::OnKeyPressed));
	}
	bool EditorLayer::OnKeyPressed(BSS::KeyPressedEvent& e)
	{
		if (e.GetRepeatCount() > 0) return false;

		bool control = BSS::Input::IsKeyPressed(BSS_KEY_LEFT_CONTROL) || BSS::Input::IsKeyPressed(BSS_KEY_RIGHT_CONTROL);
		bool shift = BSS::Input::IsKeyPressed(BSS_KEY_LEFT_SHIFT) || BSS::Input::IsKeyPressed(BSS_KEY_RIGHT_SHIFT);

		switch (e.GetKeyCode())
		{
		case BSS_KEY_N:
			
			if (control) NewScene();
			break;

		case BSS_KEY_O:
			
			if (control) OpenDialog();
			break;

		case BSS_KEY_S:
			
			if (control && shift) SaveScene();
			break;

		case BSS_KEY_Q:

			m_GizmoType = -1;
			break;

		case BSS_KEY_W:
			m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
			break;

		case BSS_KEY_E:
			m_GizmoType = ImGuizmo::OPERATION::ROTATE;
			break;

		case BSS_KEY_R:
			m_GizmoType = ImGuizmo::OPERATION::SCALE;
			break;


		default:
			break;
		}
	}

	void EditorLayer::OpenDialog()
	{
		std::string filepath = FileDialogs::OpenFile("Gart Scenes (*.gart)\0*.gart\0");
		if (!filepath.empty())
		{
			m_ActiveScene = std::make_shared<Scene>();
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
			m_HierarchyPanel.SetContext(m_ActiveScene);

			SceneSerialization l_Serializer(m_ActiveScene);
			l_Serializer.DeSerialize(filepath);
		}
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = std::make_shared<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
		m_HierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::SaveScene()
	{
		std::string filepath = FileDialogs::SaveFile("Gart Scenes (*.gart)\0*.gart\0");
		if (!filepath.empty())
		{
			SceneSerialization l_Serializer(m_ActiveScene);
			l_Serializer.Serialize(filepath);
		}
	}

}


