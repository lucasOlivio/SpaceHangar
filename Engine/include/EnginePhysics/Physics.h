#pragma once

#include "scene/SceneView.h"
#include "events/CollisionEvent.h"
#include "components/Transform.h"
#include "components/Model.h"

class Physics
{
private:
	bool m_isRunning;

	SceneView* m_pSceneView;
	CollisionEvent* m_pCollisionEvent;

	// Collisions that happened in the frame
	std::vector<sCollisionEvent*> m_vecFrameCollisions;
	// All the collisions that we already tested in the frame (true or false)
	std::vector<sCollisionEvent*> m_vecFrameCollisionsVisited;

public:
	Physics(SceneView* pSceneView, CollisionEvent* pCollisionEvent);
	~Physics();

	// Update object transform based on velocity and acceleration
	void ApplyForce(EntityID entityID, double deltaTime);

	// Check every object in scene for collision between the collisionShapes
	// Add all the collision events packages with all info to the vector, it doesn't clear the vector!
	// These collision events pointers must be handled by the caller! (delete)
	void CheckCollisions(EntityID entityA, std::vector<sCollisionEvent*>& collisionsOut);
	// Add the collisions for this entity to the frame collision events
	void CheckCollisions(EntityID entityA);

	// Check if collision test was already made
	bool IsAlreadyTested(EntityID entityA, EntityID entityB);

	// Resets collisions
	void NewFrame();

	// Apply force on the entity
	void Update(EntityID entityID, double deltaTime);

	bool IsRunning();
	void SetRunning(bool isRunning);

	// Collision tests
	// Returns the collision event packet with the minimum need collision info

	// Sphere - Mesh
	bool SphereTriMeshIndirect_Test(sSphere* pSphere, TransformComponent* pTransformSphere,
									sMesh* pMesh, TransformComponent* pTransformMesh,
									sCollisionEvent* pCollision);

	// Sphere - Sphere
	bool SphereSphere_Test(sSphere* pSphereA, TransformComponent* pTransformA,
							sSphere* pSphereB, TransformComponent* pTransformB,
							sCollisionEvent* pCollision);
};
