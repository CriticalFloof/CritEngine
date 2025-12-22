#pragma once
#include "../Component.h"
#include "../../Math/Common.h"

struct SpatialComponent : ECS::Component
{
	// Transform
	Engine::Quaternion rotation;
	Engine::Vector3 position;
	Engine::Vector3 scale;

	// Physics
	Engine::Vector3 velocity;
	Engine::Vector3 acceleration;
	Engine::Vector3 angularVelocity;
	Engine::Vector3 angularAcceleration;
	Engine::Vector3 centerOfMass;
	float mass;
	float friction;
	float elasticity;
	float drag;
	uint32_t collisionMask;


	ENGINE_API Engine::Matrix4f GetMatrix();
};