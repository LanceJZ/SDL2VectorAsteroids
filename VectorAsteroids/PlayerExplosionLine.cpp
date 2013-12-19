#include "PlayerExplosionLine.h"

void PlayerExplosionLine::Update(double *frame)
{
	UpdateFrame(frame);

	NewLine.LineStart = m_Location + Line.LineStart;
	NewLine.LineEnd = m_Location + Line.LineEnd;
}

void PlayerExplosionLine::Draw(void)
{
	Window::DrawLine(NewLine.LineStart, NewLine.LineEnd, 150, 0, 230, 255);
}

void PlayerExplosionLine::Initialize(ShipLine line, Vector2i location)
{
	Line = line;
	Line.LineStart -= location;
	Line.LineEnd -= location;
	m_Location = location;
	int max = 200;
	m_Velocity = Vector2f((Random(-max, max) * 0.1) - (Random(-max, max) * 0.1), (Random(-max, max) * 0.1) - (Random(-max, max) * 0.1));
}

PlayerExplosionLine::PlayerExplosionLine(boost::random::mt19937 &generator) : Entity(generator)
{
}


PlayerExplosionLine::~PlayerExplosionLine()
{
}