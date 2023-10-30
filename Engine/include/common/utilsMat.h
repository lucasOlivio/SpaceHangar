#include "components/iComponent.h"
#include <glm/mat4x4.hpp>

namespace myutils
{
	void ApplyTransformInModelMat(iComponent* pTransform, glm::mat4& matModelOut);

    double distance(const glm::vec3& p1, const glm::vec3& p2);

    // Function to determine if a point is inside the circumcircle of a triangle
    bool IsInsideCircumcircle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& testPoint);

    // From: Real-Time Collision Detection- Ericson, Christer- 9781558607323- Books - Amazon.ca
    // Chapter 5:
    glm::vec3 ClosestPtPointTriangle(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c);

    // Calculate the reflection normal between and object and a triangle
    glm::vec3 GetReflectionNormal(glm::vec3 direction, glm::vec3 triangleVertices[3]);
    // https://gamedev.stackexchange.com/questions/150322/how-to-find-collision-reflection-vector-on-a-sphere
    glm::vec3 GetReflectionNormal(glm::vec3 collisionPoint, glm::vec3 velocityA, glm::vec3 centerB);

    // Check closest contact point between two spheres
    // https://stackoverflow.com/questions/42880053/finding-the-collision-point-of-2-spheres
    glm::vec3 GetSpheresContactPont(glm::vec3 centerA, float radA, glm::vec3 centerB, float radB);

    // Calculate new velocity response correcting with the restitution
    glm::vec3 ResolveVelocity(glm::vec3 velocity, glm::vec3 reflectionNormal, float restitution);

    // Calculate sin wave based on time
    float CalculateSinWave(float currentTime, float amplitude, float frequency, float phaseOffset);
}