#include "../Headers/Tank.h"
#include "../Headers/Shell.h"
#include "../../ObjectPools/Headers/ShellPool.h"

namespace GameObjects
{
	Tank::Tank(ShellPool* shellpool, TankColor tankcolor) :
		m_IsExploded(false),
		m_IsDisabled(false),
		m_FireShellDelay(0.0),
		m_DisabledCooldown(0),
		m_ShellPool(shellpool),
		m_BodySprite(nullptr),
		m_TurretSprite(nullptr),
		m_PhysicsBody(nullptr),
		m_FireSprite(nullptr),
		m_ExplosionSprite(nullptr),
		m_TankColor(tankcolor)
	{
		// Initialize the Tank's position and angle based on its color
		SetPosition(TANK_STARTING_POSITIONS[m_TankColor]);
		SetAngle(TANK_STARTING_ANGLE[m_TankColor]);

		Create();

		// Initialize the input array
		for (int i = 0; i < TankInputCount; i++)
			m_Input[i] = TANK_DEFAULT_INPUTS[i];
	}

	Tank::~Tank()
	{
		if (m_PhysicsBody != nullptr)
		{
			delete m_PhysicsBody;
			m_PhysicsBody = nullptr;
		}

		if (m_BodySprite != nullptr)
		{
			delete m_BodySprite;
			m_BodySprite = nullptr;
		}

		if (m_TurretSprite)
		{
			delete m_TurretSprite;
			m_TurretSprite = nullptr;
		}
		if (m_ExplosionSprite != nullptr)
		{
			delete m_ExplosionSprite;
			m_ExplosionSprite = nullptr;
		}

		if (m_FireSprite != nullptr)
		{
			delete m_FireSprite;
			m_FireSprite = nullptr;
		}
	}

	void Tank::Update(double deltatime)
	{
		m_FireShellDelay = Math::Clamp(m_FireShellDelay - deltatime, 0, TANK_FIRE_SHELL_DELAY);

		//Get the angle and calculate cos and sin of the angle
		float tankangle = m_PhysicsBody->GetAngle();
		float costankangle = cosf(tankangle);
		float sintankangle = sinf(tankangle);

		// Calculate tank velocity
		Vector2 linearvelocity = m_PhysicsBody->GetLinearVelocity();
		Vector2 rightnormal = Vector2(costankangle * NORMAL_RIGHT.x - sintankangle * NORMAL_RIGHT.y, sintankangle * NORMAL_RIGHT.x + costankangle * NORMAL_RIGHT.y);
		Vector2 lateralVelocity = rightnormal * Math::Dot(rightnormal, linearvelocity);

		// Calculate the impulse and angular impulse to apply to the body
		Vector2 impulse = -lateralVelocity * m_PhysicsBody->GetMass();
		float angularImpulse = 0.1f * m_PhysicsBody->GetInertia() * -m_PhysicsBody->GetAngularVelocity();

		m_PhysicsBody->ApplyLinearImpulse(impulse);
		m_PhysicsBody->ApplyAngularImpulse(angularImpulse);

		// Handle turret rotation if the tank's enabled
		if (!m_IsDisabled && !m_IsExploded)
		{
			if (IsKeyDown(m_Input[TankInputTurretLeft]))
				m_TurretSprite->SetAngle(m_TurretSprite->GetAngle() + TURRET_ANGULAR_SPEED * deltatime);
			if (IsKeyDown(m_Input[TankInputTurretRight]))
				m_TurretSprite->SetAngle(m_TurretSprite->GetAngle() - TURRET_ANGULAR_SPEED * deltatime);

			float force = 0.0f;
			float torque = 0.0f;

			if (IsKeyDown(m_Input[TankInputUp]))
				force += TANK_FORCE_AMOUNT;
			if (IsKeyDown(m_Input[TankInputDown]))
				force -= TANK_FORCE_AMOUNT;

			if (IsKeyDown(m_Input[TankInputLeft]) && IsKeyDown(m_Input[TankInputDown]))
				torque -= TANK_TORQUE_AMOUNT;
			else if (IsKeyDown(m_Input[TankInputLeft]))
				torque += TANK_TORQUE_AMOUNT;

			if (IsKeyDown(m_Input[TankInputRight]) && IsKeyDown(m_Input[TankInputDown]))
				torque += TANK_TORQUE_AMOUNT;
			else if (IsKeyDown(m_Input[TankInputRight]))
				torque -= TANK_TORQUE_AMOUNT;

			// Apply the force to the physics body 
			Vector2 force2d = Vector2(cosf(tankangle) * force, sinf(tankangle) * force);
			m_PhysicsBody->ApplyForces(force2d);

			// Apply the torque to the physics body 
			if (force != 0.0)
				m_PhysicsBody->ApplyTorque(torque);

			if (IsKeyDown(m_Input[TankInputFireTurret]) && m_FireShellDelay <= 0.0)
				FireShell();
		}

		if (m_IsDisabled)
		{
			m_DisabledCooldown--;
			
			if (m_DisabledCooldown <= 0)
			{
				m_DisabledCooldown = 0;
				m_IsDisabled = false;
			}
		}

		// Set the Polygon's position and angle, using the data from the physics body
		SetPosition(Math::MetersToPixels(m_PhysicsBody->GetPosition()));
		SetAngle(Math::RadiansToDegrees(tankangle));
	}

