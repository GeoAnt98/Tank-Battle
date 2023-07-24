#include "../Headers/Bomb.h"

namespace GameObjects
{
	Bomb::Bomb() :
		m_Sprite(new AnimatedSprite("SpriteAtlas"))
	{
		m_Sprite->SetDoesLoop(false);
		m_Sprite->SetScale(Vector2(1.2f, 1.2f));
		m_Sprite->SetAnchor(Vector2(0.5f, 0.5f));
		m_Sprite->SetPosition(GetPosition());
		m_Sprite->AttachTo(this);

		for (int i = 1; i <= 16; i++)
			m_Sprite->AddFrame("Explosion-" + to_string(i));

		m_Sprite->SetFrameIndex(m_Sprite->GetFrameCount() - 1);
	}

	Bomb::~Bomb()
	{
		if (m_Sprite != nullptr)
		{
			delete m_Sprite;
			m_Sprite = nullptr;
		}
	}

	void Bomb::Update(double aDelta)
	{
		if (m_Sprite->GetFrameIndex() == m_Sprite->GetFrameCount() - 1)
			SetEnabled(false);
	}

	void Bomb::Draw()
	{
		m_Sprite->Draw();
	}

	void Bomb::SetEnabled(bool isenabled)
	{
		m_IsEnabled = isenabled;
	}

	void Bomb::SetFrameIndex(int index)
	{
		m_Sprite->SetFrameIndex(index);
	}

	bool Bomb::IsEnabled()
	{
		return m_IsEnabled;
	}
}