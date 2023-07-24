#pragma once

#include "../../ObjectPools/Headers/IPoolObject.h"
#include <GameDev2D.h>

using namespace ObjectPools;
using namespace GameDev2D;

namespace GameObjects
{
	const float SHELL_SPEED = 1000.0f;
	const float SHELL_FORCE = 60.0f;

	class Shell : public Transformable, public IPoolObject
	{
	public:
		Shell();
		~Shell();

		bool IsEnabled() override;

		float GetWidth();
		float GetHeight();

		void Update(double deltatime) override;
		void Draw() override;

		void Fire(Vector2 position, float angleindegrees);
		void SetEnabled(bool isenabled) override;

	private:
		Physics::Body* m_PhysicsBody;

		SpriteAtlas* m_Sprite;
	};
}