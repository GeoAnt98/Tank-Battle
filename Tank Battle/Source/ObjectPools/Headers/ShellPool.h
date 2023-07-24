#pragma once

#include "IPoolTemplate.h"

namespace GameObjects
{
	class Shell;
}

namespace ObjectPools
{
	const unsigned int SHELL_POOL_SIZE = 10;

	class ShellPool : public IPoolTemplate<GameObjects::Shell>
	{
	public:
		ShellPool();
		~ShellPool();

		void Update(double deltatime);
		void Draw();

		void ReturnToPool(GameObjects::Shell* shell) override;
		void ReturnAllToPool() override;

		GameObjects::Shell* GetObjectW() override;
		vector<GameObjects::Shell*> GetActiveObjects() override;
		vector<GameObjects::Shell*> GetInactiveObjects() override;
	};
}