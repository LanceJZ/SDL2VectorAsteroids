#ifndef UFOCONTROLLER_H
#define UFOCONTROLLER_H
#include "Common.h"
#include "LargeUFO.h"
#include "SmallUFO.h"
#include "Timer.h"

class UFOController : public Common
{
private:
	Mix_Chunk *p_LargeEngineSound;
	Mix_Chunk *p_SmallEngineSound;

	LargeUFO *pLargeUFO;
	SmallUFO *pSmallUFO;
	Timer *pTimer;

	float m_TimerSpawn;
	float m_TimerSpawnAmount;
	int m_SpawnCounter = 1;
	int m_WaveCounter = 1;
	int m_UFOSoundChannel = -1;
	bool m_UFOActive;
	Vector2i m_PlayerLocation;

	void SpawnLargeUFO(void);
	void SpawnSmallUFO(void);
	void DoesUFOSpawn(void);
	void PassPlayerLocation(void);

public:
	UFOController(void);
	~UFOController(void);

	bool GetSmallShotActive(int Shot);
	bool GetLargeShotActive(int Shot);
	float GetLargeShotRadius(int Shot);
	float GetSmallShotRadius(int Shot);
	bool GetLUFOActive(void);
	bool GetSUFOActive(void);
	float GetLUFORadius(void);
	float GetSUFORadius(void);
	void LargeUFODistroyed(void);
	void SmallUFODistroyed(void);
	Vector2i GetLargeUFO(void);
	Vector2i GetSmallUFO(void);
	Vector2i GetLargeShot(int Shot);
	Vector2i GetSmallShot(int Shot);

	void Update(double *frame, Vector2i *playerLocation);
	void Draw(void);
	void ResetTimer(void);
	void NewGame(void);
	void SetLUFOActive(bool Active);
	void SetSUFOActive(bool Active);
	void SetLUFOShotActive(bool Active, int Shot);
	void SetSUFOShotActive(bool Active, int Shot);
	void SetWave(int number);
	void PauseTimer(void);
	void UnpauseTimer(void);
	int PlayerShotLargeUFO(void);
	int PlayerShotSmallUFO(void);	
};

#endif