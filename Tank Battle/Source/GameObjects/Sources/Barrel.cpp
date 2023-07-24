#include "../Headers/Barrel.h"

namespace GameObjects
{
    Barrel::Barrel() :
		m_IsExploded(false),
		m_Detonator(Detonator::None),
		m_BarrelSprite(nullptr),
		m_BlueDetonatorSprite(nullptr),
        m_GreenDetonatorSprite(nullptr),
        m_PhysicsBody(nullptr)
	{
		// Create the barrel sprite
		m_BarrelSprite = new SpriteAtlas("SpriteAtlas");
		m_BarrelSprite->UseFrame("Barrel");
		m_BarrelSprite->SetAnchor(Vector2(0.5f, 0.5f));
		m_BarrelSprite->AttachTo(this);

		m_ExplosionSprite = new AnimatedSprite("SpriteAtlas");
		m_ExplosionSprite->SetAnchor(Vector2(0.5f, 0.5f));

		m_BlueDetonatorSprite = new SpriteAtlas("SpriteAtlas");
		m_BlueDetonatorSprite->UseFrame("Detonator-Blue");
		m_BlueDetonatorSprite->SetAnchor(Vector2(0.5f, 0.5f));
		m_BlueDetonatorSprite->AttachTo(this);

		m_GreenDetonatorSprite = new SpriteAtlas("SpriteAtlas");
		m_GreenDetonatorSprite->UseFrame("Detonator-Green");
		m_GreenDetonatorSprite->SetAnchor(Vector2(0.5f, 0.5f));
		m_GreenDetonatorSprite->AttachTo(this);

		for (int i = 1; i <= 16; i++)
			m_ExplosionSprite->AddFrame("Explosion-" + to_string(i));

		m_ExplosionSprite->AttachTo(this);

		// Initialize the physics body
		float density = 1.0f;
		float radius = Math::PixelsToMeters(GetHeight() / 2.0f);
		Physics::CircleCollider* circleCollider = new Physics::CircleCollider(radius);

		m_PhysicsBody = Physics::World::GetInstance()->CreateBody(circleCollider, density);
		m_PhysicsBody->SetLinearDamping(Vector2(0.2f, 0.2));
		m_PhysicsBody->SetAngularDamping(1.0f);
		m_PhysicsBody->GetCollider()->SetIsEnabled(m_IsEnabled);
		m_PhysicsBody->SetMass(DEFAULT_BARREL_MASS);
		m_PhysicsBody->SetGameObject(this);

		SetEnabled(false);
	}

	Barrel::~Barrel()
	{
		if (m_PhysicsBody != nullptr)
		{
			delete m_PhysicsBody;
			m_PhysicsBody = nullptr;
		}

		if (m_BarrelSprite != nullptr)
		{
			delete m_BarrelSprite;
			m_BarrelSprite = nullptr;
		}

        if (m_BlueDetonatorSprite != nullptr)
        {
            delete m_BlueDetonatorSprite;
			m_BlueDetonatorSprite = nullptr;
        }

		if (m_GreenDetonatorSprite != nullptr)
		{
			delete m_GreenDetonatorSprite;
			m_GreenDetonatorSprite = nullptr;
		}
	}

	void Barrel::Update(double deltatime)
	{
		SetPosition(Math::MetersToPixels(m_PhysicsBody->GetPosition()));

		if (m_Detonator == Detonator::None && IsExploded() && m_ExplosionSprite->GetFrameIndex() == m_ExplosionSprite->GetFrameCount() - 1)
			SetEnabled(false);
	}

	void Barrel::Draw()
	{
		if (IsExploded())
		{
			if (m_Detonator == Detonator::Blue)
				m_BlueDetonatorSprite->Draw();
			else if (m_Detonator == Detonator::Green)
				m_GreenDetonatorSprite->Draw();

			m_ExplosionSprite->Draw();
		}
		else		
			m_BarrelSprite->Draw();	
	}

	void Barrel::Explode()
	{
		if (!IsExploded())
		{
			m_IsExploded = true;
			m_ExplosionSprite->SetFrameIndex(0);

			if (m_Detonator == Detonator::None)
				m_PhysicsBody->GetCollider()->SetIsEnabled(false);
		}
	}

	void Barrel::Reset(Vector2 position)
	{
		m_IsExploded = false;
		m_Detonator = Detonator::None;

		SetPosition(position);

		m_PhysicsBody->SetLinearVelocity(Vector2(0, 0));
		m_PhysicsBody->SetAngularVelocity(0.0f);
		m_PhysicsBody->SetMass(DEFAULT_BARREL_MASS);
		m_PhysicsBody->SetPosition(Math::PixelsToMeters(GetPosition()));
		
		SetEnabled(true);
	}

	void Barrel::SetDetonator(Detonator color)
	{
		m_Detonator = color;
	}

	void Barrel::SetEnabled(bool isenabled)
	{
		m_IsEnabled = isenabled;
		m_PhysicsBody->GetCollider()->SetIsEnabled(m_IsEnabled);
	}

	bool Barrel::IsExploded()
	{
		return m_IsExploded;
	}

	bool Barrel::IsEnabled()
	{
		return m_IsEnabled;
	}

	float Barrel::GetWidth()
	{
		return m_BarrelSprite->GetWidth();
	}

	float Barrel::GetHeight()
	{
		return m_BarrelSprite->GetHeight();
	}

	Detonator Barrel::GetDetonator()
	{
		return m_Detonator;
	}
}