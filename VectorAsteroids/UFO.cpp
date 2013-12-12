#include "UFO.h"

void UFO::Update(double *frame)
{
	if (m_Active)
	{
		CheckForSide();
		CheckForEdge();
		UpdateFrame(frame);
		DoesUFOChangeVector();
	}

	for (int shot = 0; shot < m_NumberOfShots; shot++)
		pShots[shot]->Update(frame);

	pExplosion->Update(frame);
}

void UFO::Draw(void)
{
	if (m_Active)
	{
		DrawUFO();
	}

	for (int shot = 0; shot < m_NumberOfShots; shot++)
		pShots[shot]->Draw();

	pExplosion->Draw();
}

void UFO::Activate(int Y)
{
	m_Location.y = Y;
	m_Active = true;
	m_ResetTimer = false;
	m_Distroyed = false;

	BuildUFO();

	if (Random(0, 100) < 50)
	{
		m_FromLeft = true;
		m_FromRight = false;
		m_Location.x = 0;
	}
	else
	{
		m_FromRight = true;
		m_FromLeft = false;
		m_Location.x = Window::GetWindowSize().x;
	}

	ChangeVector();
	ResetShotTimer(0);
}	

void UFO::SetUFOShotActive(bool Active, int Shot)
{
	pShots[Shot]->SetActive(Active);
}

bool UFO::GetResetTimer(void)
{
	return m_ResetTimer;
}

bool UFO::GetActive(void)
{
	return m_Active;
}

bool UFO::GetShotActive(int Shot)
{
	return pShots[Shot]->GetActive();
}

Vector2i UFO::GetLocation()
{
	return m_Location;
}

Vector2i UFO::GetShotLocation(int shot)
{
	return pShots[shot]->GetLocation();
}

float UFO::GetRadius(void)
{
	return m_Radius;
}

float UFO::GetShotRadius(int Shot)
{
	return pShots[Shot]->GetRadius();
}

bool UFO::GetDistroyed(void)
{
	return m_Distroyed;
}

void UFO::TimerReset(void)
{
	m_ResetTimer = false;
}

void UFO::Deactivate(void)
{
	m_Active = false;
	m_ResetTimer = true;
}

void UFO::NewGame(void)
{
	Deactivate();
	TimerReset();

	for (int shot = 0; shot < 2; shot++)
	{
		SetUFOShotActive(false, shot);
	}
}

void UFO::GotDistroyed(void)
{
	//Play UFO explosion sound.
	Mix_PlayChannel(-1, p_ExplosionSound, 0);
	pExplosion->Activate(m_Location, m_Radius);
	m_Distroyed = true;
	Deactivate();
}

UFO::UFO(boost::random::mt19937 &random, Mix_Chunk *ShotSound, Mix_Chunk *ExplosionSound) : Entity(random)
{
	m_NumberOfShots = 2;

	m_ResetTimer = false;
	m_FromLeft = false;
	m_FromRight = false;

	p_ShotSound = ShotSound;
	p_ExplosionSound = ExplosionSound;

	m_TimerAmountChangeVector = 4;
	m_TimerAmountFireShot = 1;
	m_TimerChangeVector = 0;
	m_TimerFireShot = 0;

	m_HLTopLX = 0;
	m_HLTopRX = 0;
	m_HLTopY = 0;

	m_HLLowerTopLX = 0;
	m_HLLowerTopRX = 0;
	m_HLLowerTopY = 0;

	m_HLUpperBottomLX = 0;
	m_HLUpperBottomRX = 0;
	m_HLUpperBottomY = 0;

	m_HLBottomLX = 0;
	m_HLBottomRX = 0;
	m_HLBottomY = 0;

	for (int shot = 0; shot < m_NumberOfShots; shot++)
	{
		pShots[shot] = new UFOShot(random);
	}

	pExplosion = new Explosion(random);
	pTimer = new Timer();
	pTimer->Start();
}

UFO::~UFO(void)
{
	delete pTimer;
	delete pExplosion;
}

//Protected methods. -----------------------------------
void UFO::FireShot(float angle)
{
	Mix_PlayChannel(-1, p_ShotSound, 0);

	for (int shot = 0; shot < 2; shot++)
	{
		if (!pShots[shot]->GetActive())
		{
			pShots[shot]->Activate(m_Location, angle);
			break;
		}
	}
}

void UFO::ResetShotTimer(float TimerAmount)
{
	m_TimerFireShot = m_TimerAmountFireShot + pTimer->Seconds() + Random(0, (int)(m_TimerAmountFireShot * 10) * 0.1);
}

//Private methods. -----------------------------------
void UFO::DoesUFOChangeVector(void)
{
	if (m_TimerChangeVector < pTimer->Seconds())
		ChangeVector();
}

