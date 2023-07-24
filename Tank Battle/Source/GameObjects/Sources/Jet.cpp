#include "../Headers/Jet.h"
#include "../Headers/Bomb.h"
#include "../../ObjectPools/Headers/BombPool.h"

namespace GameObjects
{
	Jet::Jet(BombPool* bombpool) :
		m_BombsLeft(-1),
		m_BombAliveTimer(0),
		m_Cooldown(0),
		m_BombPool(bombpool)
	{
		m_Sprite = new Sprite(GetTexture("Jet"));
		m_Sprite->SetScale(Vector2(0.2f, 0.2f));
		m_Sprite->SetAngle(0);
		m_Sprite->SetAnchor(Vector2(0.5f, 0.5f));
		m_Sprite->SetPosition(Vector2(GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f));

		float density = 1.0f;
		float radius = Math::PixelsToMeters(14.0f);
		Physics::CircleCollider* collider = new Physics::CircleCollider(radius);

		// Create the physics body
		m_PhysicsBody = Physics::World::GetInstance()->CreateBody(collider, density);
		m_PhysicsBody->SetLinearDamping(Vector2(0.2f, 0.2));
		m_PhysicsBody->SetAngularDamping(1.0f);
		m_PhysicsBody->SetMass(250);
		m_PhysicsBody->SetGameObject(this);

		SetEnabled(false);
	}

	Jet::~Jet()
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

	void Jet::Update(double deltatime)
	{
		m_Cooldown -= deltatime;
		m_BombAliveTimer -= deltatime;

		// Disable the collider after a bomb drops
		if (m_BombAliveTimer <= 0 && m_PhysicsBody->GetCollider()->GetIsEnabled())
			m_PhysicsBody->GetCollider()->SetIsEnabled(false);

		Vector2 direction = Math::CalculateDirection(m_Sprite->GetAngle());
		m_Sprite->SetPosition(m_Sprite->GetPosition() + (direction * JET_SPEED) * deltatime);

		if (m_BombsLeft <= 0)
		{
			// Disable the jet when it goes out of bounds
			if (m_Sprite->GetPosition().x < -100 || m_Sprite->GetPosition().x > GetScreenWidth() + 100 ||
				m_Sprite->GetPosition().y < -100 || m_Sprite->GetPosition().y > GetScreenHeight() + 100)
				SetEnabled(false);
		}
		else
		{
			if (m_Cooldown <= 0)
			{
				m_Cooldown = rand() % (70, 90);
				DropBomb();
			}
		}
	}

	void Jet::Draw()
	{
		m_Sprite->Draw();
	}

	void Jet::StartRun()
	{
		SetEnabled(true);

		int X[4] = { -1, 0, 1, 0 };
		int Y[4] = { -1, 0, 1, 0 };

		int dir = rand() % 4;

		if (dir == 1) // Move up
		{
			m_BombsLeft = 8;
			m_Sprite->SetAngle(90);
			m_Sprite->SetPosition(Vector2(GetScreenWidth() * Math::Random(0.1, 0.9), -90));
		}
		else if (dir == 2) // Move right
		{
			m_BombsLeft = 10;
			m_Sprite->SetAngle(0);
			m_Sprite->SetPosition(Vector2(-90, GetScreenHeight() * Math::Random(0.1, 0.9)));
		}
		else if (dir == 3) // Move down
		{
			m_BombsLeft = 8;
			m_Sprite->SetAngle(270);
			m_Sprite->SetPosition(Vector2(GetScreenWidth() * Math::Random(0.1, 0.9), GetScreenHeight() + 90));
		}
		else // Move left
		{
			m_BombsLeft = 10;
			m_Sprite->SetAngle(180);
			m_Sprite->SetPosition(Vector2(GetScreenWidth() + 90, GetScreenHeight() * Math::Random(0.1, 0.9)));
		}
	}

	void Jet::DropBomb()
	{
		m_BombsLeft--;

		if (m_Sprite->GetPosition().x > 0 && m_Sprite->GetPosition().x < GetScreenWidth()
			&& m_Sprite->GetPosition().y > 0 && m_Sprite->GetPosition().y < GetScreenHeight())
		{
			Bomb* bomb = m_BombPool->GetObjectW();

			if (bomb != nullptr)
			{ 
				m_BombAliveTimer = 20;

				bomb->SetPosition(m_Sprite->GetPosition());
				bomb->SetFrameIndex(0);
				bomb->SetEnabled(true);

				m_PhysicsBody->SetPosition(Math::PixelsToMeters(bomb->GetPosition()));
				m_PhysicsBody->GetCollider()->SetIsEnabled(true);

				Services::GetGraphics()->GetCamera()->Shake(1.3f, 1.0f);
			}		
		}
	}

	void Jet::SetEnabled(bool isenabled)
	{
		m_IsEnabled = isenabled;
		m_PhysicsBody->GetCollider()->SetIsEnabled(m_IsEnabled);
	}

	bool Jet::IsEnabled()
	{
		return m_IsEnabled;
	}

	bool Jet::IsActive()
	{
		return false;
	}

	float Jet::GetWidth()
	{
		return m_Sprite->GetWidth();
	}

	float Jet::GetHeight()
	{
		return m_Sprite->GetHeight();
	}
}
