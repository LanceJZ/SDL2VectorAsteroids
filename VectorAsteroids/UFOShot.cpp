#include "UFOShot.h"

UFOShot::UFOShot(boost::random::mt19937 &random) : Shot(random)
{
	m_ShotTimeLimit = 1.5;
}

UFOShot::~UFOShot(void)
{
}