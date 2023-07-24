#pragma once

#include "ObjectColors.h"
#include <GameDev2D.h>

using namespace GameDev2D;

namespace ObjectPools
{
	class ShellPool;
}

namespace GameObjects
{
	const float TANK_STARTING_ANGLE[] = { 90.0f, 270.0f };
	const float TANK_FORCE_AMOUNT = 100.0f; // Tank speed
	const float TANK_TORQUE_AMOUNT = 140.0f; // Tank turret rotation
	const float TANK_ANGULAR_SPEED = 180.0f;
	const float TURRET_ANGULAR_SPEED = 180.0f;
	const double TANK_FIRE_SHELL_DELAY = 0.5f;

	const string TANK_COLOR_ENUM_NAMES[] = { "Tank-Blue", "Tank-Green" };

	const Vector2 NORMAL_RIGHT = Vector2(0.0f, 1.0f);

	const Vector2 TANK_STARTING_POSITIONS[] = 
	{ 
		Vector2(500, 125.0f), 
		Vector2(500, 650.0f) 
	};
	const Keyboard::Key TANK_DEFAULT_INPUTS[] = 
	{ 
		Keyboard::D,
		Keyboard::A, 
		Keyboard::W, 
		Keyboard::S, 
		Keyboard::Left, 
		Keyboard::Right, 
		Keyboard::Spacebar 
	};

	enum TankInput
	{
		TankInputLeft = 0,
		TankInputRight,
		TankInputUp,
		TankInputDown,
		TankInputTurretLeft,
		TankInputTurretRight,
		TankInputFireTurret,
		TankInputCount
	};

	class Tank : public Transformable
	{
	public:
		Tank(ObjectPools::ShellPool* shellpool, TankColor tankcolor);
		~Tank();

		bool HasExploded();
		bool IsDisabled();

		float GetWidth();
		float GetHeight();
		float GetTurretAngle();

		void Update(double deltatime);
		void Draw();

		void Create();
		void Explode();
		void Disable();
		void FireShell();
		void Reset();

		void SetLeftInput(Keyboard::Key leftinput);
		void SetRightInput(Keyboard::Key rightinput);
		void SetUpInput(Keyboard::Key upinput);
		void SetDownInput(Keyboard::Key downinput);
		void SetTurretLeftInput(Keyboard::Key leftturretinput);
		void SetTurretRightInput(Keyboard::Key rightturretinput);
		void SetFireTurretInput(Keyboard::Key fireturretinput);

		Vector2 GetShellPosition();

	private:
		bool m_IsExploded;
		bool m_IsDisabled;
		int m_DisabledCooldown;
		double m_FireShellDelay;

		Physics::Body* m_PhysicsBody;

		SpriteAtlas* m_BodySprite;
		SpriteAtlas* m_TurretSprite;
		AnimatedSprite* m_ExplosionSprite;
		AnimatedSprite* m_FireSprite;

		ObjectPools::ShellPool* m_ShellPool;

		TankColor m_TankColor;
		Keyboard::Key m_Input[TankInputCount];
	};
}