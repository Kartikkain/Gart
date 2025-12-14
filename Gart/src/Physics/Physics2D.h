#pragma once

#include "box2d/b2_body.h"
#include "Scene/Components.h"

namespace Gart
{
	inline b2BodyType GartBodyTypeToBox2DBodyType(RigidBody2DComponent::BodyType bodytype)
	{
		switch (bodytype)
		{
		case Gart::RigidBody2DComponent::BodyType::Static: return b2_staticBody;
		case Gart::RigidBody2DComponent::BodyType::Dynamic: return b2_dynamicBody;
		case Gart::RigidBody2DComponent::BodyType::Kinematic: return b2_kinematicBody;
		}


		return b2_staticBody;
	}

	inline RigidBody2DComponent::BodyType GetBox2DToGartBodyType(b2BodyType bodytype)
	{
		switch (bodytype)
		{
		case b2_staticBody: return Gart::RigidBody2DComponent::BodyType::Static;
		case b2_dynamicBody: return Gart::RigidBody2DComponent::BodyType::Dynamic;
		case b2_kinematicBody: return Gart::RigidBody2DComponent::BodyType::Kinematic;
		}

		return Gart::RigidBody2DComponent::BodyType::Static;
	}
}