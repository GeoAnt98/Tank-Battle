#pragma once

#include "../../ObjectPools/Headers/IPoolObject.h"
#include <GameDev2D.h>

using namespace ObjectPools;
using namespace GameDev2D;

namespace GameObjects
{
	class Bomb : public Transformable, public IPoolObject
	{
	public:
		Bomb();
		~Bomb();

		void Update(double deltatime) override;
		void Draw() override;

		bool IsEnabled() override;

		void SetEnabled(bool isenabled) override;
		void SetFrameIndex(int index);

	private:
		AnimatedSprite* m_Sprite;
	};
}