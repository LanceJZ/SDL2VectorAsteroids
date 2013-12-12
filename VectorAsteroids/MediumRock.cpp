#include "MediumRock.h"

int MediumRock::PlayerShotRock()
{
	return 50;
}

MediumRock::MediumRock(boost::random::mt19937 &random, Mix_Chunk *ExplosionSound) : Rock(random, ExplosionSound)
{
	m_MaxVelocity = 60;
	m_RockLow = 0;
	m_RockMed = 15;
	m_RockHigh = 20;
	// how much the rock varies.
	m_RockVarienceHigh = 7;
	m_RockVarienceMed = 6;
	m_RockVarienceLow = 5;
	m_Radius = 18;
}

MediumRock::~MediumRock(void)
{
}