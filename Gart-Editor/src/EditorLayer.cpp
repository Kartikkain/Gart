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
	const std::filesystem::path s_AssetPath = "assets";
	EditorLayer::EditorLayer()
		:Layer("Sandbox2D"), m_OrthoCamera(1280.0f / 720.0f)
	{

	}

	void EditorLayer::OnAttach()
	{
		GART_PROFILE_FUNCTION();

		m_OrthoCamera.SetZoomLevel(7.0f);

		m_IconPlay = Gart::Texture2D::Create("Resources/Icons/PlayButton.png");
		m_IconStop = Gart::Texture2D::Create("Resources/Icons/StopButton.png");
		m_IconSimulate = Gart::Texture2D::Create("Resources/Icons/SimulateButton.png");

		m_Texture = Gart::Texture2D::Create("assets/textures/smile.png");
		m_SpriteSheet = Gart::Texture2D::Create("assets/game/textures/RPG.png");
		m_Tree = Gart::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 2,1 }, { 128,128 }, { 1,2 });
		m_Stairs = Gart::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 7,6 }, { 128,128 }, { 1,1 });
		m_TileSet['D'] = Gart::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 1,11 }, { 128,128 }, { 1,1 });
		m_TileSet['W'] = Gart::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 11,11 }, { 128,128 }, { 1,1 });

		m_MapWidth = s_MapWidth;
		m_MapHeight = strlen(s_MapTiles) / s_MapWidth;

		Gart::FrameBufferSpecification SceneViewFrameBuffer;

		SceneViewFrameBuffer.Attachments = { FrameBufferTextureFormat::RGBA8,FrameBufferTextureFormat::RED_INTEGER,FrameBufferTextureFormat::Depth };
		SceneViewFrameBuffer.Width = 1280;
		SceneViewFrameBuffer.Height = 720;
		m_EditorCamera = EditorCamera(30.0f, 1.77, 0.1f, 1000.0f);
		m_framebuffer = Gart::FrameBuffer::Create(SceneViewFrameBuffer);

		// Init here
		/*m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
		m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
		m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
		m_Particle.LifeTime = 1.0f;
		m_Particle.Velocity = { 0.0f, 0.0f };
		m_Particle.VelocityVariation = { 3.0f, 1.0f };
		m_Particle.Position = { 0.0f, 0.0f };*/

		m_EditorScene = std::make_shared<Scene>();
		m_ActiveScene = m_EditorScene;
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
		m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
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

		/*if (m_ViewPortFocus)
		{
			m_OrthoCamera.OnUpdate(ts);
			m_EditorCamera.OnUpdate(ts);
		}*/

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

		m_framebuffer->ClearColorAttachment(1, -1);
		//m_ActiveScene->OnUpdateEditor(ts,m_EditorCamera);


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

		switch (m_SceneState)
		{
		case SceneState::Edit: 
		{
			if (m_ViewPortFocus)
				m_OrthoCamera.OnUpdate(ts);
			
			m_EditorCamera.OnUpdate(ts);

			m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);

			break;
		}

		case SceneState::Simulation:
		{
			m_EditorCamera.OnUpdate(ts);
			m_ActiveScene->OnUpdateSimulation(ts, m_EditorCamera);
			break;
		}

		case SceneState::Play:
			m_ActiveScene->OnUpdateRuntime(ts);
			break;
		}

		auto[mx,my] = ImGui::GetMousePos();
		mx -= m_ViewportBound[0].x;
		my -= m_ViewportBound[0].y;
		glm::vec2 l_viewPortSize = m_ViewportBound[1] - m_ViewportBound[0];
		my = l_viewPortSize.y - my;
		int mousex = (int)mx;
		int mousey = (int)my;
		if (mousex >= 0 && mousey >= 0 && mousex <= (int)l_viewPortSize.x && mousey < (int)l_viewPortSize.y)
		{
			int pixel = m_framebuffer->ReadPixel(1, mousex, mousey);
			m_HoveredEntity = pixel == -1 && pixel < -1 ? Entity() : Entity((entt::entity)pixel, m_ActiveScene.get());
			BSS_CLIENT_INFO("pixel {0}", pixel);
		}
		OnOverlayRender();

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
		m_ContentBrowserPanel.OnimGuiRender();

		ImGui::Begin("Setting");

		auto l_stats = Gart::Renderer2D::GetStats();
		std::string EntityName = "None";
		ImGui::Text("Renderer2D stats:");
		if (m_HoveredEntity)
			EntityName = m_HoveredEntity.GetComponent<TagComponent>().m_Tag;

		ImGui::Text("Hovered Entity : %s", EntityName.c_str());
		ImGui::Text("Draw Calls: %d", l_stats.DrawCalls);
		ImGui::Text("Quad Counts: %d", l_stats.QuadCounts);
		ImGui::Text("Number Of Vertices: %d", l_stats.GetNumbersOfVertices());
		ImGui::Text("Number Of Indicies: %d", l_stats.GetNumbersOfIndices());
		ImGui::Checkbox("Show Physics Colliders", &m_ShowPhysicsColliders);
		ImGui::End();


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Scene View");

		auto m_ViewPortminRegion = ImGui::GetWindowContentRegionMin();
		auto m_ViewPortmaxRegion = ImGui::GetWindowContentRegionMax();
		auto m_ViewPortOffset = ImGui::GetWindowPos();

		m_ViewportBound[0] = { m_ViewPortminRegion.x + m_ViewPortOffset.x, m_ViewPortminRegion.y + m_ViewPortOffset.y };
		m_ViewportBound[1] = { m_ViewPortmaxRegion.x + m_ViewPortOffset.x, m_ViewPortmaxRegion.y + m_ViewPortOffset.y };

		m_ViewPortFocus = ImGui::IsWindowFocused();
		m_ViewPortHover = ImGui::IsWindowHovered();
		BSS::Application::Get().GetImGuiLayer()->BlockEvent(!m_ViewPortFocus && !m_ViewPortHover);
		ImVec2 l_ViewPortSize = ImGui::GetContentRegionAvail();
		if (m_ViewPortSize != *((glm::vec2*)&l_ViewPortSize) && l_ViewPortSize.x > 0 && l_ViewPortSize.y > 0)
		{
			m_framebuffer->Resize((uint32_t)l_ViewPortSize.x,(uint32_t)l_ViewPortSize.y);
			m_ViewPortSize = { l_ViewPortSize.x,l_ViewPortSize.y };
			m_EditorCamera.SetViewportSize(l_ViewPortSize.x, l_ViewPortSize.y);
			m_OrthoCamera.OnResize(l_ViewPortSize.x, l_ViewPortSize.y);
			
			m_ActiveScene->OnViewportResize(l_ViewPortSize.x, l_ViewPortSize.y);
		}
		
		if(m_ViewPortFocus) BSS_CORE_INFO("Scene View port Size : ({0}, {1}) Focused", l_ViewPortSize.x, l_ViewPortSize.y);
		else BSS_CORE_INFO("Scene View port Size : ({0}, {1})", l_ViewPortSize.x, l_ViewPortSize.y);
		uint32_t texture = m_framebuffer->GetColorAttachmetID(0);
		ImGui::Image((void*)texture, { m_ViewPortSize.x,m_ViewPortSize.y },ImVec2(0,1),ImVec2(1,0));

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				OpenScene(std::filesystem::path(s_AssetPath) / path);
			}
			ImGui::EndDragDropTarget();
		}

		Entity m_SelectedEntity = m_HierarchyPanel.GetSelectedEntity();

		if (m_SelectedEntity && m_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			

			ImGuizmo::SetRect(m_ViewportBound[0].x, m_ViewportBound[0].y,
				m_ViewportBound[1].x - m_ViewportBound[0].x,
				m_ViewportBound[1].y - m_ViewportBound[0].y);

			
			//Editor Camera

			const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
			glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

			// Entity transform

			auto& tc = m_SelectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			bool snap = BSS::Input::IsKeyPressed(BSS_KEY_LEFT_CONTROL);

			float snapValue = 0.5f;
			if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;

			float snapValues[3] = { snapValue,snapValue,snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),nullptr,snap?snapValues:nullptr);

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

		UI_Toolbar();

		ImGui::End();


	}

	void EditorLayer::OnEvent(BSS::Event& e)
	{
		GART_PROFILE_FUNCTION();
		m_EditorCamera.OnEvent(e);
		m_OrthoCamera.OnEvent(e);
		BSS::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<BSS::KeyPressedEvent>(BSS_EVENT_BIND_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<BSS::MouseButtonPressedEvent>(BSS_EVENT_BIND_FN(EditorLayer::OnMouseButtonPressed));
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
			else if (control) SaveS();
			break;

		case BSS_KEY_D:
			if (control) OnDuplicateEntity();
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

	bool EditorLayer::OnMouseButtonPressed(BSS::MouseButtonPressedEvent& e)
	{
		if (e.GetButton() == BSS_MOUSE_BUTTON_LEFT)
		{
			if(m_ViewPortHover && !ImGuizmo::IsOver() && !BSS::Input::IsKeyPressed(BSS_KEY_LEFT_ALT))
				m_HierarchyPanel.SetSelectedEntity(m_HoveredEntity);
		}

		return false;
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

	void EditorLayer::OpenScene(const std::filesystem::path& filepath)
	{
		if (m_SceneState != SceneState::Edit)
			OnScreenStop();

		Ref<Scene> newScene = std::make_shared<Scene>();
		SceneSerialization serializer(newScene);

		if (serializer.DeSerialize(filepath.string()))
		{
			m_EditorScene = newScene;
			m_EditorScene->OnViewportResize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
			m_ActiveScene = m_EditorScene;
			m_HierarchyPanel.SetContext(m_ActiveScene);
			m_EditorScenePath = filepath;
			//SceneSerialization l_Serializer(m_ActiveScene);
			//l_Serializer.DeSerialize(filepath.string());
		}
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = std::make_shared<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewPortSize.x, (uint32_t)m_ViewPortSize.y);
		m_HierarchyPanel.SetContext(m_ActiveScene);

		m_EditorScenePath = std::filesystem::path();
	}

	void EditorLayer::SaveScene()
	{
		std::string filepath = FileDialogs::SaveFile("Gart Scenes (*.gart)\0*.gart\0");
		if (!filepath.empty())
		{
			
			m_EditorScenePath = filepath;
			SerializeScene(m_ActiveScene, m_EditorScenePath);
		}
	}

	void EditorLayer::SaveS()
	{
		if (!m_EditorScenePath.empty())
		{
			SerializeScene(m_ActiveScene, m_EditorScenePath);
		}
		else SaveScene();
	}

	void EditorLayer::OnOverlayRender()
	{
		

		if (m_SceneState == SceneState::Play)
		{
			Entity l_camera = m_ActiveScene->GetPrimaryCamera();
			if(l_camera)
				Renderer2D::BeginScene(l_camera.GetComponent<CameraComponent>().camera, l_camera.GetComponent<TransformComponent>().GetTransform());
		}
		else
		{
			Renderer2D::BeginScene(m_EditorCamera);
		}

		if (m_ShowPhysicsColliders)
		{

			// Draw Circle Physics Colliders

			{
				auto view = m_ActiveScene->GetAllEntityWith<TransformComponent, CircleCollider2DComponent>();
				for (auto entity : view)
				{
					auto [tc, cc2d] = view.get<TransformComponent, CircleCollider2DComponent>(entity);
					glm::vec3 translate = tc.Translate + glm::vec3(cc2d.Offset, 0.001f);
					glm::vec3 scale = tc.Scale * glm::vec3(cc2d.Radius * 2.0f);
					glm::mat4 transform = glm::translate(glm::mat4(1.0f), translate)
						* glm::scale(glm::mat4(1.0f), scale);
					Renderer2D::DrawCircle(transform, glm::vec4(0, 1, 0, 1), 0.05f);
				}
			}

			// Drawing Box Physics Colliders

			{
				auto view = m_ActiveScene->GetAllEntityWith<TransformComponent, BoxCollider2DComponent>();
				for (auto entity : view)
				{
					auto [tc, bc2d] = view.get<TransformComponent, BoxCollider2DComponent>(entity);

					glm::vec3 translate = tc.Translate + glm::vec3(bc2d.Offset, 0.001f);
					glm::vec3 scale = tc.Scale * glm::vec3(bc2d.Size * 2.0f, 1);

					glm::mat4 transform = glm::translate(glm::mat4(1.0f), translate)
						* glm::rotate(glm::mat4(1.0), tc.Rotation.z, glm::vec3(0, 0, 1))
						* glm::scale(glm::mat4(1.0f), scale);

					Renderer2D::DrawRect(transform, glm::vec4(0, 1, 0, 1));

				}
			}
		}

		Renderer2D::EndScene();
	}
	void EditorLayer::SerializeScene(Ref<Scene> scene, const std::filesystem::path& filepath)
	{
		SceneSerialization l_Serializer(scene);
		l_Serializer.Serialize(filepath.string());
	}

	void EditorLayer::OnScreenPlay()
	{
		if (m_SceneState == SceneState::Simulation)
			OnScreenStop();
		m_SceneState = SceneState::Play;
		m_ActiveScene = Scene::Copy(m_EditorScene);
		m_ActiveScene->OnRuntimeStart();
		
	}

	void EditorLayer::OnSimulation()
	{
		if (m_SceneState == SceneState::Play)
			OnScreenStop();

		m_SceneState = SceneState::Simulation;
		m_ActiveScene = Scene::Copy(m_EditorScene);
		m_ActiveScene->OnSimulationStart();
	}

	void EditorLayer::OnScreenStop()
	{
		if (m_SceneState == SceneState::Play)
			m_ActiveScene->OnRuntimeStop();
		else if (m_SceneState == SceneState::Simulation)
			m_ActiveScene->OnSimulationStop();

		m_SceneState = SceneState::Edit;
		m_ActiveScene = m_EditorScene;
	}

	void EditorLayer::OnDuplicateEntity()
	{
		if (m_SceneState != SceneState::Edit) return;

		Entity selectedEntity = m_HierarchyPanel.GetSelectedEntity();

		if (selectedEntity)
		{
		  m_EditorScene->DuplicateEntity(selectedEntity);
		}
	}



	void EditorLayer::UI_Toolbar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

		auto& colors = ImGui::GetStyle().Colors;
		const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];

		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));

		const auto& buttonActive = colors[ImGuiCol_ButtonActive];

		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

		ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		float size = ImGui::GetWindowHeight() - 4.0f;
		bool toolbar = (bool)m_ActiveScene;
		{
			Ref<Texture2D> icon = (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulation) ? m_IconPlay : m_IconStop;

			ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

			if (ImGui::ImageButton((ImTextureID)icon->GetRenderID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0) && toolbar)
			{
				if (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Simulation && toolbar)
					OnScreenPlay();
				else if (m_SceneState == SceneState::Play)
					OnScreenStop();
			}
		}
		ImGui::SameLine();
		{

			Ref<Texture2D> icon = (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play) ? m_IconSimulate : m_IconStop;

			if (ImGui::ImageButton((ImTextureID)icon->GetRenderID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0) && toolbar)
			{
				if (m_SceneState == SceneState::Edit || m_SceneState == SceneState::Play && toolbar)
					OnSimulation();
				else if (m_SceneState == SceneState::Simulation)
					OnScreenStop();
			}
		}
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);

		ImGui::End();
	}

}


