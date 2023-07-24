#include "World.h"
#include "Body.h"
#include "CircleCollider.h"
#include "BoxCollider.h"
#include "../Math/Math.h"
#include "../GameDev2D.h"

namespace GameDev2D
{
	namespace Physics
	{
		World* World::s_Instance = nullptr;

		World* World::GetInstance()
		{
			if (s_Instance == nullptr)
			{
				s_Instance = new World();
			}
			return s_Instance;
		}

		World::World() :
			m_Gravity(0.0f, 0.0f),
			m_Listener(nullptr)
		{

		}

		World::~World()
		{
			for (unsigned int i = 0; i < m_Bodies.size(); i++)
				delete m_Bodies.at(i);
		}

		void World::Step(double aTimeStep)
		{
			// Clear the contacts
			m_Contacts.clear();


			// Check for collisions
			for (unsigned int i = 0; i < m_Bodies.size(); i++)
			{
				for (unsigned int j = i + 1; j < m_Bodies.size(); j++)
				{
					Body* a = m_Bodies.at(i);
					Body* b = m_Bodies.at(j);

					if (a->GetInverseMass() == 0.0f || b->GetInverseMass() == 0.0f)
					{
						continue;
					}

					// Initialize a Manifold object
					Manifold manifold(a, b);

					// Check the collision
					if (CheckCollision(a, b, &manifold) == true)
					{
						// There was a collision, notify the listener
						if (m_Listener != nullptr)
						{
							// If the collisionCallBack method returns true, add the manifold to the contacts vector
							if (m_Listener->CollisionCallBack(a, b) == true)
							{
								m_Contacts.push_back(manifold);
							}
						}
						else
						{
							// If there is no listener, then we assume there's contact
							m_Contacts.push_back(manifold);
						}
					}
				}
			}

			for (unsigned int i = 0; i < m_Bodies.size(); i++)
			{
				m_Bodies.at(i)->SyncForces(aTimeStep, m_Gravity);
			}

			// Correct any overlap before syncing velocities
			for (unsigned int i = 0; i < m_Contacts.size(); i++)
			{
				m_Contacts.at(i).CorrectOverlap();
			}

			for (unsigned int i = 0; i < m_Bodies.size(); i++)
			{
				m_Bodies.at(i)->SyncVelocities(aTimeStep);
			}

			for (unsigned int i = 0; i < m_Bodies.size(); i++)
			{
				m_Bodies.at(i)->ClearForces();
			}
		}

		void World::DebugDraw()
		{
#ifdef DEBUG_DRAW
			for (unsigned int i = 0; i < m_Bodies.size(); i++)
			{
				Body* body = m_Bodies.at(i);
				Collider* collider = body->GetCollider();

				if (collider->GetType() == ColliderType_Circle)
				{
					CircleCollider* circle = (CircleCollider*)collider;
					float radius = Math::MetersToPixels(circle->GetRadius());
					Vector2 position = Math::MetersToPixels(body->GetPosition());

					GameDev2D::DrawCircle(position.x - radius, position.y - radius, radius, Color::YellowColor(), false);

				}
				else if (collider->GetType() == ColliderType_Box)
				{
					BoxCollider* box = (BoxCollider*)collider;
					float width = Math::MetersToPixels(box->GetWidth());
					float height = Math::MetersToPixels(box->GetHeight());
					Vector2 position = Math::MetersToPixels(body->GetPosition());
					float angle = Math::RadiansToDegrees(body->GetAngle());

					GameDev2D::DrawRectangle(position.x, position.y, width, height, angle, Color::YellowColor(), false);
				}
			}
#endif
		}

		Body* World::CreateBody(Collider* aCollider, float aDensity)
		{
			Body* body = new Body(aCollider, aDensity);
			m_Bodies.push_back(body);
			return body;
		}

		void World::SetGravity(Vector2 aGravity)
		{
			m_Gravity = aGravity;
		}

		Vector2 World::GetGravity()
		{
			return m_Gravity;
		}

		void World::SetListener(WorldListener* aListener)
		{
			m_Listener = aListener;
		}


