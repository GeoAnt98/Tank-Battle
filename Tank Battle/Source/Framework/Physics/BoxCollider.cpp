#include "BoxCollider.h"

namespace GameDev2D
{
	namespace Physics
	{
		BoxCollider::BoxCollider(float aWidth, float aHeight) :
			m_Width(aWidth),
			m_Height(aHeight)
		{
			//Calculate the box's half width and half height
			float halfWidth = m_Width / 2.0f;
			float halfHeight = m_Height / 2.0f;

			//Initialize the vertices
			m_Vertices[0] = Vector2(-halfWidth, -halfHeight);
			m_Vertices[1] = Vector2(halfWidth, -halfHeight);
			m_Vertices[2] = Vector2(halfWidth, halfHeight);
			m_Vertices[3] = Vector2(-halfWidth, halfHeight);

			//Initialize the normals
			m_Normals[0] = Vector2(0.0f, -1.0f);
			m_Normals[1] = Vector2(1.0f, 0.0f);
			m_Normals[2] = Vector2(0.0f, 1.0f);
			m_Normals[3] = Vector2(-1.0f, 0.0f);
		}

		ColliderType BoxCollider::GetType()
		{
			return ColliderType_Box;
		}

		float BoxCollider::ComputeMass(float aDensity)
		{
			return aDensity * m_Width * m_Height * 0.5f;
		}

		float BoxCollider::ComputeInertia(float aMass)
		{
			return aMass * (m_Width * m_Width + m_Height * m_Height) / 12.0f;
		}

		float BoxCollider::GetWidth()
		{
			return m_Width;
		}

		float BoxCollider::GetHeight()
		{
			return m_Height;
		}

		Vector2 BoxCollider::GetVerticesAtIndex(unsigned int aIndex)
		{
			if (aIndex < BOX_COLLIDER_VERTEX_COUNT)
			{
				return m_Vertices[aIndex];
			}
			return Vector2(0.0f, 0.0f);
		}

		Vector2 BoxCollider::GetNormalsAtIndex(unsigned int aIndex)
		{
			if (aIndex < BOX_COLLIDER_VERTEX_COUNT)
			{
				return m_Normals[aIndex];
			}
			return Vector2(0.0f, 0.0f);
		}
	}
}