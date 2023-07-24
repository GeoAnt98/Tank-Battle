#include "../Headers/ShellPool.h"
#include "../../GameObjects/Headers/Shell.h"

namespace ObjectPools
{
	ShellPool::ShellPool()
	{
		for (int i = 0; i < SHELL_POOL_SIZE; i++)
			m_InactiveObjects.push_back(new GameObjects::Shell());
	}

	ShellPool::~ShellPool()
	{
		for (int i = 0; i < m_InactiveObjects.size(); i++)
			delete m_InactiveObjects.at(i);

		for (int i = 0; i < m_ActiveObjects.size(); i++)
			delete m_ActiveObjects.at(i);
	}

	void ShellPool::Update(double deltatime)
	{
		for (int i = 0; i < m_ActiveObjects.size(); i++)
		{
			GameObjects::Shell* shell = m_ActiveObjects.at(i);
			shell->Update(deltatime);

			Vector2 position = m_ActiveObjects[i]->GetPosition();

			// Disable the shell if it's out of bounds
			if (!shell->IsEnabled() || position.x < 0.0f || position.x > GetScreenWidth() || position.y < 0.0f || position.y > GetScreenHeight())
				ReturnToPool(shell);
		}
	}

	void ShellPool::Draw()
	{
		for (int i = 0; i < m_ActiveObjects.size(); i++)
			m_ActiveObjects.at(i)->Draw();
	}

	GameObjects::Shell* ShellPool::GetObjectW()
	{
		if (!m_InactiveObjects.empty())
		{
			GameObjects::Shell* shell = m_InactiveObjects.back();
			m_ActiveObjects.push_back(shell);
			m_InactiveObjects.pop_back();
			return shell;
		}

		return nullptr;
	}

	void ShellPool::ReturnToPool(GameObjects::Shell* shell)
	{
		shell->SetEnabled(false);

		for (int i = 0; i < m_ActiveObjects.size(); i++)
			if (m_ActiveObjects[i] == shell)
			{
				m_ActiveObjects.erase(m_ActiveObjects.begin() + i);
				m_InactiveObjects.push_back(shell);
			}
	}

	void ShellPool::ReturnAllToPool()
	{
		for (int i = m_ActiveObjects.size() - 1; i >= 0; i--)
			ReturnToPool(m_ActiveObjects.at(i));
	}

	vector<GameObjects::Shell*> ShellPool::GetActiveObjects()
	{
		return m_ActiveObjects;
	}
	
	vector<GameObjects::Shell*> ShellPool::GetInactiveObjects()
	{
		return m_InactiveObjects;
	}
}