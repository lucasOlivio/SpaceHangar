#pragma once

#include "Event.h"
#include "common/types.h"
#include "EnginePhysics/PhysicsProperties.hpp"
#include <glm/vec3.hpp>
#include <map>

struct sCollisionEvent
{
	EntityID entityA;
	EntityID entityB;
	sTriangle* pMeshTriangleCollision;
	glm::vec3 contactPointA;
	glm::vec3 contactPointB;
	glm::vec3 velocityAtCollisionA;
	glm::vec3 velocityAtCollisionB;
	glm::vec3 reflectionNormalA;
	glm::vec3 reflectionNormalB;
};

// Collision events triggered by physics engine
class CollisionEvent : public Event
{
private:
	std::vector<sCollisionEvent*> m_pFrameCollisions;
public:
	CollisionEvent();
	virtual ~CollisionEvent();

	void TriggerCollision(std::vector<sCollisionEvent*> vecFrameCollisionsIn);

	std::vector<sCollisionEvent*>& GetCollisions();
};