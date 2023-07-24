#include "../Headers/BombPool.h"
#include "../../GameObjects/Headers/Bomb.h"

namespace ObjectPools
{
	BombPool::BombPool()
	{
		for (int i = 0; i < BOMB_POOL_SIZE; i++)
			m_InactiveObjects.push_back(new GameObjects::Bomb());
	}

	BombPool::~BombPool()
	{
		for (int i = 0; i < m_InactiveObjects.size(); i++)
			delete m_InactiveObjects.at(i);

		for (int i = 0; i < m_ActiveObjects.size(); i++)
			delete m_ActiveObjects.at(i);
	}

	void BombPool::Update(double deltatime)
	{
		for (int i = 0; i < m_ActiveObjects.size(); i++)
		{
			GameObjects::Bomb* bomb = m_ActiveObjects.at(i);
			bomb->Update(deltatime);

			if (!bomb->IsEnabled())
				ReturnToPool(bomb);
		}
	}

	void BombPool::Draw()
	{
		for (int i = 0; i < m_ActiveObjects.size(); i++)
			m_ActiveObjects.at(i)->Draw();
	}

	GameObjects::Bomb* BombPool::GetObjectW()
	{
		if (!m_InactiveObjects.empty())
		{
			GameObjects::Bomb* bomb = m_InactiveObjects.back();
			m_ActiveObjects.push_back(bomb);
			m_InactiveObjects.pop_back();
			return bomb;
		}

		return nullptr;
	}

	void BombPool::ReturnToPool(GameObjects::Bomb* bomb)
	{
		bomb->SetEnabled(false);

		for (int i = 0; i < m_ActiveObjects.size(); i++)
			if (m_ActiveObjects[i] == bomb)
			{
				m_ActiveObjects.erase(m_ActiveObjects.begin() + i);
				m_InactiveObjects.push_back(bomb);
				return;
			}
	}

	void BombPool::ReturnAllToPool()
	{
		for (int i = m_ActiveObjects.size() - 1; i >= 0; i--)
			ReturnToPool(m_ActiveObjects.at(i));
	}

	vector<GameObjects::Bomb*> BombPool::GetActiveObjects()
	{
		return m_ActiveObjects;
	}

	vector<GameObjects::Bomb*> BombPool::GetInactiveObjects()
	{
		return m_InactiveObjects;
	}
}