#pragma once

#include "IPoolTemplate.h"

namespace GameObjects
{
	class Jet;
}

namespace ObjectPools
{
	class BombPool;

	const unsigned int JET_POOL_SIZE = 3;

	class JetPool : public IPoolTemplate<GameObjects::Jet>
	{
	public:
		JetPool(BombPool* bombpool);
		~JetPool();

		void Update(double deltatime);
		void Draw();

		void ReturnToPool(GameObjects::Jet* jet) override;
		void ReturnAllToPool() override;

		GameObjects::Jet* GetObjectW() override;
		vector<GameObjects::Jet*> GetActiveObjects() override;
		vector<GameObjects::Jet*> GetInactiveObjects() override;
	};
}