	void Tank::Draw()
	{
		m_BodySprite->Draw();
		m_TurretSprite->Draw();

		if (m_IsDisabled)
			m_FireSprite->Draw();

		if (m_IsExploded)
			m_ExplosionSprite->Draw();
	}

	void Tank::Create()
	{
		if (m_BodySprite == nullptr)
		{
			m_BodySprite = new SpriteAtlas("SpriteAtlas");
			m_BodySprite->UseFrame(TANK_COLOR_ENUM_NAMES[m_TankColor]);
			m_BodySprite->SetAnchor(Vector2(0.5f, 0.5f));
			m_BodySprite->AttachTo(this);
		}

		if (m_TurretSprite == nullptr)
		{
			m_TurretSprite = new SpriteAtlas("SpriteAtlas");
			m_TurretSprite->UseFrame("Turret");
			m_TurretSprite->SetAnchor(Vector2(0.2f, 0.5f));
			m_TurretSprite->AttachTo(this);
		}

		if (m_ExplosionSprite == nullptr)
		{
			m_ExplosionSprite = new AnimatedSprite("SpriteAtlas");
			m_ExplosionSprite->SetDoesLoop(false);
			m_ExplosionSprite->SetAnchor(Vector2(1.0f, 0.5f));

			for (int i = 1; i <= 16; i++)
				m_ExplosionSprite->AddFrame("Explosion-" + to_string(i));

			m_ExplosionSprite->AttachTo(this);
		}

		if (m_FireSprite == nullptr)
		{
			m_FireSprite = new AnimatedSprite("SpriteAtlas");

			for (int i = 10; i > 1; i--)
				m_FireSprite->AddFrame("Explosion-" + to_string(i));

			for (int i = 2; i < 11; i++)
				m_FireSprite->AddFrame("Explosion-" + to_string(i));

			m_FireSprite->SetDoesLoop(true);
			m_FireSprite->SetFrameIndex(0);
			m_FireSprite->SetFrameSpeed(20.0f);
			m_FireSprite->SetScale(Vector2(0.7f, 0.7f));
			m_FireSprite->SetAnchor(Vector2(0.5f, 0.5f));
			m_FireSprite->AttachTo(this);
		}

		if (m_PhysicsBody == nullptr)
		{
			Physics::BoxCollider* collider = new Physics::BoxCollider(Math::PixelsToMeters(GetWidth()), Math::PixelsToMeters(GetHeight()));
			m_PhysicsBody = Physics::World::GetInstance()->CreateBody(collider, 2.0f);
			m_PhysicsBody->SetPosition(Math::PixelsToMeters(GetPosition()));
			m_PhysicsBody->SetAngle(Math::DegreesToRadians(GetAngle()));
			m_PhysicsBody->SetMass(18.4082031f);
			m_PhysicsBody->SetInertia(47.6998405f);
			m_PhysicsBody->SetLinearDamping(Vector2(1.5f, 1.5f));
			m_PhysicsBody->SetAngularDamping(1.5f);
			m_PhysicsBody->SetGameObject(this);
		}
	}

