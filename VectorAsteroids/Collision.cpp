#include "Collision.h"

void Collision::Update(void)
{

	if (pPlayer->GetActive())
	{
		if (pPlayer->GetHit())
		{
			if (!pPlayer->GetExplosionOn())
			{
				if (SafeForPlayer())
				{
					pPlayer->Spawn();
				}
			}
		}
		else
		{
			CheckUFO(); //Collisions that do not involve the rocks, for player.
		}
	}
	else
	{
		bool shotsDone = true;
		
		for (int shot = 0; shot < 4; shot++)
		{
			if (pPlayer->GetShotActive(shot))
				shotsDone = false;
		}

		if (shotsDone)
			pHUD->SetGameOver(true);
	}

	CheckLargeRocks();
	CheckMedRocks();
	CheckSmallRocks();
	CheckPlayer();
	pHUD->Update();
}

void Collision::Draw()
{
	pHUD->Draw();
}

void Collision::NewGame(void)
{
	pHUD->NewGame();
	pPlayer->NewGame();
	pPlayer->Spawn();
	pRocks->NewGame();
	pUFOs->NewGame();
}

void Collision::RightArrowKey(bool KeyDown)
{
	pHUD->RightArrowKey(KeyDown);
}

void Collision::LeftArrowKey(bool KeyDown)
{
	pHUD->LeftArrowKey(KeyDown);
}

void Collision::DownArrowKey(bool KeyDown)
{
	pHUD->DownArrowKey(KeyDown);
}

void Collision::UpArrowKey(bool KeyDown)
{
	pHUD->UpArrowKey(KeyDown);
}

void Collision::FireKey(bool KeyDown)
{
	pHUD->FireKey(KeyDown);
}

Collision::Collision(Player *player, RockController *rocks, UFOController *UFOs)
{
	pPlayer = player;
	pRocks = rocks;
	pUFOs = UFOs;

	pHUD = new HUD();

	playerShotCount = pPlayer->GetNumberOfShots();
	playerShotRadius = pPlayer->GetShotRadius();
	UFOShotRadius = pUFOs->GetLargeShotRadius(0);
}


Collision::~Collision()
{
}

void Collision::CheckLargeRocks(void)
{
	//Check to see if something hit a Large rock -----------------------------------------------------------------
	int rockLargeCount = pRocks->GetLargeRockCount();
	int rockLargeRadius = pRocks->GetLargeRockRadius();

	for (int rock = 0; rock < rockLargeCount; rock++)
	{
		if (pRocks->GetLargeRockActive(rock))
		{
			Vector2i largeLocation = pRocks->GetLargeLocation(rock);

			//Does Player ship hit rock.
			if (pPlayer->GetActive() && !pPlayer->GetHit())
			{

				if (pPlayer->CirclesIntersect(largeLocation, rockLargeRadius))
				{
					pPlayer->Hit();

					if (pHUD->GetPlayerHit() < 1)
					{
						pPlayer->SetActive(false);
					}

					pHUD->SetPlayerScore(pRocks->GetLargeRockPoints());
					pRocks->LargeRockDistroyed(rock);
					break;
				}
			}

			//Does UFO run into rock.
			if (pUFOs->GetLUFOActive())
			{
				if (CirclesIntersect(pUFOs->GetLargeUFO(), largeLocation, pUFOs->GetLUFORadius(), rockLargeRadius))
				{
					pUFOs->LargeUFODistroyed();
					pRocks->LargeRockDistroyed(rock);
					break;
				}
			}

			if (pUFOs->GetSUFOActive())
			{
				if (CirclesIntersect(pUFOs->GetSmallUFO(), largeLocation, pUFOs->GetSUFORadius(), rockLargeRadius))
				{
					pUFOs->SmallUFODistroyed();
					pRocks->LargeRockDistroyed(rock);
					break;
				}
			}

			//Does Player Shot Larege Rock ---------------
			for (int shot = 0; shot < playerShotCount; shot++)
			{
				if (pPlayer->GetShotActive(shot))
				{
					if (CirclesIntersect(pPlayer->GetShotLocation(shot), largeLocation,
						playerShotRadius, rockLargeRadius))
					{
						pPlayer->SetShotActive(shot, false);
						pHUD->SetPlayerScore(pRocks->GetLargeRockPoints());
						pRocks->LargeRockDistroyed(rock);
						break;
					}
				}
			}

			//Does UFO Shot Large Rock -------------------
			for (int shot = 0; shot < 2; shot++)
			{
				if (pUFOs->GetLargeShotActive(shot))
				{
					if (CirclesIntersect(pUFOs->GetLargeShot(shot), largeLocation, UFOShotRadius, rockLargeRadius))
					{
						pUFOs->SetLUFOShotActive(false, shot);
						pRocks->LargeRockDistroyed(rock);
						break;
					}
				}
				if (pUFOs->GetSmallShotActive(shot))
				{
					if (CirclesIntersect(pUFOs->GetSmallShot(shot), largeLocation, UFOShotRadius, rockLargeRadius))
					{
						pUFOs->SetSUFOShotActive(false, shot);
						pRocks->LargeRockDistroyed(rock);
						break;
					}
				}
			}
		}
	}
}

