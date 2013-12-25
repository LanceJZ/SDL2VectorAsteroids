#ifndef LARGEROCK_H
#define LARGEROCK_H
#include "Rock.h"

class LargeRock :
	public Rock
{
public:
	LargeRock(Mix_Chunk *ExplosionSound);
	~LargeRock(void);

	int PlayerShotRock(void);
};

#endif