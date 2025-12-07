#pragma once

#include "BSS.h"
#include "Panel/SceneHeirarchyPanel.h"
#include "Renderer/EditorCamera.h"
#include "Panel/ContentBrowserPanel.h"
//#include "ParticlesSystem.h"

namespace Gart
{
	class EditorLayer : public BSS::Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;
		virtual void OnAttach() override;
		virtual void OnDitach() override;
		void OnUpdate(Gart::TimeStep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(BSS::Event& e) override;
		bool OnKeyPressed(BSS::KeyPressedEvent& e);
		bool OnMouseButtonPressed(BSS::MouseButtonPressedEvent& e);
	private:
		void OpenDialog();

		bool OpenProject();
		void OpenProject(const std::filesystem::path& filepath);
		void NewProject();
		void SaveProject();

		void OpenScene(const std::filesystem::path& filepath);
		void NewScene();
		void SaveScene();

		void SaveS();
		void OnOverlayRender();

		void SerializeScene(Ref<Scene> scene, const std::filesystem::path& filepath);
		void OnScreenPlay();

		void OnSimulation();
		void OnScreenStop();

		void OnDuplicateEntity();
		void UI_Toolbar();
	private:
		Gart::OrthoGraphicCameraController m_OrthoCamera;
		Gart::EditorCamera m_EditorCamera;
		std::filesystem::path m_EditorScenePath;
		Ref<Scene> m_ActiveScene;
		Ref<Scene> m_EditorScene;
		Entity m_SquareEntity;
		Entity m_SquareEntityGreen;
		Entity m_CameraComponent;
		Entity m_SecondaryCameraComponent;
		Entity m_HoveredEntity;

		bool m_PrimaryCamera = false;
		// Temporary 
		Gart::Ref<Gart::Shader> m_Shader;
		Gart::Ref<Gart::VertexArray> m_VertexArray;
		Gart::Ref<Gart::Texture2D> m_Texture;
		Gart::Ref<Gart::Texture2D> m_SpriteSheet;
		Gart::Ref<Gart::SubTexture2D> m_Tree;
		Gart::Ref<Gart::SubTexture2D> m_Stairs;
		Gart::Ref<Gart::FrameBuffer> m_framebuffer;
		glm::vec3 m_TriangleColor = { 1.0f,0.0f,0.0f };
		glm::vec2 m_ViewPortSize = { 0.0f,0.0f };
		glm::vec2 m_ViewportBound[2];
		bool m_ViewPortFocus;
		bool m_ViewPortHover;
		bool m_ShowPhysicsColliders = true;
		int m_GizmoType = -1;

		enum class SceneState
		{
			Edit = 0, 
			Play = 1,
			Simulation = 2
		};

		SceneState m_SceneState = SceneState::Edit;

		//ParticleSystem m_ParticleSystem;
		//ParticleProps m_Particle;
		std::unordered_map<char, Gart::Ref<Gart::SubTexture2D>> m_TileSet;
		uint32_t m_MapWidth, m_MapHeight;
		SceneHeirarchyPanel m_HierarchyPanel;
		Scope<ContentBrowserPanel> m_ContentBrowserPanel;

		Ref<Texture2D> m_IconPlay, m_IconStop, m_IconSimulate,m_IconSimulateStop,m_IconPause,m_IconStep;
	};
}
