#include "LargeRock.h"

int LargeRock::PlayerShotRock()
{
	return 20;	
}

LargeRock::LargeRock(boost::random::mt19937 &random, Mix_Chunk *ExplosionSound) : Rock(random, ExplosionSound)
{
	m_MaxVelocity = 35;
	m_RockLow = 0;
	m_RockMed = 35;
	m_RockHigh = 45;
	// how much the rock varies.
	m_RockVarienceHigh = 16;
	m_RockVarienceMed = 15;
	m_RockVarienceLow = 14;
	m_Radius = 50;
}

LargeRock::~LargeRock(void)
{
}