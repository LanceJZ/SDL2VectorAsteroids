#include "RockController.h"

void RockController::Update(double *Frame)
{
	for (int rock = 0; rock < m_LargeRocks.size(); rock++)
	{
		m_LargeRocks[rock]->Update(Frame);
	}

	for (int rock = 0; rock < m_MedRocks.size(); rock++)
	{
		m_MedRocks[rock]->Update(Frame);
	}

	for (int rock = 0; rock < m_SmallRocks.size(); rock++)
	{
		m_SmallRocks[rock]->Update(Frame);
	}
}

void RockController::Draw(void)
{
	for (int rock = 0; rock < m_LargeRocks.size(); rock++)
	{
		m_LargeRocks[rock]->Draw();
	}

	for (int rock = 0; rock < m_MedRocks.size(); rock++)
	{
		m_MedRocks[rock]->Draw();
	}

	for (int rock = 0; rock < m_SmallRocks.size(); rock++)
	{
		m_SmallRocks[rock]->Draw();
	}
}

void RockController::Initialize(void)
{
	ClearAllRocks();
}

void RockController::SpawnNewWave(int NumberOfRocks)
{	
	for (int rock = 0; rock < NumberOfRocks; rock++)
	{
		bool spawnnewrock = true;

		for (int rockcheck = 0; rockcheck < m_LargeRocks.size(); rockcheck++)
		{
			if (!m_LargeRocks[rockcheck]->GetActive())
			{
				spawnnewrock = false;
				m_LargeRocks[rockcheck]->Activate(Vector2i(0, GetRandomY()));
				break;
			}
		}

		if (spawnnewrock)
		{
			m_LargeRocks.push_back(new LargeRock(m_Random, p_ExplosionSound));
			m_LargeRocks[m_LargeRocks.size() - 1]->Activate(Vector2i(0, GetRandomY()));
		}
	}
}

void RockController::SpawnMedRocks(Vector2i location)
{
	for (int rock = 0; rock < 2; rock++)
	{
		bool spawnnewrock = true;

		for (int rockcheck = 0; rockcheck < m_MedRocks.size(); rockcheck++)
		{
			if (!m_MedRocks[rockcheck]->GetActive())
			{
				spawnnewrock = false;
				m_MedRocks[rockcheck]->Activate(location);				
				break;
			}
		}

		if (spawnnewrock)
		{
			m_MedRocks.push_back(new MediumRock(m_Random, p_ExplosionSound));
			m_MedRocks[m_MedRocks.size() - 1]->Activate(location);
		}
	}
}

void RockController::SpawnSmallRocks(Vector2i location)
{
	for (int rock = 0; rock < 2; rock++)
	{
		bool spawnnewrock = true;

		for (int rockcheck = 0; rockcheck < m_SmallRocks.size(); rockcheck++)
		{
			if (!m_SmallRocks[rockcheck]->GetActive())
			{
				spawnnewrock = false;
				m_SmallRocks[rockcheck]->Activate(location);				
				break;
			}
		}

		if (spawnnewrock)
		{
			m_SmallRocks.push_back(new SmallRock(m_Random, p_ExplosionSound));
			m_SmallRocks[m_SmallRocks.size() - 1]->Activate(location);
		}
	}
}

void RockController::SmallRockDistroyed(int rock)
{
	m_SmallRocks[rock]->Distroyed();
}

void RockController::MedRockDistroyed(int rock)
{
	SpawnSmallRocks(m_MedRocks[rock]->GetLocation());
	m_MedRocks[rock]->Distroyed();
}

void RockController::LargeRockDistroyed(int rock)
{
	SpawnMedRocks(m_LargeRocks[rock]->GetLocation());
	m_LargeRocks[rock]->Distroyed();
}

void RockController::NewGame(void)
{
	ClearAllRocks();
	m_NumberOfRocks = 4;
	SpawnNewWave(m_NumberOfRocks);
}

