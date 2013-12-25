#include "UFOController.h"

void UFOController::Update(double *frame, Vector2i *playerLocation)
{

	if (!m_UFOActive)
	{
		DoesUFOSpawn();
	}

	if (pLargeUFO->GetResetTimer())
		ResetTimer();

	if (pSmallUFO->GetResetTimer())
		ResetTimer();

	pLargeUFO->Update(frame);
	pSmallUFO->Update(frame, playerLocation);
}

void UFOController::Draw(void)
{
	pLargeUFO->Draw();
	pSmallUFO->Draw();
}

void UFOController::SetLUFOActive(bool Active)
{
	pLargeUFO->SetActive(Active);
}

void UFOController::SetSUFOActive(bool Active)
{
	pSmallUFO->SetActive(Active);
}

void UFOController::SetLUFOShotActive(bool Active, int Shot)
{
	pLargeUFO->SetUFOShotActive(Active, Shot);
}

void UFOController::SetSUFOShotActive(bool Active, int Shot)
{
	pSmallUFO->SetUFOShotActive(Active, Shot);
}

bool UFOController::GetLargeShotActive(int Shot)
{
	return pLargeUFO->GetShotActive(Shot);
}

bool UFOController::GetSmallShotActive(int Shot)
{
	return pSmallUFO->GetShotActive(Shot);
}

Vector2i UFOController::GetLargeShot(int Shot)
{
	return pLargeUFO->GetShotLocation(Shot);
}

Vector2i UFOController::GetSmallShot(int Shot)
{
	return pSmallUFO->GetShotLocation(Shot);
}


bool UFOController::GetLUFOActive(void)
{
	return pLargeUFO->GetActive();
}

bool UFOController::GetSUFOActive(void)
{
	return pSmallUFO->GetActive();
}

Vector2i UFOController::GetLargeUFO(void)
{
	return pLargeUFO->GetLocation();
}

Vector2i UFOController::GetSmallUFO(void)
{
	return pSmallUFO->GetLocation();
}

float UFOController::GetLargeShotRadius(int Shot)
{
	return pLargeUFO->GetShotRadius(Shot);
}

float UFOController::GetSmallShotRadius(int Shot)
{
	return pSmallUFO->GetShotRadius(Shot);
}

float UFOController::GetLUFORadius(void)
{
	return pLargeUFO->GetRadius();
}

float UFOController::GetSUFORadius(void)
{
	return pSmallUFO->GetRadius();
}

void UFOController::NewGame(void)
{
	m_SpawnCounter = 0;

	pLargeUFO->NewGame();
	pSmallUFO->NewGame();

	ResetTimer();
}

void UFOController::ResetTimer(void)
{
	m_TimerSpawn = m_TimerSpawnAmount + m_WaveCounter + (float)pTimer->Seconds() + 
		Window::Random((int)(m_TimerSpawnAmount * 0.1), (int)(m_TimerSpawnAmount * 0.5));
	pLargeUFO->TimerReset();
	pSmallUFO->TimerReset();
	m_UFOActive = false;

	//Turn off engine sound.
	if (m_UFOSoundChannel > -1)
	{
		Mix_HaltChannel(m_UFOSoundChannel);
		m_UFOSoundChannel = -1;
	}

}

void UFOController::SetWave(int number)
{
	pSmallUFO->SetWaveNumber(number);
	m_WaveCounter = number;
}

UFOController::UFOController() : Common()
{
	m_TimerSpawn = 0;
	m_TimerSpawnAmount = 14.25;
	m_UFOActive = false;

	p_LargeEngineSound = Mix_LoadWAV("UFOLarge.wav");
	Mix_VolumeChunk(p_LargeEngineSound, MIX_MAX_VOLUME / 4);
	p_SmallEngineSound = Mix_LoadWAV("UFOSmall.wav");
	Mix_VolumeChunk(p_SmallEngineSound, MIX_MAX_VOLUME / 4);
	Mix_Chunk *UFOShotSound = Mix_LoadWAV("UFOShot.wav");
	Mix_VolumeChunk(UFOShotSound, MIX_MAX_VOLUME / 4);
	Mix_Chunk *UFOExplosionSound = Mix_LoadWAV("UFOExplosion.wav");
	Mix_VolumeChunk(UFOExplosionSound, MIX_MAX_VOLUME / 4);

	pLargeUFO = new LargeUFO(UFOShotSound, UFOExplosionSound);
	pSmallUFO = new SmallUFO(UFOShotSound, UFOExplosionSound);
	pTimer = new Timer();
	pTimer->Start();
	NewGame();
}

UFOController::~UFOController(void)
{
	delete pLargeUFO;
	delete pSmallUFO;
	delete pTimer;
}

//Private methods. ---------------------------------------------------
void UFOController::SpawnLargeUFO(void)
{
	//Play UFO engine sound.
	if (m_UFOSoundChannel < 0)
		m_UFOSoundChannel = Mix_PlayChannel(-1, p_LargeEngineSound, -1);

	pLargeUFO->Activate(GetRandomY());
}

void UFOController::SpawnSmallUFO(void)
{
	//Play UFO engine sound.
	if (m_UFOSoundChannel < 0)
		m_UFOSoundChannel = Mix_PlayChannel(-1, p_SmallEngineSound, -1);

	pSmallUFO->Activate(GetRandomY());
}

void UFOController::LargeUFODistroyed(void)
{
	pLargeUFO->GotDistroyed();
	ResetTimer();
}

void UFOController::SmallUFODistroyed(void)
{
	pSmallUFO->GotDistroyed();
	ResetTimer();
}

void UFOController::DoesUFOSpawn(void)
{
	double sec = pTimer->Seconds();
	if (m_TimerSpawn < pTimer->Seconds())
	{
		float spawnPercent = (float)(pow(0.915, (m_SpawnCounter * 2) / m_WaveCounter));

		// After each UFO is spawned the chance it is a small UFO increases.
		// The first spawn it is 0% chance, it goes down after each one.
		if (Window::Random(0, 99) < spawnPercent * 100)
		{
			SpawnLargeUFO();
		}
		else
		{
			SpawnSmallUFO();
		}

		m_SpawnCounter++;
		m_UFOActive = true;

		pSmallUFO->SetSpawnedNumber(m_SpawnCounter);

		std::cout << std::endl;
		std::cout << "Spawn Counter. " << m_SpawnCounter << std::endl;
		std::cout << "Wave Counter. " << m_WaveCounter << std::endl;
		std::cout << "Spawn Chance for Large UFO. " << spawnPercent * 100 << std::endl;
	}
}

int UFOController::PlayerShotLargeUFO(void)
{
	return pLargeUFO->PlayerShotUFO();
}

int UFOController::PlayerShotSmallUFO(void)
{
	return pSmallUFO->PlayerShotUFO();
}

void UFOController::PauseTimer(void)
{
	pTimer->Pause();
	pLargeUFO->TimerPause();
	pSmallUFO->TimerPause();
}

void UFOController::UnpauseTimer(void)
{
	pTimer->Unpause();
	pLargeUFO->TimerUnpause();
	pSmallUFO->TimerUnpause();
}