void Collision::CheckMedRocks(void)
{
	//Check to see if something hit a Med rock -----------------------------------------------------------------
	int rockMedCount = pRocks->GetMedRockCount();
	int rockMedRadius = pRocks->GetMedRockRadius();

	for (int rock = 0; rock < rockMedCount; rock++)
	{
		if (pRocks->GetMedRockActive(rock))
		{
			Vector2i medLocation = pRocks->GetMedLocation(rock);

			//Does Player ship hit rock.
			if (pPlayer->GetActive() && !pPlayer->GetHit())
			{
				if (pPlayer->CirclesIntersect(medLocation, rockMedRadius))
				{
					pHUD->SetPlayerScore(pRocks->GetMedRockPoints());
					pPlayer->Hit();

					if (pHUD->GetPlayerHit() < 1)
					{
						pPlayer->SetActive(false);
					}

					pRocks->MedRockDistroyed(rock);
					break;
				}
			}

			//Does UFO run into Med rock.
			if (pUFOs->GetLUFOActive())
			{
				if (CirclesIntersect(pUFOs->GetLargeUFO(), medLocation, pUFOs->GetLUFORadius(), rockMedRadius))
				{
					pUFOs->LargeUFODistroyed();
					pRocks->MedRockDistroyed(rock);
					break;
				}
			}

			if (pUFOs->GetSUFOActive())
			{
				if (CirclesIntersect(pUFOs->GetSmallUFO(), medLocation, pUFOs->GetSUFORadius(), rockMedRadius))
				{
					pUFOs->SmallUFODistroyed();
					pRocks->MedRockDistroyed(rock);
					break;
				}
			}

			//Does Player Shot Med Rock ---------------
			for (int shot = 0; shot < playerShotCount; shot++)
			{
				if (pPlayer->GetShotActive(shot))
				{
					if (CirclesIntersect(pPlayer->GetShotLocation(shot), medLocation,
						playerShotRadius, rockMedRadius))
					{
						pRocks->MedRockDistroyed(rock);
						pHUD->SetPlayerScore(pRocks->GetMedRockPoints());
						pPlayer->SetShotActive(shot, false);
						//Do explosion particle FX based on rock radius
						break;
					}
				}
			}

			//Does UFO Shot Med Rock -------------------
			for (int shot = 0; shot < 2; shot++)
			{
				if (pUFOs->GetLargeShotActive(shot))
				{
					if (CirclesIntersect(pUFOs->GetLargeShot(shot), medLocation, UFOShotRadius, rockMedRadius))
					{
						pRocks->MedRockDistroyed(rock);
						pUFOs->SetLUFOShotActive(false, shot);
						break;
					}
				}

				if (pUFOs->GetSmallShotActive(shot))
				{
					if (CirclesIntersect(pUFOs->GetSmallShot(shot), medLocation, UFOShotRadius, rockMedRadius))
					{
						pRocks->MedRockDistroyed(rock);
						pUFOs->SetSUFOShotActive(false, shot);
						break;
					}
				}
			}
		}
	}
}

