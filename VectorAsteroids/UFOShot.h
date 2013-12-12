#ifndef UFOSHOT_H
#define UFOSHOT_H
#include "shot.h"

class UFOShot :
	public Shot
{
public:
	UFOShot(boost::random::mt19937 &random);
	~UFOShot(void);

};

#endif