#include "Game.h"
#include "ObjectPools/Headers/BarrelPool.h"
#include "ObjectPools/Headers/ShellPool.h"
#include "ObjectPools/Headers/BombPool.h"
#include "ObjectPools/Headers/JetPool.h"
#include "GameObjects/Headers/Barrel.h"
#include "GameObjects/Headers/Shell.h"
#include "GameObjects/Headers/Jet.h"
#include "GameObjects/Headers/Tank.h"

namespace GameDev2D
{
	Game::Game() :
		m_IsGameOver(false),
		m_GameOverTimer(-1),
		m_JetCoolDown(rand() % (MIN_JET_COOLDOWN, MAX_JET_COOLDOWN)),
		m_TimeStep(1.0f / (double)TARGET_FPS),
		m_BlueScore(0),
		m_GreenScore(0),
		m_BarrelPool(nullptr),
		m_ShellPool(nullptr),
		m_BombPool(nullptr),
		m_JetPool(nullptr),
		m_BackgroundSprite(nullptr),
		m_ScoreLabel(nullptr),
		m_WinnerLabel(nullptr)
	{
		LoadTexture("SpriteAtlas");
		LoadTexture("Jet");
		LoadTexture("Background");
		LoadTexture("ScoreBackground");
		LoadFont("heavy_Data", "ttf", 60);

		m_BarrelPool = new BarrelPool();
		m_ShellPool = new ShellPool();
		m_BombPool = new BombPool();
		m_JetPool = new JetPool(m_BombPool);

		// Create the battlefield background
		m_BackgroundSprite = new Sprite(GetTexture("Background"));
		m_BackgroundSprite->SetPosition(Vector2(GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f));
		m_BackgroundSprite->SetScale(Vector2(0.9f, 0.9f));
		m_BackgroundSprite->SetAnchor(Vector2(0.5f, 0.5f));

		m_ScoreBackgroundSprite = new Sprite(GetTexture("ScoreBackground"));
		m_ScoreBackgroundSprite->SetPosition(Vector2(GetScreenWidth() * 0.5f, GetScreenHeight() * 0.935f));
		m_ScoreBackgroundSprite->SetScale(Vector2(1.11f, 1.0f));
		m_ScoreBackgroundSprite->SetAnchor(Vector2(0.5f, 0.5f));

		m_ScoreLabel = new Label(GetFont("heavy_data", "ttf", 18));
		m_ScoreLabel->SetPosition(Vector2(GetScreenWidth() * 0.5f, GetScreenHeight() * 0.9725f));
		m_ScoreLabel->SetAnchor(Vector2(0.5f, 1.0f));
		m_ScoreLabel->SetJustification(Label::JustifyLeft);
		m_ScoreLabel->SetColor(Color::GrayColor());
		m_ScoreLabel->SetText("Blue: " + to_string(m_BlueScore) + "\nGreen: " + to_string(m_GreenScore));

		m_WinnerLabel = new Label(GetFont("heavy_data", "ttf", 36));
		m_WinnerLabel->SetPosition(Vector2(GetScreenWidth() / 2, GetScreenHeight() / 2));
		m_WinnerLabel->SetAnchor(Vector2(0.5f, 0.5f));
		m_WinnerLabel->SetJustification(Label::JustifyCenter);
		m_WinnerLabel->SetColor(Color::LimeColor());

		CreateTanks();
		SetupBarrels();

		// Set the Physics world listener
		Physics::World::GetInstance()->SetListener(this);
	}

	Game::~Game()
	{
		UnloadTexture("SpriteAtlas");
		UnloadTexture("Jet");
		UnloadTexture("Background");
		UnloadTexture("ScoreBackground");
		UnloadFont("heavy_Data", "ttf", 60);

		if (m_BarrelPool != nullptr)
		{
			delete m_BarrelPool;
			m_BarrelPool = nullptr;
		}

		if (m_ShellPool != nullptr)
		{
			delete m_ShellPool;
			m_ShellPool = nullptr;
		}

		if (m_BombPool != nullptr)
		{
			delete m_BombPool;
			m_BombPool = nullptr;
		}

		if (m_JetPool != nullptr)
		{
			delete m_JetPool;
			m_JetPool = nullptr;
		}

		if (m_BlueTank != nullptr)
		{
			delete m_BlueTank;
			m_BlueTank = nullptr;
		}

		if (m_GreenTank != nullptr)
		{
			delete m_GreenTank;
			m_GreenTank = nullptr;
		}

		if (m_BackgroundSprite != nullptr)
		{
			delete m_BackgroundSprite;
			m_BackgroundSprite = nullptr;
		}

		if (m_ScoreBackgroundSprite != nullptr)
		{
			delete m_ScoreBackgroundSprite;
			m_ScoreBackgroundSprite = nullptr;
		}

		if (m_ScoreLabel != nullptr)
		{
			delete m_ScoreLabel;
			m_ScoreLabel = nullptr;
		}

		if (m_WinnerLabel != nullptr)
		{
			delete m_WinnerLabel;
			m_WinnerLabel = nullptr;
		}
	}

