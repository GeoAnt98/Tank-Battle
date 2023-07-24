#ifndef __COLLIDER_H__
#define __COLLIDER_H__

namespace GameDev2D
{
	namespace Physics
	{
		//Forward declarations
		class Body;

		//Enum to help manage the type of colliders
		enum ColliderType
		{
			ColliderType_Circle,
			ColliderType_Box
		};

		class Collider
		{
		public:
			Collider();
			virtual ~Collider();

			virtual ColliderType GetType() = 0;
			virtual float ComputeMass(float density) = 0;
			virtual float ComputeInertia(float mass) = 0;

			void SetBody(Body* body);
			Body* GetBody();

			void SetAngle(float angleInRadians);
			float GetAngle();

			bool GetIsEnabled();
			void SetIsEnabled(bool isEnabled);

		private:
			Body* m_Body;
			float m_Angle;
			bool m_IsEnabled;
		};
	}
}


#endif
