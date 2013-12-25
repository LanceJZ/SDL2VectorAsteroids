#ifndef ROCKCONTROLLER_H
#define ROCKCONTROLLER_H
#include <vector>
#include <vmath.h>
#include "Common.h"
#include "LargeRock.h"
#include "MediumRock.h"
#include "SmallRock.h"

class RockController : Common
{
private:
	int m_NumberOfRocks;
	
	std::vector<LargeRock*> m_LargeRocks;
	std::vector<MediumRock*> m_MedRocks;
	std::vector<SmallRock*> m_SmallRocks;

	void CreateRocks(void);
	void ClearAllRocks(void);
	void SpawnNewWave(int NumberOfRocks);
	void SpawnMedRocks(Vector2i location);
	void SpawnSmallRocks(Vector2i location);

	Mix_Chunk *p_ExplosionSound = nullptr;

public:
	RockController(void);
	~RockController(void);

	void Update(double *Frame);
	void Draw(void);
	void Initialize(void);
	void NewGame(void);
	bool CheckEndOfWave(void);
	void NewWave(void);

	//Outside Rock Access
	//Large Rocks
	void LargeRockDistroyed(int rock);
	Vector2i GetLargeLocation(int rock);
	int GetLargeRockCount();
	int GetLargeRockRadius();
	bool GetLargeRockActive(int rock);
	int GetLargeRockPoints();
	//Medium Rocks
	void MedRockDistroyed(int rock);
	Vector2i GetMedLocation(int rock);
	int GetMedRockCount();
	int GetMedRockRadius();
	bool GetMedRockActive(int rock);
	int GetMedRockPoints();
	//Small Rocks
	Vector2i GetSmallLocation(int rock);
	void SmallRockDistroyed(int rock);
	int GetSmallRockCount();
	int GetSmallRockRadius();
	bool GetSmallRockActive(int rock);
	int GetSmallRockPoints();
};

#endif