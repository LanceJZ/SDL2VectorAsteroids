#include "Dot.h"

void Dot::Draw()
{
	if (m_Active)
	{
		Window::DrawPoint(m_Location, 190, 0, 255, m_Alpha);
	}
}

void Dot::Update(double *Frame)
{
	if (m_Active)
	{
		if (m_Timer < pTimer->Seconds())
			Deactivate();

		UpdateFrame(Frame);

		if (m_Alpha > 1)
			m_Alpha -= 1;
	}
}

void Dot::Activate(Vector2i location, float Angle, int size)
{
	m_Active = true;
	m_Location = location + Vector2i(Random(0, size), Random(0, size));

	float sinRot = sin(Angle);
	float cosRot = cos(Angle);

	int maxV = 50;
	m_Velocity = Vector2f(cosRot * (float)Random(10, maxV), sinRot * (float)Random(10, maxV));
	m_Timer = pTimer->Seconds() + (float)Random(m_TimerAmount / 2, m_TimerAmount) * 0.01;
}

void Dot::Deactivate(void)
{
	m_Active = false;
}

void Dot::PauseTimer(void)
{
	pTimer->Pause();
}

void Dot::UnpauseTimer(void)
{
	pTimer->Unpause();
}

Dot::Dot(boost::random::mt19937 &random) : Entity(random)
{
	m_Active = false;
	m_TimerAmount = 100;
	pTimer = new Timer();
	pTimer->Start();
}

Dot::~Dot(void)
{
	delete pTimer;
}
