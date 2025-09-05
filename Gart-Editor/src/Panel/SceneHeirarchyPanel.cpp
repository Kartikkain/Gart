#include "bsspch.h"
#include "SceneHeirarchyPanel.h"
#include "Scene/Components.h"
#include  "imgui.h"
#include <filesystem>
#include <imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>
namespace Gart
{
	const std::filesystem::path s_AssetPath = "assets";

	SceneHeirarchyPanel::SceneHeirarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHeirarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
		m_SelectedEntity = {};
	}

	void SceneHeirarchyPanel::OnGUIRender()
	{
		ImGui::Begin("Hierarchy");

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
		}

		ImGui::End();
	}

	void SceneHeirarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().m_Tag;
		ImGuiTreeNodeFlags flags = (m_SelectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
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

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name,bool removeable ,Entity entity, UIFunction uiFunction)
	{
		const ImGuiTreeNodeFlags treenodeflags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed |ImGuiTreeNodeFlags_FramePadding ;
		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();
			ImVec2 contentregionAvail = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4,4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool tree = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treenodeflags, name.c_str());
			ImGui::PopStyleVar();

			bool IsComponentRemove = false;
			if (removeable)
			{
				ImGui::SameLine(contentregionAvail.x - lineHeight * 0.5f);
				if (ImGui::Button("+", ImVec2{ lineHeight,lineHeight }))
				{
					ImGui::OpenPopup("ComponentSettng");
				}
			}


			if (ImGui::BeginPopup("ComponentSettng"))
			{
				if (ImGui::MenuItem("RemmoveComponent"))
					IsComponentRemove = true;

				ImGui::EndPopup();
			}

			if (tree)
			{
				
				uiFunction(component);
				ImGui::TreePop();
			}

			if (IsComponentRemove)
				entity.RemoveComponent<T>();

		}
	}


	static void DrawVec3Control(const std::string& label, glm::vec3& value, float resetvalue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

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

		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			value.x = resetvalue;
		ImGui::PopFont();

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &value.x, 0.1f);
		ImGui::PopItemWidth();
		ImGui::SameLine();


		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f,0.8f,0.3f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f,0.9f,0.4f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f,0.8f,0.3f,1.0f });

		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			value.y = resetvalue;
		ImGui::PopFont();

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &value.y, 0.1f);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f,0.25f,0.8f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f,0.35f,0.9f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f,0.25f,0.8f,1.0f });

		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			value.z = resetvalue;
		ImGui::PopFont();

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
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
			ImGui::SameLine();
			ImGui::PushItemWidth(-1);
			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("AddComponent");

			if (ImGui::BeginPopup("AddComponent"))
			{
				if (!m_SelectedEntity.HasComponent<CameraComponent>())
				{
					if (ImGui::MenuItem("Camera"))
					{
						m_SelectedEntity.AddComponent<CameraComponent>();
						ImGui::CloseCurrentPopup();
					}
				}
				
				if (!m_SelectedEntity.HasComponent<SpriteRenderer>())
				{
					if (ImGui::MenuItem("Sprite Renderer"))
					{
						m_SelectedEntity.AddComponent<SpriteRenderer>();
						ImGui::CloseCurrentPopup();
					}
				}
				
				if (!m_SelectedEntity.HasComponent<RigidBody2DComponent>())
				{
					if (ImGui::MenuItem("RigidBody 2D"))
					{
						m_SelectedEntity.AddComponent<RigidBody2DComponent>();
						ImGui::CloseCurrentPopup();
					}
				}

				if (!m_SelectedEntity.HasComponent<BoxCollider2DComponent>())
				{
					if (ImGui::MenuItem("Box Collider 2D"))
					{
						m_SelectedEntity.AddComponent<BoxCollider2DComponent>();
						ImGui::CloseCurrentPopup();
					}
				}

				if (!m_SelectedEntity.HasComponent<CircleRendererComponent>())
				{
					if (ImGui::MenuItem("Circle Renderer"))
					{
						m_SelectedEntity.AddComponent<CircleRendererComponent>();
						ImGui::CloseCurrentPopup();
					}
				}

				ImGui::EndPopup();

			}

			ImGui::PopItemWidth();

		}

		DrawComponent<TransformComponent>("Transform", false, entity, [](auto& component)
		{
				auto& translate = component.Translate;
				auto& rotation = glm::degrees(component.Rotation);
				auto& scale = component.Scale;
				DrawVec3Control("Position", translate);
				DrawVec3Control("Rotation", rotation);
				component.Rotation = glm::radians(rotation);
				DrawVec3Control("Scale", scale, 1.0f);
		});
		
		DrawComponent<CameraComponent>("Camera Component", true, entity, [](auto& component)
		{
				auto& l_camera = component.camera;
				const char* ProjectionTypeString[] = { "Prespective","Orthographic" };
				const char* currentProjectionTypeString = ProjectionTypeString[(int)l_camera.GetProjectionType()];

				ImGui::Checkbox("Primary", &component.Primary);

				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool IsSelected = currentProjectionTypeString == ProjectionTypeString[i];
						if (ImGui::Selectable(ProjectionTypeString[i], IsSelected))
						{
							currentProjectionTypeString = ProjectionTypeString[i];
							l_camera.SetProjectionType((SceneCamera::ProjectionType)i);
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


					ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);
				}

		});

		DrawComponent<SpriteRenderer>("Sprite Renderer", true, entity, [](auto& component)
		{
				ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
				ImGui::Button("Texture",ImVec2(100.0f,0.0f));

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path texturePath = std::filesystem::path(s_AssetPath) / path;
						component.Texture = Texture2D::Create(texturePath.string());
					}
					ImGui::EndDragDropTarget();
				}

				ImGui::DragFloat("TillingFactor", &component.TillingFactor, 0.1, 0.0, 100.0);
		});	

		DrawComponent<RigidBody2DComponent>("RigidBody 2D", true, entity, [](auto& component)
		{

				const char* BodyTypeString[] = { "Static","Dynamic","kinematic"};
				const char* currentBodyTypeString = BodyTypeString[(int)component.Type];

				if (ImGui::BeginCombo("Body Type", currentBodyTypeString))
				{
					for (int i = 0; i < 3; i++)
					{
						bool IsSelected = currentBodyTypeString == BodyTypeString[i];
						if (ImGui::Selectable(BodyTypeString[i], IsSelected))
						{
							currentBodyTypeString = BodyTypeString[i];
							component.Type = (RigidBody2DComponent::BodyType)i;
						}

						if (IsSelected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}


				ImGui::Checkbox("Fixed Rotation", &component.FixedRotation);
		});


		DrawComponent<BoxCollider2DComponent>("Box Collider 2D", true, entity, [](auto& component)
			{

				ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset));
				ImGui::DragFloat2("Size", glm::value_ptr(component.Size));
				ImGui::DragFloat("Density", &component.Density, 0.01, 0.0, 1.0);
				ImGui::DragFloat("Friction", &component.Friction, 0.01, 0.0, 1.0);
				ImGui::DragFloat("Restitution", &component.Restitution, 0.01, 0.0, 1.0);
				ImGui::DragFloat("Restitution ThreshHold", &component.RestitutionThreshHold, 0.01, 0.0);
			});


		DrawComponent<CircleRendererComponent>("Circle Renderer", true, entity, [](auto& component)
			{

				ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
				ImGui::DragFloat("Thickness", &component.Thickness, 0.025f, 0.0, 1.0);
				ImGui::DragFloat("Fade", &component.Fade, 0.00025f, 0.0, 1.0);
				
			});

	}
	void SceneHeirarchyPanel::SetSelectedEntity(Entity& entity)
	{
		m_SelectedEntity = entity;
	}
}