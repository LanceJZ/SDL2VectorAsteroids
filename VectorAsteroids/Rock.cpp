#include "Rock.h"

void Rock::Initialize(boost::random::mt19937 &random)
{
	m_Active = false;
	m_Distroyed = false;
	m_MaxVelocity = 0;
	m_RockLow = 0;
	m_RockMed = 0;
	m_RockHigh = 0;
	// how much the rock varies.
	m_RockVarienceHigh = 0;
	m_RockVarienceMed = 0;
	m_RockVarienceLow = 0;
	// Instance explosion.
	pExplosion = new Explosion(random);
}

void Rock::Update(double *frame)
{
	pExplosion->Update(frame);

	if (m_Active)
	{
		UpdateFrame(frame);
		CheckForEdge();
	}
}

void Rock::Draw(void)
{
	if (m_Active)
	{
		DrawRock();
	}

	pExplosion->Draw();
}

void Rock::SetDistroyed(bool Distroyed)
{
	m_Distroyed = Distroyed;
}

bool Rock::GetDistroyed(void)
{
	return m_Distroyed;
}

Vector2f Rock::GetLocation(void)
{
	return m_Location;
}

float Rock::GetRadius(void)
{
	return m_Radius;
}

int Rock::PlayerShotRock(void)
{
	return 0;
}

void Rock::Activate(Vector2i location)
{
	float angle = Random(0, 600) * 0.01;

	float sinRot = sin(angle);
	float cosRot = cos(angle);

	int speed = Random(m_MaxVelocity/4 ,m_MaxVelocity);

	m_Velocity.x = cosRot * speed;
	m_Velocity.y = sinRot * speed;

	m_Active = true;
	m_Location = location;

	BuildRock();
}

void Rock::PauseTimer(void)
{
	pExplosion->PauseTimer();
}

void Rock::UnpauseTimer(void)
{
	pExplosion->UnpauseTimer();
}

Rock::Rock(boost::random::mt19937 &random, Mix_Chunk *ExplosionSound) : Entity(random)
{
	p_ExplosionSound = ExplosionSound;

	//Rock Color.
	m_Color.Red = 100;
	m_Color.Green = 50;
	m_Color.Blue = 255;
	m_Color.Alpha = 255;

	Initialize(random);
}

Rock::~Rock(void)
{
	delete pExplosion;
}

//Private methods ----------------------------------------------------------------
void Rock::BuildRock(void)
{
	m_RockPoints[0].x = m_RockLow + Random(0, m_RockVarienceLow);
	m_RockPoints[0].y = -m_RockMed - Random(0, m_RockVarienceMed);

	m_RockPoints[1].x = m_RockMed + Random(0, m_RockVarienceMed);
	m_RockPoints[1].y = -m_RockHigh - Random(0, m_RockVarienceHigh);

	m_RockPoints[2].x = m_RockHigh + Random(0, m_RockVarienceHigh);
	m_RockPoints[2].y = -m_RockMed - Random(0, m_RockVarienceMed);

	m_RockPoints[3].x = m_RockMed + Random(0, m_RockVarienceMed);
	m_RockPoints[3].y = m_RockLow + Random(0, m_RockVarienceLow);

	m_RockPoints[4].x = m_RockHigh + Random(0, m_RockVarienceHigh);
	m_RockPoints[4].y = m_RockMed + Random(0, m_RockVarienceMed);

	m_RockPoints[5].x = m_RockMed + Random(0, m_RockVarienceMed);
	m_RockPoints[5].y = m_RockHigh + Random(0, m_RockVarienceHigh);

	m_RockPoints[6].x = m_RockLow + Random(0, m_RockVarienceLow);
	m_RockPoints[6].y = m_RockMed + Random(0, m_RockVarienceMed);

	m_RockPoints[7].x = -m_RockMed - Random(0, m_RockVarienceMed);
	m_RockPoints[7].y = m_RockHigh + Random(0, m_RockVarienceHigh);

	m_RockPoints[8].x = -m_RockHigh - Random(0, m_RockVarienceHigh);
	m_RockPoints[8].y = m_RockMed + Random(0, m_RockVarienceHigh);

	m_RockPoints[9].x = -m_RockMed - Random(0, m_RockVarienceMed);
	m_RockPoints[9].y = m_RockLow + Random(0, m_RockVarienceLow);

	m_RockPoints[10].x = -m_RockHigh - Random(0, m_RockVarienceHigh);
	m_RockPoints[10].y = -m_RockMed - Random(0, m_RockVarienceMed);

	m_RockPoints[11].x = -m_RockMed - Random(0, m_RockVarienceMed);
	m_RockPoints[11].y = -m_RockHigh - Random(0, m_RockVarienceHigh);
}

void Rock::DrawRock(void)
{
	Line rockLine;

	for (int point = 0; point < 11; point++)
	{
		rockLine.LineStart = m_RockPoints[point] + m_Location;
		rockLine.LineEnd = m_RockPoints[point + 1] + m_Location;

		Window::DrawLine(&rockLine , &m_Color);
	}

	rockLine.LineStart = m_RockPoints[11] + m_Location;
	rockLine.LineEnd = m_RockPoints[0] + m_Location;

	Window::DrawLine(&rockLine, &m_Color);
}

void Rock::Distroyed(void)
{
	//Play Rock explosion sound.
	Mix_PlayChannel(-1, p_ExplosionSound, 0);

	m_Active = false;
	m_Distroyed = true;
	pExplosion->Activate(m_Location, m_Radius);
}