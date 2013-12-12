#ifndef PLAYERSHOT_H
#define PLAYERSHOT_H
#include "shot.h"

class PlayerShot :
	public Shot
{
public:
	PlayerShot(boost::random::mt19937 &random);
	~PlayerShot(void);
};

#endif