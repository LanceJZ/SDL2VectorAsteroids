#ifndef LARGEUFO_H
#define LARGEUFO_H
#include "UFO.h"
#include "Entity.h"

class LargeUFO :
	public UFO
{
private:
	void DoesUFOShot(void);

public:
	LargeUFO(boost::random::mt19937 &random, Mix_Chunk *Shotsound, Mix_Chunk *ExplosionSound);
	~LargeUFO(void);

	void Update(double *frame);
	int PlayerShotUFO(void);
};

#endif