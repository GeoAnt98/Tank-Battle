#include "../Headers/Shell.h"

namespace GameObjects
{
	Shell::Shell() :
		m_Sprite(new SpriteAtlas("SpriteAtlas")),
		m_PhysicsBody(nullptr)
	{
		// Create the Sprite
		m_Sprite->UseFrame("Shell");
		m_Sprite->SetScale(Vector2(1.5f, 1.5f));
		m_Sprite->SetAnchor(Vector2(0.5f, 0.5f));
		m_Sprite->AttachTo(this);

		// Initialize the physics body
		float density = 1.0f;
		float radius = Math::PixelsToMeters(GetHeight() / 2.0f);
		Physics::CircleCollider* collider = new Physics::CircleCollider(radius);
		collider->SetIsEnabled(m_IsEnabled);

		// Create the physics body
		m_PhysicsBody = Physics::World::GetInstance()->CreateBody(collider, density);
		m_PhysicsBody->SetLinearDamping(Vector2(0.2f, 0.2));
		m_PhysicsBody->SetAngularDamping(1.0f);
		m_PhysicsBody->GetCollider()->SetIsEnabled(m_IsEnabled);
		m_PhysicsBody->SetGameObject(this);

		SetEnabled(false);
	}

	Shell::~Shell()
	{
		if (m_PhysicsBody != nullptr)
		{
			delete m_PhysicsBody;
			m_PhysicsBody = nullptr;
		}

		if (m_Sprite != nullptr)
		{
			delete m_Sprite;
			m_Sprite = nullptr;
		}
	}

	void Shell::Update(double aDelta)
	{
		if (m_IsEnabled)
		{
			// Set the polygon's position and angle, using the data from the physics body
			SetPosition(Math::MetersToPixels(m_PhysicsBody->GetPosition()));
			SetAngle(Math::RadiansToDegrees(m_PhysicsBody->GetAngle()));
		}
	}

	void Shell::Draw()
	{
		if (m_IsEnabled)
			m_Sprite->Draw();
	}

	float Shell::GetWidth()
	{
		return m_Sprite->GetWidth();
	}

	float Shell::GetHeight()
	{
		return m_Sprite->GetHeight();
	}

	void Shell::SetEnabled(bool isenabled)
	{
		m_IsEnabled = isenabled;
		m_PhysicsBody->GetCollider()->SetIsEnabled(m_IsEnabled);
	}

	bool Shell::IsEnabled()
	{
		return m_IsEnabled;
	}

	void Shell::Fire(Vector2 position, float angleindegrees)
	{
		SetEnabled(true);

		if (m_PhysicsBody != nullptr)
		{
			// Reset the physics body linear and angular velocities
			m_PhysicsBody->SetLinearVelocity(Vector2(0.0f, 0.0f));
			m_PhysicsBody->SetAngularVelocity(0.0f);

			// Set the shell's position, angle and linear velocity
			m_PhysicsBody->SetPosition(Math::PixelsToMeters(position));

			float angleInRadians = Math::DegreesToRadians(angleindegrees);
			m_PhysicsBody->SetAngle(angleInRadians);

			Vector2 force2d = Vector2(cosf(angleInRadians) * SHELL_FORCE, sinf(angleInRadians) * SHELL_FORCE);
			m_PhysicsBody->ApplyForces(force2d);
		}
	}
}