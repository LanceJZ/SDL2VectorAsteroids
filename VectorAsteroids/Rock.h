#ifndef ROCK_H
#define ROCK_H
#include "Entity.h"
#include "Explosion.h"
#include "Window.h"

class Rock :
	public Entity
{
private:
	Color m_Color;
	Vector2i m_RockPoints[12];

	Explosion *pExplosion;
	Mix_Chunk *p_ExplosionSound = nullptr;

	void BuildRock(void);
	void DrawRock(void);

protected:
	bool m_Distroyed;
	int m_MaxVelocity;
	int m_RockLow;
	int m_RockMed;
	int m_RockHigh;
	// how much the rock varies.
	int m_RockVarienceHigh;
	int m_RockVarienceMed;
	int m_RockVarienceLow;

public:
	Rock(boost::random::mt19937 &random, Mix_Chunk *ExplosionSound);
	~Rock(void);

	virtual int PlayerShotRock(void);
	void Initialize(boost::random::mt19937 &random);
	void Update(double *frame);
	void Draw(void);
	void Activate(Vector2i location);
	bool GetDistroyed(void);
	void SetDistroyed(bool Distroyed);
	Vector2f GetLocation(void);
	float GetRadius(void);
	void Distroyed(void);
	void PauseTimer(void);
	void UnpauseTimer(void);
};

#endif