#include "bsspch.h"
#include "SceneHeirarchyPanel.h"
#include "Scene/Components.h"
#include  "imgui.h"
#include <imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>
namespace Gart
{
	SceneHeirarchyPanel::SceneHeirarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHeirarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
	}

	void SceneHeirarchyPanel::OnGUIRender()
	{
		ImGui::Begin("Heirarchy");

		m_Context->m_Registery.each([&](auto entityID)
			{
				Entity entity{ entityID, m_Context.get() };
				DrawEntityNode(entity);
			});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectedEntity = {};



		if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
			{
				m_Context->CreateEntity("Empty Entity");
			}

			ImGui::EndPopup();
		}


		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectedEntity)
		{
			DrawComponents(m_SelectedEntity);

			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("AddComponent");

			if (ImGui::BeginPopup("AddComponent"))
			{
				if (ImGui::MenuItem("Camera"))
				{
					m_SelectedEntity.AddComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Sprite Renderer"))
				{
					m_SelectedEntity.AddComponent<SpriteRenderer>();
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();

			}
		}

		ImGui::End();
	}

	void SceneHeirarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().m_Tag;
		ImGuiTreeNodeFlags flags = (m_SelectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool open = ImGui::TreeNodeEx((void*)(uint32_t)entity, flags, tag.c_str());

		if (ImGui::IsItemClicked())
		{
			m_SelectedEntity = entity;
		}

		bool IsItemDeleted = false;
		if (ImGui::BeginPopupContextItem(0, 1))
		{
			if (ImGui::MenuItem("Delete Entity"))
			{
				IsItemDeleted = true;
			}

			ImGui::EndPopup();
		}

		if (open)
		{
			ImGui::TreePop();
		}

		if (IsItemDeleted)
		{
			m_Context->DestroyEntity(entity);
			if (m_SelectedEntity == entity)
				m_SelectedEntity = {};
		}
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& value, float resetvalue = 0.0f, float columnWidth = 100.0f)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0,0 });

		float lineheight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineheight + 3.0f,  lineheight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f,0.1f,0.15f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f,0.2f,0.2f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f,0.1f,0.15f,1.0f });

		if (ImGui::Button("X", buttonSize))
			value.x = resetvalue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &value.x, 0.1f);
		ImGui::PopItemWidth();
		ImGui::SameLine();


		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f,0.8f,0.3f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f,0.9f,0.4f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f,0.8f,0.3f,1.0f });

		if (ImGui::Button("Y", buttonSize))
			value.y = resetvalue;

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &value.y, 0.1f);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f,0.25f,0.8f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f,0.35f,0.9f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f,0.25f,0.8f,1.0f });

		if (ImGui::Button("Z", buttonSize))
			value.z = resetvalue;

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &value.z, 0.1f);
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}
	void SceneHeirarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().m_Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		const ImGuiTreeNodeFlags treenodeflags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

		if (entity.HasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), treenodeflags, "Transform"))
			{
				auto& transform = entity.GetComponent<TransformComponent>();
				auto& translate = transform.Translate;
				auto& rotation = glm::degrees(transform.Rotation);
				auto& scale = transform.Scale;
				DrawVec3Control("Position", translate);
				DrawVec3Control("Rotation", rotation);
				transform.Rotation = glm::radians(rotation);
				DrawVec3Control("Scale", scale, 1.0f);

				ImGui::TreePop();
			}

		}

		if (entity.HasComponent<CameraComponent>())
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4,4 });
			bool tree = ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), treenodeflags, "Camera");
			ImGui::SameLine(ImGui::GetWindowWidth() - 20.0f);
			if (ImGui::Button("+",ImVec2{20,20}))
			{
				ImGui::OpenPopup("ComponentSettng");
			}

			ImGui::PopStyleVar();

			bool IsComponentRemove = false;

			if (ImGui::BeginPopup("ComponentSettng"))
			{
				if (ImGui::MenuItem("RemmoveComponent"))
					IsComponentRemove = true;
				
				ImGui::EndPopup();
			}

			if (tree)
			{
				auto& cameraComponent = entity.GetComponent<CameraComponent>();
				auto& l_camera = cameraComponent.camera;
				const char* ProjectionTypeString[] = { "Prespective","Orthographic" };
				const char* currentProjectionTypeString = ProjectionTypeString[(int)l_camera.GetProjectionType()];

				ImGui::Checkbox("Primary", &cameraComponent.Primary);

				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool IsSelected = currentProjectionTypeString == ProjectionTypeString[i];
						if (ImGui::Selectable(ProjectionTypeString[i], IsSelected))
						{
							currentProjectionTypeString = ProjectionTypeString[i];
							cameraComponent.camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if (IsSelected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				if (l_camera.GetProjectionType() == SceneCamera::ProjectionType::Prespective)
				{

					float PrespectiveFOV = glm::degrees(l_camera.GetPrespectiveFOV());
					if (ImGui::DragFloat("FOV", &PrespectiveFOV))
						l_camera.SetPrespectiveFOV(glm::radians(PrespectiveFOV));

					float PrespectiveNearClip = l_camera.GetPrespectiveNearClip();
					if (ImGui::DragFloat("Near Clip", &PrespectiveNearClip))
						l_camera.SetPrespectiveNearClip(PrespectiveNearClip);

					float PrespectiveFarClip = l_camera.GetPrespectiveFarClip();
					if (ImGui::DragFloat("Far Clip", &PrespectiveFarClip))
						l_camera.SetPrespectiveFarClip(PrespectiveFarClip);
				}

				if (l_camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float OrthoSize = l_camera.GetOrthographicSize();
					if (ImGui::DragFloat("OrthoGraphic  Size", &OrthoSize))
						l_camera.SetOrthographicSize(OrthoSize);

					float OrthoNearClip = l_camera.GetOrthographicNearClip();
					if (ImGui::DragFloat("Near Clip", &OrthoNearClip))
						l_camera.SetOrthographicNearClip(OrthoNearClip);

					float OrthoFarClip = l_camera.GetOrthographicFarClip();
					if (ImGui::DragFloat("Far Clip", &OrthoFarClip))
						l_camera.SetOrthographicFarClip(OrthoFarClip);


					ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.FixedAspectRatio);

				}



				ImGui::TreePop();
			}

			if (IsComponentRemove)
				entity.RemoveComponent<CameraComponent>();
		}

		if (entity.HasComponent<SpriteRenderer>())
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4,4 });
			bool tree = ImGui::TreeNodeEx((void*)typeid(SpriteRenderer).hash_code(), treenodeflags, "Sprite Renderer");
			ImGui::SameLine(ImGui::GetWindowWidth() - 20.0f);
			if (ImGui::Button("+", ImVec2{ 20,20 }))
			{
				ImGui::OpenPopup("ComponentSettng");
			}
			ImGui::PopStyleVar();

			bool IsComponentRemove = false;

			if (ImGui::BeginPopup("ComponentSettng"))
			{
				if (ImGui::MenuItem("RemmoveComponent"))
					IsComponentRemove = true;

				ImGui::EndPopup();
			}
			if (tree)
			{
				auto& spriteRennderer = entity.GetComponent<SpriteRenderer>();
				ImGui::ColorEdit4("Color", glm::value_ptr(spriteRennderer.Color));
				ImGui::TreePop();
			}

			if (IsComponentRemove)
				entity.RemoveComponent<SpriteRenderer>();

		}

	}
}