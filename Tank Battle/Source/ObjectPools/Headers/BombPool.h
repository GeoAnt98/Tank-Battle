#pragma once

#include "IPoolTemplate.h"

namespace GameObjects
{
	class Bomb;
}

namespace ObjectPools
{
	const unsigned int BOMB_POOL_SIZE = 40;

	class BombPool : public IPoolTemplate<GameObjects::Bomb>
	{
	public:
		BombPool();
		~BombPool();

		void Update(double deltatime);
		void Draw();

		void ReturnToPool(GameObjects::Bomb* bomb) override;
		void ReturnAllToPool() override;

		GameObjects::Bomb* GetObjectW() override;
		vector<GameObjects::Bomb*> GetActiveObjects() override;
		vector<GameObjects::Bomb*> GetInactiveObjects() override;
	};
}