	void Game::Update(double deltatime)
	{
		m_JetCoolDown -= deltatime;

		if (m_IsGameOver)
		{
			m_GameOverTimer--;

			if (m_GameOverTimer <= 0)
				ResetGame();
		}
		else if (!m_IsGameOver && m_JetCoolDown <= 0)
		{
			GameObjects::Jet* jet = m_JetPool->GetObjectW();

			if (jet != nullptr)
			{
				m_JetCoolDown = rand() % (MIN_JET_COOLDOWN, MAX_JET_COOLDOWN);
				jet->StartRun();
			}
		}

		m_BarrelPool->Update(deltatime);
		m_ShellPool->Update(deltatime);
		m_BombPool->Update(deltatime);
		m_JetPool->Update(deltatime);
		m_BlueTank->Update(deltatime);
		m_GreenTank->Update(deltatime);

		Physics::World::GetInstance()->Step(m_TimeStep);
	}

	void Game::Draw()
	{
		m_BackgroundSprite->Draw();
		m_BarrelPool->Draw();
		m_BlueTank->Draw();
		m_GreenTank->Draw();
		m_ShellPool->Draw();
		m_BombPool->Draw();
		m_JetPool->Draw();

		if (m_IsGameOver)
		{
			m_WinnerLabel->Draw();
			m_ScoreBackgroundSprite->Draw();
			m_ScoreLabel->Draw();
		}

		//Physics::World::GetInstance()->DebugDraw();
	}

	void Game::ResetGame()
	{
		m_IsGameOver = false;
		m_GameOverTimer = -1;

		m_BarrelPool->ReturnAllToPool();
		m_ShellPool->ReturnAllToPool();
		m_JetPool->ReturnAllToPool();

		m_BlueTank->Reset();
		m_GreenTank->Reset();

		SetupBarrels();
	}

	void Game::CreateTanks()
	{
		m_BlueTank = new GameObjects::Tank(m_ShellPool, GameObjects::TankColor::BlueTank);
		m_BlueTank->SetUpInput(Keyboard::W);
		m_BlueTank->SetDownInput(Keyboard::S);
		m_BlueTank->SetLeftInput(Keyboard::A);
		m_BlueTank->SetRightInput(Keyboard::D);
		m_BlueTank->SetTurretLeftInput(Keyboard::Q);
		m_BlueTank->SetTurretRightInput(Keyboard::E);
		m_BlueTank->SetFireTurretInput(Keyboard::Tab);

		m_GreenTank = new GameObjects::Tank(m_ShellPool, GameObjects::TankColor::GreenTank);
		m_GreenTank->SetUpInput(Keyboard::Up);
		m_GreenTank->SetDownInput(Keyboard::Down);
		m_GreenTank->SetLeftInput(Keyboard::Left);
		m_GreenTank->SetRightInput(Keyboard::Right);
		m_GreenTank->SetTurretLeftInput(Keyboard::N);
		m_GreenTank->SetTurretRightInput(Keyboard::M);
		m_GreenTank->SetFireTurretInput(Keyboard::Spacebar);
	}

	void Game::SetupBarrels()
	{
		bool alternaterow = false;
		int barrelamount = 6;

		for (int row = 0; row < 9; row++)
		{
			for (int column = 0; column < barrelamount; column++)
			{
				// Blacklisted rows and columns reserved for tank spawns
				if ((column == 2 || column == 3) && (row == 0 || row == 8)
					|| (column >= 2 && column <= 4) && (row == 1 || row == 7))
					continue;

				GameObjects::Barrel* barrel = m_BarrelPool->GetObjectW();

				if (barrel == nullptr) break;;

				if (alternaterow)
					barrel->Reset(Vector2(m_BackgroundSprite->GetWidth() * 0.1f + (120 * column), m_BackgroundSprite->GetHeight() * 0.14 + (60 * row)));
				else
					barrel->Reset(Vector2(m_BackgroundSprite->GetWidth() * 0.14f + (120 * column), m_BackgroundSprite->GetHeight() * 0.14 + (60 * row)));
			}
			alternaterow = !alternaterow;

			if (alternaterow)
				barrelamount = 7;
			else
				barrelamount = 6;
		}

		vector<GameObjects::Barrel*> barrels = m_BarrelPool->GetActiveObjects();

		int bluebarrel = Math::Random(0, barrels.size() - 1);
		int greenbarrel = Math::Random(0, barrels.size() - 1);

		while (bluebarrel == greenbarrel)
			greenbarrel = Math::Random(0, barrels.size() - 1);

		barrels.at(bluebarrel)->SetDetonator(GameObjects::Detonator::Blue);
		barrels.at(greenbarrel)->SetDetonator(GameObjects::Detonator::Green);
	}