void UFO::ChangeVector(void)
{
	float angle = 0;

	if (m_FromLeft)
	{
		angle = 5 + (Random(0, 200) *0.01);
	}

	if (m_FromRight)
	{
		angle = 2 + (Random(0, 200) *0.01);
	}

	float sinRot = sin(angle);
	float cosRot = cos(angle);

	m_Velocity.x = cosRot * m_MaxVelocity;
	m_Velocity.y = sinRot * m_MaxVelocity;

	m_TimerChangeVector = m_TimerAmountChangeVector + pTimer->Seconds() + Random(0, (int)(m_TimerAmountChangeVector * 0.75));
}

void UFO::CheckForSide(void)
{
	if (m_Location.x < 0 || m_Location.x > Window::GetWindowSize().x)
		Deactivate();
}

void UFO::BuildUFO(void)
{
	m_HLTopLX = -(m_Width / 7);
	m_HLTopRX = (m_Width / 7);
	m_HLTopY = -(m_Width / 5);

	m_HLLowerTopLX = -(m_Width / 4);
	m_HLLowerTopRX = (m_Width / 4);
	m_HLLowerTopY = 0;

	m_HLUpperBottomLX = -(m_Width / 2);
	m_HLUpperBottomRX = m_Width / 2;
	m_HLUpperBottomY = m_Width / 6;

	m_HLBottomLX = -(m_Width / 3);
	m_HLBottomRX = (m_Width / 3);
	m_HLBottomY = (m_Width / 3);
}

void UFO::DrawUFO(void)
{	
	int locationX = m_Location.x;
	int locationY = m_Location.y;
	int colorR = 100;
	int colorB = 255;

	Window::DrawLine(Vector2i(m_HLTopLX + locationX, m_HLTopY + locationY), 
		Vector2i(m_HLTopRX + locationX, m_HLTopY + locationY), colorR, 0, colorB, 255);
	Window::DrawLine(Vector2i(m_HLLowerTopLX + locationX, m_HLLowerTopY + locationY), 
		Vector2i(m_HLLowerTopRX + locationX, m_HLLowerTopY + locationY), colorR, 0, colorB, 255);
	Window::DrawLine(Vector2i(m_HLUpperBottomLX + locationX, m_HLUpperBottomY + locationY),
		Vector2i(m_HLUpperBottomRX + locationX, m_HLUpperBottomY + locationY), colorR, 0, colorB, 255);
	Window::DrawLine(Vector2i(m_HLBottomLX + locationX, m_HLBottomY + locationY), 
		Vector2i(m_HLBottomRX + locationX, m_HLBottomY + locationY), colorR, 0, colorB, 255);
	Window::DrawLine(Vector2i(m_HLTopRX + locationX, m_HLTopY + locationY), 
		Vector2i(m_HLLowerTopRX + locationX, m_HLLowerTopY + locationY), colorR, 0, colorB, 255);
	Window::DrawLine(Vector2i(m_HLLowerTopRX + locationX, m_HLLowerTopY + locationY), 
		Vector2i(m_HLUpperBottomRX + locationX, m_HLUpperBottomY + locationY), colorR, 0, colorB, 255);
	Window::DrawLine(Vector2i(m_HLUpperBottomRX + locationX, m_HLUpperBottomY + locationY), 
		Vector2i(m_HLBottomRX + locationX, m_HLBottomY + locationY), colorR, 0, colorB, 255);
	Window::DrawLine(Vector2i(m_HLTopLX + locationX, m_HLTopY + locationY), 
		Vector2i(m_HLLowerTopLX + locationX, m_HLLowerTopY + locationY), colorR, 0, colorB, 255);
	Window::DrawLine(Vector2i(m_HLLowerTopLX + locationX, m_HLLowerTopY + locationY), 
		Vector2i(m_HLUpperBottomLX + locationX, m_HLUpperBottomY + locationY), colorR, 0, colorB, 255);
	Window::DrawLine(Vector2i(m_HLUpperBottomLX + locationX, m_HLUpperBottomY + locationY), 
		Vector2i(m_HLBottomLX + locationX, m_HLBottomY + locationY), colorR, 0, colorB, 255);
}

void UFO::TimerPause(void)
{
	pTimer->Pause();
	pExplosion->PauseTimer();

	for (int shot = 0; shot < 2; shot++)
	{
		pShots[shot]->TimerPause();
	}
}

void UFO::TimerUnpause(void)
{
	pTimer->Unpause();
	pExplosion->UnpauseTimer();

	for (int shot = 0; shot < 2; shot++)
	{
		pShots[shot]->TimerUnpause();
	}
}