		bool World::CheckCollision(Body* aBodyA, Body* aBodyB, Manifold* aManifold)
		{
			bool result = false;

			if (!aBodyA->GetCollider()->GetIsEnabled()
				|| !aBodyB->GetCollider()->GetIsEnabled())
			{
				return result;
			}

			// Collision checks
			if (aBodyA->GetCollider()->GetType() == ColliderType_Circle &&
				aBodyB->GetCollider()->GetType() == ColliderType_Circle)
			{
				result = CheckCircleToCircle(aBodyA, aBodyB, aManifold);
			}
			else if (aBodyA->GetCollider()->GetType() == ColliderType_Circle &&
				aBodyB->GetCollider()->GetType() == ColliderType_Box)
			{
				result = CheckCircleToBox(aBodyA, aBodyB, aManifold);
			}
			else if (aBodyA->GetCollider()->GetType() == ColliderType_Box &&
				aBodyB->GetCollider()->GetType() == ColliderType_Circle)
			{
				result = CheckCircleToBox(aBodyB, aBodyA, aManifold);
				aManifold->FlipNormal();
			}
			else if (aBodyA->GetCollider()->GetType() == ColliderType_Box &&
				aBodyB->GetCollider()->GetType() == ColliderType_Box)
			{
				result = CheckBoxToBox(aBodyB, aBodyA, aManifold);
			}

			return result;
		}

		bool World::CheckCircleToCircle(Body* bodyA, Body* bodyB, Manifold* manifold)
		{
			CircleCollider* circleColliderA = (CircleCollider*)bodyA->GetCollider();
			CircleCollider* circleColliderB = (CircleCollider*)bodyB->GetCollider();

			if (circleColliderA->GetIsEnabled() == false || circleColliderB->GetIsEnabled() == false)
				return false;

			// Check the distance against the radii for collision
			float distanceSquared = Math::CalculateDistanceSquared(bodyA->GetPosition(), bodyB->GetPosition());
			float combinedRadii = circleColliderA->GetRadius() + circleColliderB->GetRadius();
			float radiiSquared = combinedRadii * combinedRadii;

			if (distanceSquared > radiiSquared)
				return false;

			// Calculate the overlap and collision normal
			float distance = sqrtf(distanceSquared);
			float overlap = combinedRadii - distance;
			Vector2 normal = Vector2(bodyB->GetPosition() - bodyA->GetPosition()) / distance;
			manifold->SetContact(overlap, normal);

			return true;
		}

		bool World::CheckCircleToBox(Body* bodyA, Body* bodyB, Manifold* manifold)
		{
			CircleCollider* circleCollider = (CircleCollider*)bodyA->GetCollider();
			BoxCollider* boxCollider = (BoxCollider*)bodyB->GetCollider();

			float radius = circleCollider->GetRadius();
			float radiusSquared = radius * radius;
			float angle = boxCollider->GetAngle();
			float c = cosf(angle);
			float s = sinf(angle);
			float cx = bodyA->GetPosition().x;
			float cy = bodyA->GetPosition().y;
			float rx = bodyB->GetPosition().x;
			float ry = bodyB->GetPosition().y;

			float circleX = c * (cx - rx) - s * (cy - ry);
			float circleY = s * (cx - rx) + c * (cy - ry);
			Vector2 circleCenter = Vector2(circleX, circleY);

			float maxSeparation = -FLT_MAX;
			int faceNormal = 0;

			for (int i = 0; i < BOX_COLLIDER_VERTEX_COUNT; i++)
			{
				Vector2 vertex1 = boxCollider->GetNormalsAtIndex(i);
				Vector2 vertex2 = circleCenter - boxCollider->GetVerticesAtIndex(i);
				float separation = Math::Dot(vertex1, vertex2);

				if (separation > radius)
				{
					return false;
				}

				if (separation > maxSeparation)
				{
					maxSeparation = separation;
					faceNormal = i;
				}
			}

			// Check to see if the circle is within the box
			if (maxSeparation < Math::EPSILON)
			{
				// Calculate the overlap and collision normal
				float overlap = circleCollider->GetRadius();
				Vector2 faceNormalV = boxCollider->GetNormalsAtIndex(faceNormal);
				Vector2 normal = Vector2(c * faceNormalV.x + s * faceNormalV.y, -s *
					faceNormalV.x + c * faceNormalV.y) * -1.0f;
				manifold->SetContact(overlap, normal);

				return true;
			}

			// Get the closest face's vertices
			Vector2 vertex1 = boxCollider->GetVerticesAtIndex(faceNormal);
			int index2 = (faceNormal + 1 < BOX_COLLIDER_VERTEX_COUNT) ? faceNormal + 1 : 0;
			Vector2 vertex2 = boxCollider->GetVerticesAtIndex(index2);
			Vector2 closestPoint = Math::CalculateClosestPoint(circleCenter, radius, vertex1, vertex2);
			float distance = Math::CalculateDistanceSquared(closestPoint, circleCenter);

			// Check for a collision
			if (distance <= radiusSquared)
			{
				// Calculate the overlap and collision normal
				float overlap = radius - sqrtf(distance);
				Vector2 normal = circleCenter - closestPoint;

				normal = Vector2(c * normal.x + s * normal.y, -s * normal.x + c * normal.y) * -1.0f;
				normal = Math::Normalize(normal);
				manifold->SetContact(overlap, normal);

				return true;
			}

			return false;
		}

