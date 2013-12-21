#ifndef SHOT_H
#define SHOT_H
#include "Entity.h"
#include "Window.h"
#include "Timer.h"

class Shot :
	public Entity
{
private:
	float m_ShotTimer;
	Timer *pTimer;
	Color m_InsideColor;
	Color m_OutsideColor;

protected:
	float m_Angle;
	float m_ShotTimeLimit;
	float m_Speed;

public:
	Shot(boost::random::mt19937 &random);
	~Shot(void);

	void Update(double *frame);
	void Draw(void);
	void Activate(Vector2f location, float Angle);
	void TimerPause(void);
	void TimerUnpause(void);
	Vector2i GetLocation(void);
	float GetX(void);
	float GetY(void);
	float GetRadius(void);
};

#endif