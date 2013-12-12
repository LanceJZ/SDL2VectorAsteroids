#include "PlayerShot.h"

PlayerShot::PlayerShot(boost::random::mt19937 &random) : Shot(random)
{
	m_ShotTimeLimit = 2.1;
	m_Speed = 300;
}

PlayerShot::~PlayerShot(void)
{
}

