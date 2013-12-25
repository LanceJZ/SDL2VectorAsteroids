#ifndef DOT_H
#define DOT_H
#include "entity.h"
#include "Timer.h"

class Dot :
	public Entity
{
private:
	float m_Timer;
	int m_TimerAmount;
	Timer *pTimer;
	float m_Alpha;
	Color m_Color;

public:
	Dot(void);
	~Dot(void);

	void Draw(void);
	void Update(double *Frame);
	void Activate(Vector2i locaiton, float Angle, int Size);
	void Deactivate(void);
	//Needs timer pause.
	void PauseTimer(void);
	void UnpauseTimer(void);
};

#endif