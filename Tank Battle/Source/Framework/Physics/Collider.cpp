#include "Collider.h"

namespace GameDev2D
{
	namespace Physics
	{
		Collider::Collider() :
			m_Body(nullptr),
			m_Angle(0.0f),
			m_IsEnabled(true)
		{

		}

		Collider::~Collider()
		{
			m_Body = nullptr;
		}

		void Collider::SetBody(Body* aBody)
		{
			m_Body = aBody;
		}

		Body* Collider::GetBody()
		{
			return m_Body;
		}

		void Collider::SetAngle(float aAngleInRadians)
		{
			m_Angle = aAngleInRadians;
		}

		float Collider::GetAngle()
		{
			return m_Angle;
		}

		bool Collider::GetIsEnabled()
		{
			return m_IsEnabled;
		}

		void Collider::SetIsEnabled(bool isEnabled)
		{
			m_IsEnabled = isEnabled;
		}
	}
}