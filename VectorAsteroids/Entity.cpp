#include "Entity.h"

bool Entity::CirclesIntersect(Vector2i Target, float TargetRadius)
{
	float distance = sqrt((Target.x - m_Location.x) * (Target.x - m_Location.x) + 
		(Target.y - m_Location.y) * (Target.y - m_Location.y));

	if (distance < m_Radius + TargetRadius)
	{
		return true;
	}

	return false;
}

bool Entity::GetActive(void)
{
	return m_Active;
}

void Entity::SetActive(bool Active)
{
	m_Active = Active;
}

Entity::Entity(boost::random::mt19937 &random) : Common(random)
{
	m_Active = false;

	m_Rotation = 0;
	m_Radius = 0;

	m_Location = Vector2f();
	m_Velocity = Vector2f();
	m_Acceleration = Vector2f();
}

Entity::~Entity(void)
{
}

//Protected methods
void Entity::CheckForEdge(void)
{
	if (m_Location.x < 0)
		m_Location.x = Window::GetWindowSize().x - 1;

	if (m_Location.x > Window::GetWindowSize().x)
		m_Location.x = 0 + 1;

	if (m_Location.y < 0)
		m_Location.y = Window::GetWindowSize().y - 1;

	if (m_Location.y > Window::GetWindowSize().y)
		m_Location.y = 0 + 1;
}

void Entity::UpdateFrame(double *Frame)
{
	double frame;
	frame = *Frame;
	m_Velocity += m_Acceleration;	
	m_Location += (m_Velocity * frame);
}