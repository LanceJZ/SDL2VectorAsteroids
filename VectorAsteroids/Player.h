#ifndef PLAYER_H
#define PLAYER_H
#include "Entity.h"
#include "PlayerShot.h"
#include "Timer.h"

class Player : public Entity
{
private:
	bool m_Thrust;
	bool m_Fire;
	bool m_TurnRight;
	bool m_TurnLeft;
	bool m_Hit;
	bool m_ExplosionOn;
	int m_ShipWidth;
	int m_ShipHeight;
	int m_NumberOfShots;
	float m_MaxThrust;
	float m_ThrustMagnitude;
	float m_TurnRate;
	double m_ThrustDrawTimer;
	double m_ThrustDrawTimerAmount;
	double m_ExplosionTimer;
	double m_ExplosiontTimerAmount;
	Vector2i m_Front;
	Vector2i m_RSide;
	Vector2i m_LSide;
	Vector2i m_LInSide;
	Vector2i m_RInSide;
	Vector2i m_LFlame;
	Vector2i m_RFlame;
	Vector2i m_FlameTip;
	Vector2f m_ExLFront;
	Vector2f m_ExRFront;
	Vector2f m_ExRSide;
	Vector2f m_ExLSide;
	Vector2f m_ExLInside;
	Vector2f m_ExRInside;
	Vector2f m_ExMoveA;
	Vector2f m_ExMoveB;
	Vector2f m_ExMoveC;

	Mix_Chunk *p_Shotsound = nullptr;
	Mix_Chunk *p_Thrustsound = nullptr;
	Mix_Chunk *p_Explosionsound = nullptr;

	int m_ThrustChannel = -1;

	Timer *pTimer;
	PlayerShot *pShots[4];

	void UpdateShip(double *Frame);
	void UpdateExplosion(double *Frame);
	void UpdateShots(double *Frame);
	void DoRotationThrust(void);
	void InitializeShot(boost::random::mt19937 &generator);
	void DrawShots(void);
	void FireShot(void);
	void DrawThrust(void);
	void DrawExplosion(void);
	void SetExplosion(void);

public:
	Player(boost::random::mt19937 &generator);
	~Player(void);

	void Draw(void);
	void Update(double *frame);
	void NewGame(void);
	void Hit(void);
	void Spawn(void);
	void FireButtonPressed(void);
	void HyperSpaceButtonPressed(void);
	void SetLocation(Vector2f location);
	void SetLocationX(float X);
	void SetLocationY(float Y);
	void SetVelocity(Vector2f velocity);
	void SetVelocityX(float X);
	void SetVelocityY(float Y);
	void SetAcceleration(Vector2f acceleration);
	void SetAccelerationX(float X);
	void SetAccelerationY(float Y);
	void SetRotation(float Rotation);
	void SetThrust(bool Thrust);
	void SetTurnRight(bool TurnRight);
	void SetTurnLeft(bool TurnLeft);
	void SetShotActive(int Shot, bool Active);
	void SetGotPoints(int AddToScore);
	void SetNewWaveNumber(int Wave);
	void TimerPause(void);
	void TimerUnpause(void);
	Vector2f GetLocation(void);
	float GetLocationX(void);
	float GetLocationY(void);
	int GetNumberOfShots(void);
	float GetRadius(void);
	float GetShotRadius(void);
	Vector2i GetShotLocation(int Shot);
	float GetShotX(int Shot);
	float GetShotY(int Shot);
	bool GetShotActive(int Shot);
	bool GetHit();
	bool GetExplosionOn();
};
#endif