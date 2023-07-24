#pragma once

#include "IPoolTemplate.h"

namespace GameObjects
{
	class Barrel;
}

namespace ObjectPools
{
	const unsigned int BARREL_POOL_SIZE = 50;

	class BarrelPool : public IPoolTemplate<GameObjects::Barrel>
	{
	public:
		BarrelPool();
		~BarrelPool();

		void Update(double deltatime);
		void Draw();

		void ReturnToPool(GameObjects::Barrel* barrel) override;
		void ReturnAllToPool() override;

		GameObjects::Barrel* GetObjectW() override;
		vector<GameObjects::Barrel*> GetActiveObjects() override;
		vector<GameObjects::Barrel*> GetInactiveObjects() override;
	};
}