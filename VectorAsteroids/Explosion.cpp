#include "Explosion.h"

void Explosion::Draw()
{
	if (m_Active)
	{
		for (int dot = 0; dot < 12; dot++)
		{
			pDots[dot]->Draw();
		}
	}
}

void Explosion::Update(double *Frame)
{
	if (m_Active)
	{
		for (int dot = 0; dot < 12; dot++)
		{
			pDots[dot]->Update(Frame);
		}
	}
}

void Explosion::Activate(Vector2i location, int Size)
{
	m_Active = true;

	float angle = 0;

	for (int dot = 0; dot < 12; dot++)
	{
		angle += (rand() % 15) * 0.1;
		pDots[dot]->Activate(location, angle, Size);
	}
}

void Explosion::PauseTimer(void)
{
	for (int dot = 0; dot < 12; dot++)
	{
		pDots[dot]->PauseTimer();
	}
}

void Explosion::UnpauseTimer(void)
{
	for (int dot = 0; dot < 12; dot++)
	{
		pDots[dot]->UnpauseTimer();
	}
}

Explosion::Explosion(boost::random::mt19937 &random)
{
	m_Active = false;

	for (int dot = 0; dot < 12; dot++)
	{
		pDots[dot] = new Dot(random);
	}
}

Explosion::~Explosion(void)
{
	delete pDots;
}