RockController::RockController(boost::random::mt19937 &random) : Common(random), m_Random(random)
{
	p_ExplosionSound = Mix_LoadWAV("RockExplosion.wav");
	Mix_VolumeChunk(p_ExplosionSound, MIX_MAX_VOLUME / 6);

	CreateRocks();
	NewGame();
}

RockController::~RockController(void)
{
}

//Private Methods -------------------------------------------------------------------
void RockController::CreateRocks(void)
{
	for (int rock = 0; rock < 8; rock++)
	{		
		m_LargeRocks.push_back(new LargeRock(m_Random, p_ExplosionSound));
	}

	for (int rock = 0; rock < 16; rock++)
	{		
		m_MedRocks.push_back(new MediumRock(m_Random, p_ExplosionSound));
	}

	for (int rock = 0; rock < 32; rock++)
	{		
		m_SmallRocks.push_back(new SmallRock(m_Random, p_ExplosionSound));
	}
}

bool RockController::CheckEndOfWave(void)
{
	bool endwave = true;

	for (int rock = 0; rock < m_LargeRocks.size(); rock++)
	{
		if (m_LargeRocks[rock]->GetActive())
			return false;
	}

	for (int rock = 0; rock < m_MedRocks.size(); rock++)
	{
		if (m_MedRocks[rock]->GetActive())
			return false;
	}

	for (int rock = 0; rock < m_SmallRocks.size(); rock++)
	{
		if (m_SmallRocks[rock]->GetActive())
			return false;
	}

	return true;
}

void RockController::NewWave(void)
{
	if (m_NumberOfRocks < 12)
		m_NumberOfRocks++;

	SpawnNewWave(m_NumberOfRocks);
}

void RockController::ClearAllRocks(void)
{
	for (int rock = 0; rock < m_LargeRocks.size(); rock++)
	{
		m_LargeRocks[rock]->SetActive(false);
		m_LargeRocks[rock]->SetDistroyed(false);
	}

	for (int rock = 0; rock < m_MedRocks.size(); rock++)
	{
		m_MedRocks[rock]->SetActive(false);
		m_MedRocks[rock]->SetDistroyed(false);
	}

	for (int rock = 0; rock < m_SmallRocks.size(); rock++)
	{
		m_SmallRocks[rock]->SetActive(false);
		m_SmallRocks[rock]->SetDistroyed(false);
	}
}

Vector2i RockController::GetLargeLocation(int rock)
{
	return m_LargeRocks[rock]->GetLocation();
}

int RockController::GetLargeRockCount()
{
	return m_LargeRocks.size();
}

int RockController::GetLargeRockRadius()
{
	return m_LargeRocks[0]->GetRadius();
}

bool RockController::GetLargeRockActive(int rock)
{
	return m_LargeRocks[rock]->GetActive();
}

int RockController::GetLargeRockPoints()
{
	return m_LargeRocks[0]->PlayerShotRock();
}

Vector2i RockController::GetMedLocation(int rock)
{
	return m_MedRocks[rock]->GetLocation();
}

int RockController::GetMedRockCount()
{
	return m_MedRocks.size();
}

int RockController::GetMedRockRadius()
{
	return m_MedRocks[0]->GetRadius();
}

bool RockController::GetMedRockActive(int rock)
{
	return m_MedRocks[rock]->GetActive();
}

int RockController::GetMedRockPoints()
{
	return m_MedRocks[0]->PlayerShotRock();
}

Vector2i RockController::GetSmallLocation(int rock)
{
	return m_SmallRocks[rock]->GetLocation();
}

int RockController::GetSmallRockCount()
{
	return m_SmallRocks.size();
}

int RockController::GetSmallRockRadius()
{
	return m_SmallRocks[0]->GetRadius();
}

bool RockController::GetSmallRockActive(int rock)
{
	return m_SmallRocks[rock]->GetActive();
}

int RockController::GetSmallRockPoints()
{
	return m_SmallRocks[0]->PlayerShotRock();
}
