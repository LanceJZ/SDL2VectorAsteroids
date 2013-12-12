#include "Shot.h"

void Shot::Update(double *Frame)
{
	if (m_Active)
	{
		UpdateFrame(Frame);
		CheckForEdge();

		if (pTimer->Seconds() > m_ShotTimer)
			m_Active = false;
	}
}

void Shot::Draw(void)
{
	if (m_Active)
	{		
		Window::DrawPoint(m_Location, 150, 80, 255, 255);
		Window::DrawPoint(m_Location + Vector2f(1,0), 180, 000, 185, 180);
		Window::DrawPoint(m_Location + Vector2f(0, 1), 180, 000, 185, 180);
		Window::DrawPoint(m_Location + Vector2f(-1, 0), 180, 000, 185, 180);
		Window::DrawPoint(m_Location + Vector2f(0, -1), 180, 000, 185, 180);
	}
}

float Shot::GetX(void)
{
	return m_Location.x;
}

float Shot::GetY(void)
{
	return m_Location.y;
}

Vector2i Shot::GetLocation(void)
{
	return m_Location;
}

float Shot::GetRadius(void)
{
	return m_Radius;
}

void Shot::Activate(Vector2f locaiton, float Angle)
{
	m_Active = true;
	m_Angle = Angle;

	float sinRot = sin(m_Angle);
	float cosRot = cos(m_Angle);

	m_Location.x = locaiton.x + cosRot * 15;
	m_Location.y = locaiton.y + sinRot * 15;

	m_Velocity.x = cosRot * m_Speed;
	m_Velocity.y = sinRot * m_Speed;

	m_ShotTimer = m_ShotTimeLimit + pTimer->Seconds();
}

void Shot::TimerPause(void)
{
	pTimer->Pause();
}

void Shot::TimerUnpause(void)
{
	pTimer->Unpause();
}

Shot::Shot(boost::random::mt19937 &random) : Entity(random)
{
	m_Active = false;
	m_Speed = 250;
	m_ShotTimer = 0;
	m_Angle = 0;
	m_ShotTimeLimit = 2.5;
	m_Radius = 2;

	pTimer = new Timer();
	pTimer->Start();
}

Shot::~Shot(void)
{
	delete pTimer;
}