	void Game::EndGame(GameObjects::Detonator winnercolor)
	{
		m_IsGameOver = true;
		m_GameOverTimer = 400;

		// Display the winner text label
		switch (winnercolor)
		{
		case GameObjects::Detonator::Blue:
			m_GreenTank->Explode();
			Services::GetGraphics()->GetCamera()->Shake(2.0f, 1.0f);
			m_WinnerLabel->SetColor(Color::CornflowerBlueColor());
			m_WinnerLabel->SetText("Blue Tank Wins!\nStarting a new round soon!\n");
			m_BlueScore++;
			break;
		case GameObjects::Detonator::Green:
			m_BlueTank->Explode();
			Services::GetGraphics()->GetCamera()->Shake(2.0f, 1.0f);
			m_WinnerLabel->SetColor(Color::LimeColor());
			m_WinnerLabel->SetText("Green Tank Wins!\nStarting a new round soon!\n");
			m_GreenScore++;
			break;
		default:
			break;
		}

		m_ScoreLabel->SetText("Blue: " + to_string(m_BlueScore) + "\nGreen: " + to_string(m_GreenScore));
	}

	bool Game::CollisionCallBack(Physics::Body* bodyA, Physics::Body* bodyB)
	{
		bool bluetank = (bodyA->GetGameObject() == m_BlueTank || bodyB->GetGameObject() == m_BlueTank);
		bool greentank = (bodyA->GetGameObject() == m_GreenTank || bodyB->GetGameObject() == m_GreenTank);
		vector<GameObjects::Barrel*> barrels = m_BarrelPool->GetActiveObjects();
		vector<GameObjects::Shell*> shells = m_ShellPool->GetActiveObjects();
		vector<GameObjects::Jet*> jets = m_JetPool->GetActiveObjects();

		// Jets on tank
		for (int j = 0; j < jets.size(); j++)
		{
			if (bodyA->GetGameObject() == jets.at(j) || bodyB->GetGameObject() == jets.at(j))
			{
				if (bluetank)
					m_BlueTank->Disable();
				if (greentank)
					m_GreenTank->Disable();
			}
		}

		for (int b = 0; b < barrels.size(); b++)
		{
			GameObjects::Barrel* barrel = barrels.at(b);

			// Tank on detonator collision
			if ((bluetank || greentank) && bodyA->GetGameObject() == barrel || bodyB->GetGameObject() == barrel)
			{
				if (barrel->GetDetonator() == GameObjects::Detonator::None && !barrel->IsExploded())
					return true;
				else if (!m_IsGameOver && barrel->IsExploded())
				{
					if (bluetank && barrel->GetDetonator() == GameObjects::Detonator::Green)
					{
						barrel->SetDetonator(GameObjects::Detonator::None);
						EndGame(GameObjects::Detonator::Blue);
					}
					else if (greentank && barrel->GetDetonator() == GameObjects::Detonator::Blue)
					{
						barrel->SetDetonator(GameObjects::Detonator::None);
						EndGame(GameObjects::Detonator::Green);
					}
					return false;
				}
			}

			// Shell on barrel collision
			for (int s = 0; s < shells.size(); s++)
			{
				GameObjects::Shell* shell = shells.at(s);

				if (shell->IsEnabled() && !barrel->IsExploded())
				{
					if ((bodyA->GetGameObject() == shell || bodyB->GetGameObject() == shell)
						&& (bodyA->GetGameObject() == barrel || bodyB->GetGameObject() == barrel))
					{
						shell->SetEnabled(false);
						barrel->Explode();
						Services::GetGraphics()->GetCamera()->Shake(3.0f, 0.5f);
						return true;
					}
				}
			}

			// Jet on barrel collision
			for (int j = 0; j < jets.size(); j++)
				if ((bodyA->GetGameObject() == jets.at(j) || bodyB->GetGameObject() == jets.at(j)) &&
					(bodyA->GetGameObject() == barrel || bodyB->GetGameObject() == barrel))
					return true;

			// Barrel on barrel collision
			if (bodyA->GetGameObject() == barrel || bodyB->GetGameObject() == barrel)
				return true;
		}

		return false;
	}

}