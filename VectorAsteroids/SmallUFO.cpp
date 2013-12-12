#include "SmallUFO.h"

void SmallUFO::Update(double *frame, Vector2i *playerLocation)
{
	if (m_Active)
	{
		DoesUFOShot();
		SetPlayerLocation(playerLocation);
	}

	UFO::Update(frame);
}

void SmallUFO::SetPlayerLocation(Vector2i *locaiton)
{
	m_PlayerLocation = *locaiton;
}

SmallUFO::SmallUFO(boost::random::mt19937 &Random, Mix_Chunk *Shotsound, Mix_Chunk *ExplosionSound) : UFO(Random, Shotsound, ExplosionSound)
{
	m_Width = 20;
	m_Radius = 10;
	m_MaxVelocity = 100;
	m_PlayerLocation = Vector2i(0, 0);
}

SmallUFO::~SmallUFO(void)
{
}

void SmallUFO::AimAtPlayer(void)
{
	FireShot(atan2(m_PlayerLocation.y - m_Location.y, m_PlayerLocation.x - m_Location.x));
	std::cout << "Small UFO aimed at player. ";
}

void SmallUFO::DoesUFOShot(void)
{
	if (m_TimerFireShot < pTimer->Seconds())
	{
		ResetShotTimer(0);

		int shotChance = (m_Spawned * 10) / (m_Wave * 2);

		if (Random(1, 100) > shotChance)
			FireShot(float(Random(0, 600)) * 0.01f);
		else
			AimAtPlayer();
	
		std::cout << "Small UFO shot chance. " << shotChance << " ";
	}		
}

int SmallUFO::PlayerShotUFO(void)
{
	return 1000;//m_PlayerReference.SetGotPoints(1000);
}

void SmallUFO::SetWaveNumber(int number)
{
	m_Wave = number;
}

void SmallUFO::SetSpawnedNumber(int number)
{
	m_Spawned = number;
}