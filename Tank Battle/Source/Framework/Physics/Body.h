#ifndef __GameDev2D__Body__
#define __GameDev2D__Body__

#include "../Math/Vector2.h"

namespace GameDev2D
{
	namespace Physics
	{
		//Forward declaration
		class Collider;

		class Body
		{
		public:
			Body(Collider* collider, float density);
			~Body();

			void SetPosition(Vector2 position);
			Vector2 GetPosition();

			void SetAngle(float angleInRadians);
			float GetAngle();

			void SetLinearVelocity(Vector2 linearVelocity);
			Vector2 GetLinearVelocity();

			void SetAngularVelocity(float angularVelocity);
			float GetAngularVelocity();

			void SetLinearDamping(Vector2 linearDamping);
			Vector2 GetLinearDamping();

			void SetAngularDamping(float angularDamping);
			float GetAngularDamping();

			void SetMass(float mass);
			float GetMass();
			float GetInverseMass();

			void SetInertia(float inertia);
			float GetInertia();
			float GetInverseInertia();

			void ApplyForces(Vector2 force);
			void ApplyTorque(float torque);

            void ApplyLinearImpulse(Vector2 linearImpulse); //<-----
            void ApplyAngularImpulse(float angularImpulse); //<-----

			void ClearForces();

			void SyncForces(double timeStep, Vector2 gravity);
			void SyncVelocities(double timeStep);

			Collider* GetCollider();
			Transformable* GetGameObject();
			void SetGameObject(Transformable* object);

		private:
			//Member variables go here
			Vector2 m_Position;		//In meters
			float	m_Angle;		//In radians

			Vector2 m_LinearVelocity;	//In m/s
			float m_AngularVelocity;	//In radians/second

			Vector2 m_Force;	//In Newtons
			float m_Torque;

			Vector2 m_LinearDamping;
			float m_AngularDamping;

			float m_Mass;		//In kg
			float m_InverseMass;
			float m_Inertia;	//In kg-square meters
			float m_InverseInertia;

			Collider* m_Collider;
			Transformable* m_GameObject;
		};
	}
}

#endif