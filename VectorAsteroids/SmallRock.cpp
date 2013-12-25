#include "SmallRock.h"

int SmallRock::PlayerShotRock(void)
{
	return 100;
}

SmallRock::SmallRock(Mix_Chunk *ExplosionSound) : Rock(ExplosionSound)
{
	m_MaxVelocity = 90;
	m_RockLow = 0;
	m_RockMed = 7;
	m_RockHigh = 10;
	// how much the rock varies.
	m_RockVarienceHigh = 5;
	m_RockVarienceMed = 4;
	m_RockVarienceLow = 3;
	m_Radius = 9;
}

SmallRock::~SmallRock(void)
{
}