void Collision::CheckSmallRocks(void)
{
	//Check to see if something hit a small rock -----------------------------------------------------------------
	int rockSmallCount = pRocks->GetSmallRockCount();
	int rockSmallRadius = pRocks->GetSmallRockRadius();

	for (int rock = 0; rock < rockSmallCount; rock++)
	{
		if (pRocks->GetSmallRockActive(rock))
		{
			Vector2i smallLocation = pRocks->GetSmallLocation(rock);

			//Does Player ship hit rock.
			if (pPlayer->GetActive() && !pPlayer->GetHit())
			{
				if (pPlayer->CirclesIntersect(smallLocation, rockSmallRadius))
				{					
					pHUD->SetPlayerScore(pRocks->GetSmallRockPoints());
					pPlayer->Hit();

					if (pHUD->GetPlayerHit() < 1)
					{
						pPlayer->SetActive(false);
					}

					pRocks->SmallRockDistroyed(rock);
					CheckEndOfWave();
					break;
				}
			}

			//Does UFO run into rock.
			if (pUFOs->GetLUFOActive())
			{
				if (CirclesIntersect(pUFOs->GetLargeUFO(), smallLocation, pUFOs->GetLUFORadius(), rockSmallRadius))
				{
					pUFOs->LargeUFODistroyed();
					pRocks->SmallRockDistroyed(rock);
					CheckEndOfWave();
					break;
				}
			}

			if (pUFOs->GetSUFOActive())
			{
				if (CirclesIntersect(pUFOs->GetSmallUFO(), smallLocation, pUFOs->GetSUFORadius(), rockSmallRadius))
				{
					pUFOs->SmallUFODistroyed();
					pRocks->SmallRockDistroyed(rock);
					CheckEndOfWave();
					break;
				}
			}

			//Does Player Shot Small Rock ---------------
			for (int shot = 0; shot < playerShotCount; shot++)
			{
				if (pPlayer->GetShotActive(shot))
				{
					if (CirclesIntersect(pPlayer->GetShotLocation(shot), smallLocation,
						playerShotRadius, rockSmallRadius))
					{
						pPlayer->SetShotActive(shot, false);
						pRocks->SmallRockDistroyed(rock);
						pHUD->SetPlayerScore(pRocks->GetSmallRockPoints());
						CheckEndOfWave();
						break;
					}
				}
			}

			//Does UFO Shot Small Rock -------------------
			for (int shot = 0; shot < 2; shot++)
			{
				if (pUFOs->GetLargeShotActive(shot))
				{
					if (CirclesIntersect(pUFOs->GetLargeShot(shot), smallLocation, UFOShotRadius, rockSmallRadius))
					{
						pRocks->SmallRockDistroyed(rock);
						pUFOs->SetLUFOShotActive(false, shot);
						CheckEndOfWave();
						break;
					}
				}

				if (pUFOs->GetSmallShotActive(shot))
				{
					if (CirclesIntersect(pUFOs->GetSmallShot(shot), smallLocation, UFOShotRadius, rockSmallRadius))
					{
						pRocks->SmallRockDistroyed(rock);
						pUFOs->SetSUFOShotActive(false, shot);
						CheckEndOfWave();
						break;
					}
				}
			}
		}
	}
}

void Collision::CheckEndOfWave(void)
{
	if (pRocks->CheckEndOfWave())
	{
		pRocks->NewWave();
		pUFOs->SetWave(pHUD->NewWave());
	}
}

void Collision::CheckPlayer(void)
{
	Vector2i LUFOLocaiton = pUFOs->GetLargeUFO();
	Vector2i SUFOLocation = pUFOs->GetSmallUFO();
	int UFOLargeRadius = pUFOs->GetLUFORadius();
	int UFOSmallRadius = pUFOs->GetSUFORadius();

	//Does player shot UFO
	for (int shot = 0; shot < playerShotCount; shot++)
	{
		Vector2i ShotLocation = pPlayer->GetShotLocation(shot);

		if (pPlayer->GetShotActive(shot))
		{
			if (pUFOs->GetLUFOActive())
			{
				if (CirclesIntersect(ShotLocation, LUFOLocaiton, playerShotRadius, UFOLargeRadius))
				{
					pPlayer->SetShotActive(shot, false);
					pUFOs->LargeUFODistroyed();
					pHUD->SetPlayerScore(pUFOs->PlayerShotLargeUFO());
					break;
				}
			}
			else if (pUFOs->GetSUFOActive())
			{
				if (CirclesIntersect(ShotLocation, SUFOLocation, playerShotRadius, UFOSmallRadius))
				{
					pPlayer->SetShotActive(shot, false);
					pUFOs->SmallUFODistroyed();
					pHUD->SetPlayerScore(pUFOs->PlayerShotSmallUFO());
					break;
				}
			}
		}
	}
}

