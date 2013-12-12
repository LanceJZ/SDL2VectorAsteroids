#ifndef ENTITY_H
#define ENTITY_H
#include <vmath.h>
#include "Window.h"
#include "Common.h"

class Entity : public Common
{
private:

protected:
	bool m_Active;

	Vector2f m_Location;
	Vector2f m_Velocity;
	Vector2f m_Acceleration;
	float m_Rotation;
	float m_Radius;

	void CheckForEdge(void);
	void UpdateFrame(double *Frame);

public:
	Entity(boost::random::mt19937 &random);
	~Entity(void);

	bool CirclesIntersect(Vector2i Target, float TargetRadius);
	bool GetActive(void);
	void SetActive(bool Active);
};

#endif