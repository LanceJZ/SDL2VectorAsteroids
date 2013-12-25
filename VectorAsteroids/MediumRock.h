#ifndef MEDIUMROCK_H
#define MEDIUMROCK_H
#include "Rock.h"

class MediumRock :
	public Rock
{
public:
	MediumRock(Mix_Chunk *ExplosionSound);
	~MediumRock(void);

	int PlayerShotRock(void);
};

#endif