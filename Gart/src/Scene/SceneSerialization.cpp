#include "bsspch.h"
#include "SceneSerialization.h"
#include "Entity.h"
#include "Scene/Components.h"
#include <fstream>
#include<yaml-cpp/yaml.h>

namespace YAML
{
	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
			{
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
			{
				return false;
			}
			
			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
			{
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

namespace Gart
{
	static std::string RigidBodyTypeToString(RigidBody2DComponent::BodyType bodytype)
	{
		switch (bodytype)
		{
		case Gart::RigidBody2DComponent::BodyType::Static : return "Static";
		case Gart::RigidBody2DComponent::BodyType::Dynamic: return "Dynamic";
		case Gart::RigidBody2DComponent::BodyType::Kinematic: return "Kinematic";
		}

		
		return {};
	}

	static RigidBody2DComponent::BodyType RigidBodyTypeFromString(std::string bodytype)
	{
		
		if(bodytype == "Static") return RigidBody2DComponent::BodyType::Static;
		if (bodytype == "Dynamic") return RigidBody2DComponent::BodyType::Dynamic;
		if (bodytype == "Kinematic") return RigidBody2DComponent::BodyType::Kinematic;
		


		return RigidBody2DComponent::BodyType::Static;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}
	SceneSerialization::SceneSerialization(Ref<Scene> scene)
		:m_Scene(scene)
	{
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << "12223144324345";
		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;
			auto& tag = entity.GetComponent<TagComponent>();
			out << YAML::Key << "Tag" << YAML::Value << tag.m_Tag;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;

			auto& TC = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Position" << TC.Translate;
			out << YAML::Key << "Rotation" << TC.Rotation;
			out << YAML::Key << "Scale" << TC.Scale;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;
			auto& cameracomponent = entity.GetComponent<CameraComponent>();
			auto& camera = cameracomponent.camera;
			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap;
			out << YAML::Key << "ProjetionType" << YAML::Value << (int) camera.GetProjectionType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPrespectiveFOV();
			out << YAML::Key << "PerspectiveNearClip" << YAML::Value << camera.GetPrespectiveNearClip();
			out << YAML::Key << "PerspectiveFarClip" << YAML::Value << camera.GetPrespectiveFarClip();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNearClip" << YAML::Value << camera.GetOrthographicNearClip();
			out << YAML::Key << "OrthographicFarClip" << YAML::Value << camera.GetOrthographicFarClip();
			out << YAML::EndMap;
			
			out << YAML::Key << "Primary" << YAML::Value << cameracomponent.Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameracomponent.FixedAspectRatio;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<SpriteRenderer>())
		{
			out << YAML::Key << "SpriteRenderer";
			out << YAML::BeginMap;
			auto& spriteRenderer = entity.GetComponent<SpriteRenderer>();
			out << YAML::Key << "Color" << YAML::Value << spriteRenderer.Color;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<RigidBody2DComponent>())
		{
			out << YAML::Key << "RigidBody2DComponent";
			out << YAML::BeginMap;
			auto& rigidBody2DComponent = entity.GetComponent<RigidBody2DComponent>();
			out << YAML::Key << "BodyType" << YAML::Value << RigidBodyTypeToString(rigidBody2DComponent.Type);
			out << YAML::Key << "FixedRotation" << YAML::Value << rigidBody2DComponent.FixedRotation;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			out << YAML::Key << "BoxCollider2DComponent";
			out << YAML::BeginMap;
			auto& boxCollider2DComponent = entity.GetComponent<BoxCollider2DComponent>();
			out << YAML::Key << "Offset" << YAML::Value << boxCollider2DComponent.Offset;
			out << YAML::Key << "Size" << YAML::Value << boxCollider2DComponent.Size;
			out << YAML::Key << "Density" << YAML::Value << boxCollider2DComponent.Density;
			out << YAML::Key << "Friction" << YAML::Value << boxCollider2DComponent.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << boxCollider2DComponent.Restitution;
			out << YAML::Key << "RestitutionThreshHold" << YAML::Value << boxCollider2DComponent.RestitutionThreshHold;
			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}
	void SceneSerialization::Serialize(const std::string& filePath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_Scene->m_Registery.each([&](auto entityID)
		{
				Entity entity = { entityID, m_Scene.get() };
				if (!entity) return;
				
				SerializeEntity(out, entity);

		});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filePath);

		fout << out.c_str();
	}
	void SceneSerialization::SerializeRuntime(const std::string& filePath)
	{
	}
	bool SceneSerialization::DeSerialize(const std::string& filePath)
	{
		std::ifstream stream(filePath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());

		if(!data["Scene"])
			return false;
		
		std::string name = data["Scene"].as<std::string>();

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();

				Entity deSerialzeEntity = m_Scene->CreateEntity(name);
				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					auto& tc = deSerialzeEntity.GetComponent<TransformComponent>();
					tc.Translate = transformComponent["Position"].as<glm::vec3>();
					tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					tc.Scale = transformComponent["Scale"].as<glm::vec3>();
				}

				auto cameraCommponent = entity["CameraComponent"];
				if (cameraCommponent)
				{
					auto& CameraComp = deSerialzeEntity.AddComponent<CameraComponent>();
					auto& cameraProps = cameraCommponent["Camera"];

					CameraComp.camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjetionType"].as<int>());
					CameraComp.camera.SetPrespectiveFOV(cameraProps["PerspectiveFOV"].as<float>());
					CameraComp.camera.SetPrespectiveNearClip(cameraProps["PerspectiveNearClip"].as<float>());
					CameraComp.camera.SetPrespectiveFarClip(cameraProps["PerspectiveFarClip"].as<float>());
					CameraComp.camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
					CameraComp.camera.SetOrthographicNearClip(cameraProps["OrthographicNearClip"].as<float>());
					CameraComp.camera.SetOrthographicFarClip(cameraProps["OrthographicFarClip"].as<float>());
					CameraComp.Primary = cameraCommponent["Primary"].as<bool>();
					CameraComp.FixedAspectRatio = cameraCommponent["FixedAspectRatio"].as<bool>();
				}

				auto spriteRenderer = entity["SpriteRenderer"];
				if (spriteRenderer)
				{
					auto& Sprite = deSerialzeEntity.AddComponent<SpriteRenderer>();
					Sprite.Color = spriteRenderer["Color"].as<glm::vec4>();
				}

				auto rigidBody2DComponent = entity["RigidBody2DComponent"];
				if (rigidBody2DComponent)
				{
					auto& rb2d = deSerialzeEntity.AddComponent<RigidBody2DComponent>();
					rb2d.Type = RigidBodyTypeFromString(rigidBody2DComponent["BodyType"].as<std::string>());
					rb2d.FixedRotation = rigidBody2DComponent["FixedRotation"].as<bool>();
				}

				auto boxCollider2DComponent = entity["BoxCollider2DComponent"];
				if (boxCollider2DComponent)
				{
					auto& bc2d = deSerialzeEntity.AddComponent<BoxCollider2DComponent>();
					bc2d.Offset = boxCollider2DComponent["Offset"].as<glm::vec2>();
					bc2d.Size = boxCollider2DComponent["Size"].as<glm::vec2>();
					bc2d.Density = boxCollider2DComponent["Density"].as<float>();
					bc2d.Friction = boxCollider2DComponent["Friction"].as<float>();
					bc2d.Restitution = boxCollider2DComponent["Restitution"].as<float>();
					bc2d.RestitutionThreshHold = boxCollider2DComponent["RestitutionThreshHold"].as<float>();
					
				}


			}

			
			
		}

		return true;
	}
	bool SceneSerialization::DeSerializeRuntime(const std::string& filePath)
	{
		return false;
	}
}