	void Tank::Reset()
	{
		m_IsExploded = false;
		m_IsDisabled = false;
		m_FireShellDelay = 0.0;
		m_DisabledCooldown = 0;

		SetPosition(TANK_STARTING_POSITIONS[m_TankColor]);
		SetAngle(TANK_STARTING_ANGLE[m_TankColor]);

		m_PhysicsBody->SetPosition(Math::PixelsToMeters(GetPosition()));
		m_PhysicsBody->SetAngle(Math::DegreesToRadians(GetAngle()));
		m_PhysicsBody->SetLinearVelocity(Vector2(0, 0));
		m_PhysicsBody->SetAngularVelocity(0);
		m_PhysicsBody->GetCollider()->SetIsEnabled(true);

		m_TurretSprite->SetAngle(0);

		m_BodySprite->UseFrame(TANK_COLOR_ENUM_NAMES[m_TankColor]);
		m_TurretSprite->UseFrame("Turret");
	}

	void Tank::Explode()
	{
		if (!m_IsExploded)
		{
			m_IsExploded = true;
			m_ExplosionSprite->SetFrameIndex(0);
			m_PhysicsBody->GetCollider()->SetIsEnabled(false);
			m_BodySprite->UseFrame("Tank-Exploded");
			m_TurretSprite->UseFrame("Turret-Exploded");
		}
	}

	void Tank::Disable()
	{
		m_IsDisabled = true;

		if (m_DisabledCooldown <= 0)
			m_DisabledCooldown = 220;
	}

	void Tank::FireShell()
	{
		if (m_IsDisabled) return;

		Shell* shell = m_ShellPool->GetObjectW();

		if (shell != nullptr)
		{
			shell->Fire(GetShellPosition(), GetTurretAngle());
			m_FireShellDelay = TANK_FIRE_SHELL_DELAY;
		}
	}

	bool Tank::HasExploded()
	{
		return m_IsExploded;
	}

	bool Tank::IsDisabled()
	{
		return m_IsDisabled;
	}

	void Tank::SetLeftInput(Keyboard::Key leftinput)
	{
		m_Input[TankInputLeft] = leftinput;
	}

	void Tank::SetRightInput(Keyboard::Key rightinput)
	{
		m_Input[TankInputRight] = rightinput;
	}

	void Tank::SetUpInput(Keyboard::Key upinput)
	{
		m_Input[TankInputUp] = upinput;
	}

	void Tank::SetDownInput(Keyboard::Key downinput)
	{
		m_Input[TankInputDown] = downinput;
	}

	void Tank::SetTurretLeftInput(Keyboard::Key leftturretinput)
	{
		m_Input[TankInputTurretLeft] = leftturretinput;
	}

	void Tank::SetTurretRightInput(Keyboard::Key rightturretinput)
	{
		m_Input[TankInputTurretRight] = rightturretinput;
	}

	void Tank::SetFireTurretInput(Keyboard::Key fireturretinput)
	{
		m_Input[TankInputFireTurret] = fireturretinput;
	}

	Vector2 Tank::GetShellPosition()
	{
		// Calculate the starting position of the shell.
		float turretLength = m_TurretSprite->GetWidth() * (0.5f - m_TurretSprite->GetAnchor().x);
		Vector2 offset = Math::CalculateDirection(GetTurretAngle()) * Vector2(turretLength, turretLength);
		Vector2 position = (m_TurretSprite->GetPosition() + GetPosition());
		return position + offset;
	}

	float Tank::GetTurretAngle()
	{
		return m_TurretSprite->GetAngle() + GetAngle();
	}

	float Tank::GetWidth()
	{
		return m_BodySprite->GetWidth();
	}

	float Tank::GetHeight()
	{
		return m_BodySprite->GetHeight();
	}
}