#ifndef COLLISION_H
#define COLLISION_H
#include "RockController.h"
#include "UFOController.h"
#include "Player.h"
#include "HUD.h"

class Collision
{
public:
	Collision(Player *player, RockController *rocks, UFOController *UFOs);
	~Collision();

	void Update(void);
	void Draw(void);
	void NewGame(void);
	void RightArrowKey(bool KeyDown);
	void LeftArrowKey(bool KeyDown);
	void DownArrowKey(bool KeyDown);
	void UpArrowKey(bool KeyDown);
	void FireKey(bool KeyDown);

private:
	Player *pPlayer = nullptr;
	RockController *pRocks = nullptr;
	UFOController *pUFOs = nullptr;
	HUD *pHUD = nullptr;

	bool CirclesIntersect(Vector2i locationOne, Vector2i locationTwo, int radiusOne, int radiusTwo);
	bool SafeForPlayer();
	int playerShotCount;
	int playerShotRadius;
	int UFOShotRadius;	

	void CheckLargeRocks(void);
	void CheckMedRocks(void);
	void CheckSmallRocks(void);
	void CheckEndOfWave(void);
	void CheckUFO(void);
	void CheckPlayer(void);
	void UFOShip(void);
	void UFOShot(void);
	void PlayerHit(void);
};

#endif