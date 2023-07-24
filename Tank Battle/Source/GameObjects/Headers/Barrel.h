#pragma once

#include "../../ObjectPools/Headers/IPoolObject.h"
#include "ObjectColors.h"
#include <GameDev2D.h>

using namespace ObjectPools;
using namespace GameDev2D;

namespace GameObjects
{
	const float DEFAULT_BARREL_MASS = 1.76f;

	class Barrel : public Transformable, public IPoolObject
	{
	public:
		Barrel();
		~Barrel();

		bool IsExploded();
		bool IsEnabled() override;

		float GetWidth();
		float GetHeight();

		void Update(double deltatime) override;
		void Draw() override;

		void SetEnabled(bool isenabled) override;
		void Reset(Vector2 position);
		void SetDetonator(Detonator color);
		void Explode();

		Detonator GetDetonator();

	private:
		bool m_IsExploded;

		Detonator m_Detonator;

		Physics::Body* m_PhysicsBody;

		SpriteAtlas* m_BarrelSprite;
		SpriteAtlas* m_BlueDetonatorSprite;
		SpriteAtlas* m_GreenDetonatorSprite;	
		AnimatedSprite* m_ExplosionSprite;
	};
}