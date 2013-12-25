#include "Dot.h"

void Dot::Draw()
{
	if (m_Active)
	{
		Window::DrawPoint(&(Vector2i)m_Location, &m_Color);
	}
}

void Dot::Update(double *Frame)
{
	if (m_Active)
	{
		if (m_Timer < pTimer->Seconds())
			Deactivate();

		UpdateFrame(Frame);

		if (m_Color.Alpha > 1)
			m_Color.Alpha -= 1;
	}
}

void Dot::Activate(Vector2i location, float Angle, int size)
{
	m_Active = true;
	m_Location = location + Vector2i(Window::Random(0, size), Window::Random(0, size));

	float sinRot = sin(Angle);
	float cosRot = cos(Angle);

	int maxV = 50;
	m_Velocity = Vector2f(cosRot * (float)Window::Random(10, maxV), sinRot * (float)Window::Random(10, maxV));
	m_Timer = pTimer->Seconds() + (float)Window::Random(m_TimerAmount / 2, m_TimerAmount) * 0.01;

	m_Color.Alpha = 255;
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

Dot::Dot() : Entity()
{
	m_Active = false;
	m_TimerAmount = 100;
	pTimer = new Timer();
	pTimer->Start();

	m_Color.Red = 190;
	m_Color.Green = 0;
	m_Color.Blue = 255;
	m_Color.Alpha = 255;
}

Dot::~Dot(void)
{
	delete pTimer;
}
