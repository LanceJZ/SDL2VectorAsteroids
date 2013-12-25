#include "PlayerExplosionLine.h"

void PlayerExplosionLine::Update(double *frame)
{
	UpdateFrame(frame);

	NewLine.LineStart = m_Location + ShipLine.LineStart;
	NewLine.LineEnd = m_Location + ShipLine.LineEnd;
}

void PlayerExplosionLine::Draw(void)
{
	Window::DrawLine(&NewLine, &m_Color);
}

void PlayerExplosionLine::Initialize(Line *ExpLine, Vector2i *Location)
{
	ShipLine = *ExpLine;
	ShipLine.LineStart -= *Location;
	ShipLine.LineEnd -= *Location;
	m_Location = *Location;
	int max = 200;
	m_Velocity = Vector2f((Window::Random(-max, max) * 0.1) - (Window::Random(-max, max) * 0.1),
		(Window::Random(-max, max) * 0.1) - (Window::Random(-max, max) * 0.1));
}

PlayerExplosionLine::PlayerExplosionLine() : Entity()
{
	m_Color.Red = 150;
	m_Color.Green = 0;
	m_Color.Blue = 230;
	m_Color.Alpha = 255;
}


PlayerExplosionLine::~PlayerExplosionLine()
{
}