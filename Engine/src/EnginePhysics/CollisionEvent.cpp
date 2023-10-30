#include "events/CollisionEvent.h"

CollisionEvent::CollisionEvent()
{
	this->m_pFrameCollisions.clear();
}

CollisionEvent::~CollisionEvent()
{
}

void CollisionEvent::TriggerCollision(std::vector<sCollisionEvent*> pFrameCollisions)
{
	this->m_pFrameCollisions = pFrameCollisions;

	this->Notify(this);
}

std::vector<sCollisionEvent*>& CollisionEvent::GetCollisions()
{
	return this->m_pFrameCollisions;
}
