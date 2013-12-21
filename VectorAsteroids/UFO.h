#ifndef UFO_H
#define UFO_H
#include "Entity.h"
#include "UFOShot.h"
#include "Explosion.h"
#include "Timer.h"

class UFO :	public Entity
{
private:
	Mix_Chunk *p_ShotSound = nullptr;
	Mix_Chunk *p_ExplosionSound = nullptr;

	Explosion *pExplosion = nullptr;
	UFOShot *pShots[2];

	Line UFOLines[10];
	Color m_Color;

	int m_NumberOfShots;
	float m_TimerChangeVector;
	bool m_ResetTimer;
	bool m_FromLeft;
	bool m_FromRight;
	bool m_Distroyed;
	float m_HLTopLX;
	float m_HLTopRX;
	float m_HLTopY;
	float m_HLLowerTopLX;
	float m_HLLowerTopRX;
	float m_HLLowerTopY;
	float m_HLUpperBottomLX;
	float m_HLUpperBottomRX;
	float m_HLUpperBottomY;
	float m_HLBottomRX;
	float m_HLBottomLX;
	float m_HLBottomY;

	void DoesUFOChangeVector(void);
	void ChangeVector(void);
	void BuildUFO(void);
	void DrawUFO(void);
	void CheckForSide(void);

protected:
	Timer *pTimer;

	float m_TimerAmountChangeVector;
	float m_TimerAmountFireShot;
	float m_TimerFireShot;
	int m_MaxVelocity;
	int m_Width;

	void FireShot(float angle);
	void ResetShotTimer(float TimerAmount);

public:
	UFO(boost::random::mt19937 &random, Mix_Chunk *Shotsound, Mix_Chunk *ExplosionSound);
	~UFO(void);

	void virtual Update(double *frame);
	void Draw(void);
	void Activate(int Y);
	void Deactivate(void);
	void NewGame(void);
	void TimerReset(void);
	void SetUFOShotActive(bool Active, int Shot);
	bool GetResetTimer(void);
	bool GetActive(void);
	bool GetShotActive(int Shot);
	Vector2i GetShotLocation(int shot);
	Vector2i GetLocation();
	float GetRadius(void);
	float GetShotRadius(int Shot);
	bool GetDistroyed(void);
	void GotDistroyed(void);
	void TimerPause(void);
	void TimerUnpause(void);
};

#endif