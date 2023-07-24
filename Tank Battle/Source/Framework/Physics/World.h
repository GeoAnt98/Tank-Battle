#ifndef __GameDev2D__World__
#define __GameDev2D__World__

#include <vector>
#include "WorldListener.h"
#include "../Math/Vector2.h"
#include "Manifold.h"  //<-----

namespace GameDev2D
{
	namespace Physics
	{
		//Forward declarations
		class Body;
		class Collider;

		class World
		{
		public:
			static World* GetInstance();

			void Step(double timeStep);
			Body* CreateBody(Collider* collider, float density);

			void SetGravity(Vector2 gravity);
			Vector2 GetGravity();

			void SetListener(WorldListener * listener);

			void DebugDraw();

		private:
			World();
			~World();

			//Private collision methods
			bool CheckCollision(Body* bodyA, Body* bodyB, Manifold* manifold); //<-----
			bool CheckCircleToCircle(Body* bodyA, Body* bodyB, Manifold* manifold); //<-----
            bool CheckCircleToBox(Body* bodyA, Body* bodyB, Manifold* manifold); //<-----
            bool CheckBoxToBox(Body* aBodyA, Body* aBodyB, Manifold* aManifold);

			Vector2 m_Gravity;
			std::vector<Body*> m_Bodies;

            vector<Manifold> m_Contacts; //<-----

			WorldListener* m_Listener;

			static World* s_Instance;
		};
	}
}

#endif