#ifndef SMALLUFO_H
#define SMALLUFO_H
#include "UFO.h"

class SmallUFO :
	public UFO
{
private:
	Vector2i m_PlayerLocation;
	int m_Wave = 1; //How many waves has it been.
	int m_Spawned = 1; //How many UFOs have been spaned.

	void AimAtPlayer(void);
	void DoesUFOShot(void);
	void SetPlayerLocation(Vector2i *location);

protected:
	void DoesPlayerShotUFO(void);

public:
	SmallUFO(boost::random::mt19937 &random, Mix_Chunk *Shotsound, Mix_Chunk *ExplosionSound);
	~SmallUFO(void);

	void Update(double *frame, Vector2i *playerLocation);
	int PlayerShotUFO(void);
	void SetWaveNumber(int number);
	void SetSpawnedNumber(int number);
};

#endif