void Collision::CheckUFO(void)
{
	Vector2i LUFOLocaiton = pUFOs->GetLargeUFO();
	Vector2i SUFOLocation = pUFOs->GetSmallUFO();
	int UFOLargeRadius = pUFOs->GetLUFORadius();
	int UFOSmallRadius = pUFOs->GetSUFORadius();


	//Does UFO Shot player -------------------
	Vector2i playerLocation = pPlayer->GetLocation();
	for (int shot = 0; shot < 2; shot++)
	{
		if (pUFOs->GetLargeShotActive(shot))
		{
			if (pPlayer->CirclesIntersect(pUFOs->GetLargeShot(shot), UFOShotRadius))
			{
				pUFOs->SetLUFOShotActive(false, shot);
				pPlayer->Hit();

				if (pHUD->GetPlayerHit() < 1)
				{
					pPlayer->SetActive(false);
				}

				break;
			}
		}

		if (pUFOs->GetSmallShotActive(shot))
		{
			if (pPlayer->CirclesIntersect(pUFOs->GetSmallShot(shot), UFOShotRadius))
			{
				pUFOs->SetSUFOShotActive(false, shot);
				pPlayer->Hit();

				if (pHUD->GetPlayerHit() < 1)
				{
					pPlayer->SetActive(false);
				}

				break;
			}
		}
	}

	//Does UFO run into player
	if (pUFOs->GetLUFOActive())
	{
		if (pPlayer->CirclesIntersect(pUFOs->GetLargeUFO(), pUFOs->GetLUFORadius()))
		{
			pUFOs->LargeUFODistroyed();
			pHUD->SetPlayerScore(pUFOs->PlayerShotLargeUFO());
			pPlayer->Hit();

			if (pHUD->GetPlayerHit() < 1)
			{
				pPlayer->SetActive(false);
			}
		}
	}

	if (pUFOs->GetSUFOActive())
	{
		if (pPlayer->CirclesIntersect(pUFOs->GetSmallUFO(), pUFOs->GetSUFORadius()))
		{
			pUFOs->SmallUFODistroyed();
			pHUD->SetPlayerScore(pUFOs->PlayerShotSmallUFO());
			pPlayer->Hit();

			if (pHUD->GetPlayerHit() < 1)
			{
				pPlayer->SetActive(false);
			}
		}
	}
}

bool Collision::SafeForPlayer()
{
	int clearRadius = 100;

	for (int rock = 0; rock < pRocks->GetLargeRockCount(); rock++)
	{
		if (pRocks->GetLargeRockActive(rock))
		{
			if (CirclesIntersect(Vector2i(Window::GetWindowSize() / 2), pRocks->GetLargeLocation(rock), clearRadius, pRocks->GetLargeRockRadius()))
				return false;
		}
	}

	for (int rock = 0; rock < pRocks->GetMedRockCount(); rock++)
	{
		if (pRocks->GetMedRockActive(rock))
		{
			if (CirclesIntersect(Vector2i(Window::GetWindowSize() / 2), pRocks->GetMedLocation(rock), clearRadius, pRocks->GetMedRockRadius()))
				return false;
		}
	}

	for (int rock = 0; rock < pRocks->GetSmallRockCount(); rock++)
	{
		if (pRocks->GetSmallRockActive(rock))
		{
			if (CirclesIntersect(Vector2i(Window::GetWindowSize() / 2), pRocks->GetSmallLocation(rock), clearRadius, pRocks->GetSmallRockRadius()))
				return false;
		}
	}

	if (pUFOs->GetLUFOActive())
	{
		return false;
	}

	if (pUFOs->GetSUFOActive())
	{
		return false;
	}

	for (int shot = 0; shot < 2; shot++)
	{
		if (pUFOs->GetLargeShotActive(shot))
		{
			return false;
		}

		if (pUFOs->GetSmallShotActive(shot))
		{
			return false;
		}
	}

	return true;
}

bool Collision::CirclesIntersect(Vector2i locationOne, Vector2i locationTwo, int radiusOne, int radiusTwo)
{
	float distance = sqrt((locationOne.x - locationTwo.x) * (locationOne.x - locationTwo.x) +
		(locationOne.y - locationTwo.y) * (locationOne.y - locationTwo.y));

	if (distance < radiusOne + radiusTwo)
	{
		return true;
	}

	return false;
}