		bool World::CheckBoxToBox(Body* bodyA, Body* bodyB, Manifold* manifold)
		{
			//BoxCollider* boxcolliderA = (BoxCollider*)bodyA->GetCollider();
			//BoxCollider* boxcolliderB = (BoxCollider*)bodyB->GetCollider();
			//Vector2 bodypositionA = bodyA->GetPosition();
			//Vector2 bodypositionB = bodyB->GetPosition();

			//float angleA = Math::RadiansToDegrees(boxcolliderA->GetAngle());
			//float angleB = Math::RadiansToDegrees(boxcolliderB->GetAngle());

			//Vector2 bodyheadingA = Vector2(cos(angleA), sin(angleA));
			//Vector2 bodyheadingB = Vector2(cos(angleB), sin(angleB));

			//Vector2 lrA = bodypositionA + (boxcolliderA->GetVerticesAtIndex(1) + bodyheadingA);
			//Vector2 urA = bodypositionA + (boxcolliderA->GetVerticesAtIndex(2) + bodyheadingA);
			//Vector2 ulA = bodypositionA + (boxcolliderA->GetVerticesAtIndex(3) + bodyheadingA);

			//Vector2 lrB = bodypositionB + (boxcolliderB->GetVerticesAtIndex(1) + bodyheadingB);
			//Vector2 urB = bodypositionB + (boxcolliderB->GetVerticesAtIndex(2) + bodyheadingB);
			//Vector2 ulB = bodypositionB + (boxcolliderB->GetVerticesAtIndex(3) + bodyheadingB);

			//Vector2 axisarr[4] =
			//{
			//	Vector2(urA.x - ulA.x, urA.y - ulA.y),
			//	Vector2(urA.x - lrA.x, urA.y - lrA.y),
			//	Vector2(urB.x - ulB.x, urB.y - ulB.y),
			//	Vector2(urB.x - lrB.x, urB.y - lrB.y)
			//};

			//float minA = 0;
			//float maxA = 0;
			//float minB = 0;
			//float maxB = 0;

			//for (int axisnum = 0; axisnum < 4; axisnum++)
			//{
			//	Vector2 axis = axisarr[axisnum];

			//	for (int i = 0; i < BOX_COLLIDER_VERTEX_COUNT; i++)
			//	{			
			//		Vector2 cornerposition = bodypositionA + (boxcolliderA->GetVerticesAtIndex(i) + bodyheadingA);
			//		float corner = (cornerposition.x * axis.x + cornerposition.y * axis.y) / (axis.x * axis.x + axis.y * axis.y);
			//		Vector2 projectedaxis = Vector2(corner * axis.x, corner * axis.y);
			//		float scalar = Math::Dot(projectedaxis, axis);

			//		if (i == 0)
			//		{
			//			minA = scalar;
			//			maxA = scalar;
			//		}
			//		else
			//		{
			//			if (minA > scalar)
			//				minA = scalar;
			//			if (maxA < scalar)
			//				maxA = scalar;
			//		}
			//	}

			//	for (int i = 0; i < BOX_COLLIDER_VERTEX_COUNT; i++)
			//	{
			//		Vector2 cornerposition = bodypositionB + (boxcolliderB->GetVerticesAtIndex(i) + bodyheadingB);
			//		float corner = (cornerposition.x * axis.x + cornerposition.y * axis.y) / (axis.x * axis.x + axis.y * axis.y);
			//		Vector2 projectedaxis = Vector2(corner * axis.x, corner * axis.y);
			//		float scalar = Math::Dot(projectedaxis, axis);

			//		if (i == 0)
			//		{
			//			minB = scalar;
			//			maxB = scalar;
			//		}
			//		else
			//		{
			//			if (minB > scalar)
			//				minB = scalar;
			//			if (maxB < scalar)
			//				maxB = scalar;
			//		}
			//	}

			//	if (!(minB <= maxA && maxB >= minA))
			//		return false;
			//}

			//manifold->SetContact(0.0f, Vector2(0, 0));
			//string line = "CONTACT!\n";
			//OutputDebugStringA(line.c_str());

			return false;
		}
	}
}