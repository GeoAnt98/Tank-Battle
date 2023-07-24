#pragma once

#include "GameObjects/Headers/ObjectColors.h"

namespace GameObjects
{
	class Tank;
}

namespace ObjectPools
{
	class BarrelPool;
	class ShellPool;
	class BombPool;
	class JetPool;
}

namespace GameDev2D
{
	class Game : public Physics::WorldListener
	{
		const unsigned int MIN_JET_COOLDOWN = 190;
		const unsigned int MAX_JET_COOLDOWN = 330;

	public:
		Game();
		~Game();

		void Update(double deltatime);
		void Draw();

		void CreateTanks();
		void SetupBarrels();
		void ResetGame();
		void EndGame(GameObjects::Detonator winnercolor);

	private:
		bool m_IsGameOver;
		int m_BlueBarrel;
		int m_GreenBarrel;
		int m_JetCoolDown;
		int m_GameOverTimer;
		int m_BlueScore;
		int m_GreenScore;
		double m_TimeStep;

		ObjectPools::BarrelPool* m_BarrelPool;
		ObjectPools::ShellPool* m_ShellPool;
		ObjectPools::BombPool* m_BombPool;
		ObjectPools::JetPool* m_JetPool;

		GameObjects::Tank* m_BlueTank;
		GameObjects::Tank* m_GreenTank;

		Sprite* m_BackgroundSprite;
		Sprite* m_ScoreBackgroundSprite;
		Label* m_ScoreLabel;
		Label* m_WinnerLabel;

		bool CollisionCallBack(Physics::Body* bodyA, Physics::Body* bodyB);
	};
}
