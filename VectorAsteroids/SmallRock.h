#ifndef SMALLROCK_H
#define SMALLROCK_H
#include "Rock.h"

class SmallRock :
	public Rock
{
public:
	SmallRock(boost::random::mt19937 &gen, Mix_Chunk *ExplosionSound);
	~SmallRock(void);

	int PlayerShotRock(void);
};

#endif