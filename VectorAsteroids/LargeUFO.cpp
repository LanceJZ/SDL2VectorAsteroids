#include "LargeUFO.h"

void LargeUFO::Update(double *frame)
{
	if (m_Active)
	{
		DoesUFOShot();
	}

	UFO::Update(frame);
}

void LargeUFO::DoesUFOShot(void)
{
	if (m_TimerFireShot < pTimer->Seconds())
	{
		ResetShotTimer(0);

		FireShot(float(Window::Random(0, 600)) * 0.01f);
	}
}

LargeUFO::LargeUFO(Mix_Chunk *Shotsound, Mix_Chunk *ExplosionSound) : UFO(Shotsound, ExplosionSound)
{
	m_Width = 40;
	m_Radius = 22;
	m_MaxVelocity = 80;
}

LargeUFO::~LargeUFO(void)
{
}

int LargeUFO::PlayerShotUFO(void)
{
	return 200;
}