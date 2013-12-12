#ifndef LARGEROCK_H
#define LARGEROCK_H
#include "Rock.h"

class LargeRock :
	public Rock
{
public:
	LargeRock(boost::random::mt19937 &random, Mix_Chunk *ExplosionSound);
	~LargeRock(void);

	int PlayerShotRock(void);
};

#endif