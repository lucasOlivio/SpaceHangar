#include "common/utilsMat.h"
#include "common/utils.h"
#include "components/Transform.h"
#include "Components/Force.h"
#include <cmath>

namespace myutils
{
    void ApplyTransformInModelMat(iComponent* pTransform, glm::mat4& matModelOut)
    {
        TransformComponent* pTransComp = dynamic_cast<TransformComponent*>(pTransform);
        if (!pTransComp)
        {
            CheckEngineError("Wrong component type!");
            return;
        }

        // Translation
        glm::mat4 matTranslate = glm::translate(glm::mat4(1.0f),
                                                pTransComp->GetPosition());

        // Rotation matrix generation
        glm::mat4 matRotation = glm::mat4(pTransComp->GetQuatOrientation());

        // Scaling matrix
        glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
                                        glm::vec3(pTransComp->GetScale()));
        //--------------------------------------------------------------

        // Combine all these transformation
        matModelOut = matModelOut * matTranslate;
        matModelOut = matModelOut * matRotation;
        matModelOut = matModelOut * matScale;

        return;
    }

    double distance(const glm::vec3& p1, const glm::vec3& p2) 
    {
        return std::hypot(p1.x - p2.x, p1.y - p2.y);
    }

    // Function to determine if a point is inside the circumcircle of a triangle
    bool IsInsideCircumcircle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& testPoint) 
    {
        // Calculate the distances between the three points that form the triangle
        double d12 = distance(p1, p2);
        double d23 = distance(p2, p3);
        double d31 = distance(p3, p1);

        // Calculate the determinant of a matrix formed by the triangle's coordinates
        double determinant = p1.x * (p2.y - p3.y) - p2.x * (p1.y - p3.y) + p3.x * (p1.y - p2.y);

        // Calculate the radius of the circumcircle
        double radius = (d12 * d23 * d31) / (4.0 * std::abs(determinant));

        // Calculate the distance between the test point and the first point of the triangle
        double distToTestPoint = distance(testPoint, p1);

        // Check if the distance to the test point is less than or equal to the circumcircle radius
        return distToTestPoint <= radius;
    }

    // From: Real-Time Collision Detection- Ericson, Christer- 9781558607323- Books - Amazon.ca
    // Chapter 5:
    glm::vec3 ClosestPtPointTriangle(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c)
    {
        glm::vec3 ab = b - a;
        glm::vec3 ac = c - a;
        glm::vec3 bc = c - b;

        // Compute parametric position s for projection P' of P on AB,
        // P' = A + s*AB, s = snom/(snom+sdenom)
        float snom = glm::dot(p - a, ab), sdenom = glm::dot(p - b, a - b);

        // Compute parametric position t for projection P' of P on AC,
        // P' = A + t*AC, s = tnom/(tnom+tdenom)
        float tnom = glm::dot(p - a, ac), tdenom = glm::dot(p - c, a - c);

        if (snom <= 0.0f && tnom <= 0.0f) return a; // Vertex region early out

        // Compute parametric position u for projection P' of P on BC,
        // P' = B + u*BC, u = unom/(unom+udenom)
        float unom = glm::dot(p - b, bc), udenom = glm::dot(p - c, b - c);

        if (sdenom <= 0.0f && unom <= 0.0f) return b; // Vertex region early out
        if (tdenom <= 0.0f && udenom <= 0.0f) return c; // Vertex region early out


        // P is outside (or on) AB if the triple scalar product [N PA PB] <= 0
        glm::vec3 n = glm::cross(b - a, c - a);
        float vc = glm::dot(n, glm::cross(a - p, b - p));
        // If P outside AB and within feature region of AB,
        // return projection of P onto AB
        if (vc <= 0.0f && snom >= 0.0f && sdenom >= 0.0f)
            return a + snom / (snom + sdenom) * ab;

        // P is outside (or on) BC if the triple scalar product [N PB PC] <= 0
        float va = glm::dot(n, glm::cross(b - p, c - p));
        // If P outside BC and within feature region of BC,
        // return projection of P onto BC
        if (va <= 0.0f && unom >= 0.0f && udenom >= 0.0f)
            return b + unom / (unom + udenom) * bc;

        // P is outside (or on) CA if the triple scalar product [N PC PA] <= 0
        float vb = glm::dot(n, glm::cross(c - p, a - p));
        // If P outside CA and within feature region of CA,
        // return projection of P onto CA
        if (vb <= 0.0f && tnom >= 0.0f && tdenom >= 0.0f)
            return a + tnom / (tnom + tdenom) * ac;

        // P must project inside face region. Compute Q using barycentric coordinates
        float u = va / (va + vb + vc);
        float v = vb / (va + vb + vc);
        float w = 1.0f - u - v; // = vc / (va + vb + vc)
        return u * a + v * b + w * c;
    }

    glm::vec3 GetReflectionNormal(glm::vec3 direction, glm::vec3 triangleVertices[3])
    {
        // Normalize sphere direction
        glm::vec3 sphereDirection = direction;
        sphereDirection = glm::normalize(sphereDirection);

        // Get triangle hit normals
        glm::vec3 edgeA = triangleVertices[1] - triangleVertices[0];
        glm::vec3 edgeB = triangleVertices[2] - triangleVertices[0];
        glm::vec3 triNormal = glm::normalize(glm::cross(edgeA, edgeB));

        // Calculate the reflection vector from the normal	
        // https://registry.khronos.org/OpenGL-Refpages/gl4/html/reflect.xhtml
        // 1st parameter is the "incident" vector
        // 2nd parameter is the "normal" vector
        return glm::reflect(sphereDirection, triNormal);
    }

    glm::vec3 GetReflectionNormal(glm::vec3 collisionPoint, glm::vec3 velocityA, glm::vec3 centerB)
    {
        // Normalize A direction
        glm::vec3 normal = glm::normalize(collisionPoint - centerB);

        return velocityA - (2 * (glm::dot(velocityA, normal)) * normal);
    }

    glm::vec3 GetSpheresContactPont(glm::vec3 centerA, float radA, glm::vec3 centerB, float radB)
    {
        return centerA + ((centerB - centerA) * radA / (radA + radB));
    }

    glm::vec3 ResolveVelocity(glm::vec3 velocity, glm::vec3 reflectionNormal, float restitution)
    {
        float newSpeed = glm::length(velocity) * restitution;
        return reflectionNormal * newSpeed;
    }

    float CalculateSinWave(float currentTime, float amplitude, float frequency, float offset) {
        // Calculate the sine value
        const double PI = 3.14;
        float sineValue = amplitude * sin(2 * PI * frequency * currentTime);

        return offset - sineValue;
    }
}