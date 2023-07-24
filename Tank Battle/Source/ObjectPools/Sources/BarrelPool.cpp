#include "../Headers/BarrelPool.h"
#include "../../GameObjects/Headers/Barrel.h"

namespace ObjectPools
{
	BarrelPool::BarrelPool()
	{
		for (int i = 0; i < BARREL_POOL_SIZE; i++)
			m_InactiveObjects.push_back(new GameObjects::Barrel());
	}

	BarrelPool::~BarrelPool()
	{
		for (int i = 0; i < m_InactiveObjects.size(); i++)
			delete m_InactiveObjects.at(i);

		for (int i = 0; i < m_ActiveObjects.size(); i++)
			delete m_ActiveObjects.at(i);
	}

	void BarrelPool::Update(double deltatime)
	{
		for (int i = 0; i < m_ActiveObjects.size(); i++)
		{
			GameObjects::Barrel* barrel = m_ActiveObjects.at(i);
			barrel->Update(deltatime);

			if (!barrel->IsEnabled())
				ReturnToPool(barrel);
		}
	}

	void BarrelPool::Draw()
	{
		for (int i = 0; i < m_ActiveObjects.size(); i++)
			m_ActiveObjects.at(i)->Draw();
	}

	GameObjects::Barrel* BarrelPool::GetObjectW()
	{
		if (!m_InactiveObjects.empty())
		{
			GameObjects::Barrel* barrel = m_InactiveObjects.back();
			m_ActiveObjects.push_back(barrel);
			m_InactiveObjects.pop_back();
			return barrel;
		}

		return nullptr;
	}

	void BarrelPool::ReturnToPool(GameObjects::Barrel* barrel)
	{
		barrel->SetEnabled(false);

		for (int i = 0; i < m_ActiveObjects.size(); i++)
			if (m_ActiveObjects[i] == barrel)
			{
				m_ActiveObjects.erase(m_ActiveObjects.begin() + i);
				m_InactiveObjects.push_back(barrel);
				return;
			}
	}

	void BarrelPool::ReturnAllToPool()
	{
		for (int i = m_ActiveObjects.size() - 1; i >= 0; i--)
			ReturnToPool(m_ActiveObjects.at(i));
	}

	vector<GameObjects::Barrel*> BarrelPool::GetActiveObjects()
	{
		return m_ActiveObjects;
	}

	vector<GameObjects::Barrel*> BarrelPool::GetInactiveObjects()
	{
		return m_InactiveObjects;
	}
}