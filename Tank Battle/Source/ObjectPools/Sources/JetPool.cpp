#include "../Headers/JetPool.h"
#include "../../GameObjects/Headers/Jet.h"

namespace ObjectPools
{
	JetPool::JetPool(BombPool* bombpool)
	{
		for (int i = 0; i < JET_POOL_SIZE; i++)
			m_InactiveObjects.push_back(new GameObjects::Jet(bombpool));
	}

	JetPool::~JetPool()
	{
		for (int i = 0; i < m_InactiveObjects.size(); i++)
			delete m_InactiveObjects.at(i);

		for (int i = 0; i < m_ActiveObjects.size(); i++)
			delete m_ActiveObjects.at(i);
	}

	void JetPool::Update(double deltatime)
	{
		for (int i = 0; i < m_ActiveObjects.size(); i++)
		{
			GameObjects::Jet* jet = m_ActiveObjects.at(i);
			jet->Update(deltatime);

			if (!jet->IsEnabled())
				ReturnToPool(jet);
		}
	}

	void JetPool::Draw()
	{
		for (int i = 0; i < m_ActiveObjects.size(); i++)
			m_ActiveObjects.at(i)->Draw();
	}

	GameObjects::Jet* JetPool::GetObjectW()
	{
		if (!m_InactiveObjects.empty())
		{
			GameObjects::Jet* jet = m_InactiveObjects.back();
			m_ActiveObjects.push_back(jet);
			m_InactiveObjects.pop_back();
			return jet;
		}

		return nullptr;
	}

	void JetPool::ReturnToPool(GameObjects::Jet* jet)
	{
		jet->SetEnabled(false);

		for (int i = 0; i < m_ActiveObjects.size(); i++)
			if (m_ActiveObjects[i] == jet)
			{
				m_ActiveObjects.erase(m_ActiveObjects.begin() + i);
				m_InactiveObjects.push_back(jet);
				return;
			}
	}

	void JetPool::ReturnAllToPool()
	{
		for (int i = m_ActiveObjects.size() - 1; i >= 0; i--)
			ReturnToPool(m_ActiveObjects.at(i));
	}

	vector<GameObjects::Jet*> JetPool::GetActiveObjects()
	{
		return m_ActiveObjects;
	}

	vector<GameObjects::Jet*> JetPool::GetInactiveObjects()
	{
		return m_InactiveObjects;
	}
}