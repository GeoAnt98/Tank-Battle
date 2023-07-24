#pragma once

#include "../../ObjectPools/Headers/IPoolObject.h"
#include <GameDev2D.h>

using namespace GameDev2D;

namespace ObjectPools
{
	class BombPool;
}

namespace GameObjects
{
	class Jet : public Transformable, public ObjectPools::IPoolObject
	{
		const int JET_SPEED = 220;

	public:
		Jet(ObjectPools::BombPool* bombpool);
		~Jet();

		bool IsActive();
		bool IsEnabled() override;

		float GetWidth();
		float GetHeight();

		void Update(double deltatime) override;
		void Draw() override;

		void DropBomb();
		void StartRun();

		void SetEnabled(bool isenabled) override;

	private:
		int m_BombsLeft;
		int m_BombAliveTimer;
		int m_Cooldown;

		Physics::Body* m_PhysicsBody;

		ObjectPools::BombPool* m_BombPool;

		Sprite* m_Sprite;
	};
}