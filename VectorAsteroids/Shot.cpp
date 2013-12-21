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
		Vector2i location;

		Window::DrawPoint(&(Vector2i)m_Location, &m_InsideColor);
		location = m_Location + Vector2i(1, 0);
		Window::DrawPoint(&location, &m_OutsideColor);
		location = m_Location + Vector2i(0, 1);
		Window::DrawPoint(&location, &m_OutsideColor);
		location = m_Location + Vector2i(-1, 0);
		Window::DrawPoint(&location, &m_OutsideColor);
		location = m_Location + Vector2i(0, -1);
		Window::DrawPoint(&location, &m_OutsideColor);
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

	//Inside color
	m_InsideColor.Red = 170;
	m_InsideColor.Green = 150;
	m_InsideColor.Blue = 200;
	m_InsideColor.Alpha = 255;

	//180, 000, 185, 180
	m_OutsideColor.Red = 200;
	m_OutsideColor.Green = 50;
	m_OutsideColor.Blue = 185;
	m_OutsideColor.Alpha = 180;
}

Shot::~Shot(void)
{
	delete pTimer;
}
