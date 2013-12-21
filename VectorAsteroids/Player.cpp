#include "Player.h"

//Public Methods
void Player::Draw(void)
{
	if (m_Active && !m_Hit)
	{
		pShip->Draw(&ShipColor);

		if (m_Thrust)
			DrawThrust();
	}
	else if (m_ExplosionOn)
		pShip->DrawExplosion();

	DrawShots();
}

void Player::Update(double *Frame)
{
	if (m_Active && !m_Hit)
	{
		CheckForEdge();
		UpdateShip();
		UpdateRotationThrust();
		UpdateFrame(Frame);
		pShip->Update(&m_Rotation, &m_Location, &m_Scale);
	}
	else if (m_ExplosionOn)
	{
		pShip->UpdateExplosion(Frame);

		if (m_ExplosionTimer < pTimer->Seconds())
			m_ExplosionOn = false;

	}

	UpdateShots(Frame);
}

void Player::FireButtonPressed(void)
{
	if (m_Active && !m_Hit)
	{
		FireShot();
	}
}

void Player::HyperSpaceButtonPressed(void)
{
	if (m_Active && !m_Hit)
	{
		 SetLocation(RandomLocation());
	}
}

void Player::SetLocation(Vector2f location)
{
	m_Location = location;
	m_Velocity = Vector2f(0, 0);
	m_Acceleration = Vector2f(0, 0);
	m_Hit = false;
}

void Player::SetLocationX(float X)
{
	if (X > 0 && X < Window::GetWindowSize().x)
		m_Location.x = X;
}

void Player::SetLocationY(float Y)
{
	if (Y > 0 && Y < Window::GetWindowSize().y)
		m_Location.y = Y;
}

void Player::SetVelocity(Vector2f velocity)
{
	m_Velocity = velocity;
}

void Player::SetVelocityX(float X)
{
	m_Velocity.x = X;
}

void Player::SetVelocityY(float Y)
{
	m_Velocity.y = Y;
}

void Player::SetAcceleration(Vector2f acceleration)
{
	m_Acceleration = acceleration;
}

void Player::SetAccelerationX(float X)
{
	m_Acceleration.x = X;
}

void Player::SetAccelerationY(float Y)
{
	m_Acceleration.y = Y;
}

Vector2f Player::GetLocation(void)
{
	return m_Location;
}

float Player::GetLocationX(void)
{
	return m_Location.x;
}

float Player::GetLocationY(void)
{
	return m_Location.y;
}

float Player::GetRadius(void)
{
	return m_Radius;
}

int Player::GetNumberOfShots(void)
{
	return m_NumberOfShots;
}

float Player::GetShotRadius(void)
{
	return pShots[0]->GetRadius();
}

float Player::GetShotX(int Shot)
{
	return  pShots[Shot]->GetX();
}

float Player::GetShotY(int Shot)
{
	return pShots[Shot]->GetY();
}

Vector2i Player::GetShotLocation(int Shot)
{
	return pShots[Shot]->GetLocation();
}

bool Player::GetShotActive(int Shot)
{
	return pShots[Shot]->GetActive();
}

bool Player::GetHit()
{
	return m_Hit;
}

bool Player::GetExplosionOn()
{
	return m_ExplosionOn;
}

void Player::SetShotActive(int Shot, bool Active)
{
	pShots[Shot]->SetActive(Active);
}

void Player::SetThrust(bool Thrust)
{
	m_Thrust = Thrust;
}

void Player::SetTurnRight(bool TurnRight)
{
	m_TurnRight = TurnRight;
}

void Player::SetTurnLeft(bool TurnLeft)
{
	m_TurnLeft = TurnLeft;
}

void Player::SetRotation(float Rotation)
{
	m_Rotation = Rotation;
}

void Player::NewGame(void)
{
	pTimer->Start();
	m_Active = true;
}

void Player::Hit(void)
{
	if (!m_Hit)
	{
		//Play Player explosion sound.
		Mix_PlayChannel(-1, p_Explosionsound, 0);

		m_ExplosionOn = true;
		m_Hit = true;
		SetExplosion();

		//Turn off thrust sound.
		if (m_ThrustChannel > -1)
		{
			Mix_HaltChannel(m_ThrustChannel);
			m_ThrustChannel = -1;
		}
	}
}

void Player::Spawn(void)
{
	m_Hit = false;
	m_Location = Window::GetWindowSize() / 2;
	m_Acceleration = Vector2i();
	m_Velocity = Vector2i();
	m_Rotation = 0;
}

// Add the Random Generator referance to the rest of the classes.
Player::Player(boost::random::mt19937 &generator) : Entity(generator)
{
	m_Thrust = false;
	m_Fire = false;
	m_TurnRight = false;
	m_TurnLeft = false;
	m_Active = false;
	m_Hit = false;
	m_ExplosionOn = false;
	m_ShipHeight = 25;
	m_ShipWidth = 15;
	m_Radius = m_ShipHeight - 7;
	m_Scale = 2;

	m_MaxThrust = 325;
	m_ThrustMagnitude = 1.55;
	m_TurnRate = 0.06;

	m_NumberOfShots = 4;
	m_ThrustDrawTimerAmount = 0.100;
	m_ExplosiontTimerAmount = 2.500;

	pTimer = new Timer();
	pShip = new PlayerShip();
	
	//Player Ship Color
	ShipColor.Red = 100;
	ShipColor.Green = 40;
	ShipColor.Blue = 255;
	ShipColor.Alpha = 255;

	p_Shotsound = Mix_LoadWAV("PlayerShot.wav");
	Mix_VolumeChunk(p_Shotsound, MIX_MAX_VOLUME / 3);
	p_Thrustsound = Mix_LoadWAV("PlayerThrust.wav");
	Mix_VolumeChunk(p_Thrustsound, MIX_MAX_VOLUME / 3);
	p_Explosionsound = Mix_LoadWAV("PlayerExplosion.wav");
	Mix_VolumeChunk(p_Explosionsound, MIX_MAX_VOLUME / 4);

	InitializeShot(generator);
	pShip->InitializeLines(generator);
}

Player::~Player(void)
{
	delete pTimer;
	delete pShots;
}

void Player::FireShot(void)
{
	int activateshot = 0;
	bool foundshot = false;

	for (int shot = 0; shot < 4; shot++)
	{
		if (!pShots[shot]->GetActive())
		{
			Mix_PlayChannel(-1, p_Shotsound, 0);

			activateshot = shot;
			foundshot = true;
			break;
		}
	}

	//If shot found that is not active, then activate that shot.
	if (foundshot)
		pShots[activateshot]->Activate(m_Location, m_Rotation);
}

void Player::UpdateShots(double *Frame)
{
	for (int shot = 0; shot < m_NumberOfShots; shot++)
	{
		pShots[shot]->Update(Frame);
	}
}

void Player::DrawShots(void)
{
	for (int shot = 0; shot < m_NumberOfShots; shot++)
		pShots[shot]->Draw();
}

void Player::DrawThrust(void)
{
	if (pTimer->Seconds() > m_ThrustDrawTimer)
	{
		m_ThrustDrawTimer = pTimer->Seconds() + m_ThrustDrawTimerAmount;
	}
	else
	{
		pShip->DrawThrust();
	}
}

void Player::InitializeShot(boost::random::mt19937 &generator)
{
	for (int shot = 0; shot < m_NumberOfShots; shot++)
	{
		pShots[shot] = new PlayerShot(generator);
	}
}

void Player::UpdateShip(void)
{
	//Get rotation keys
	if (m_TurnRight)
	{
		if (m_Rotation < 6)
			m_Rotation += m_TurnRate;
		else
			m_Rotation -= 6.25;
	}

	if (m_TurnLeft)
	{
		if (m_Rotation > 0)
			m_Rotation -= m_TurnRate;
		else
			m_Rotation += 6.25;
	}
}

void Player::UpdateRotationThrust(void)
{
	//Do thrust key
	if (m_Thrust)
	{
		//Play thrust sound.
		if (m_ThrustChannel < 0)
			m_ThrustChannel = Mix_PlayChannel(-1, p_Thrustsound, -1);

		if (m_Velocity.x > m_MaxThrust)
			m_Velocity.x = m_MaxThrust;

		if (m_Velocity.x < -m_MaxThrust)
			m_Velocity.x = -m_MaxThrust;

		if (m_Velocity.y > m_MaxThrust)
			m_Velocity.y = m_MaxThrust;

		if (m_Velocity.y < -m_MaxThrust)
			m_Velocity.y = -m_MaxThrust;

		m_Acceleration.x = cos(m_Rotation) * m_ThrustMagnitude;
		m_Acceleration.y = sin(m_Rotation) * m_ThrustMagnitude;
	}
	else
	{
		//Turn off thrust sound.
		if (m_ThrustChannel > -1)
		{
			Mix_HaltChannel(m_ThrustChannel);
			m_ThrustChannel = -1;
		}

		m_Acceleration = Vector2f(0, 0);
	}
}

void Player::SetExplosion(void)
{
	pShip->SetExplosion(&(Vector2i)m_Location);
	m_ExplosionTimer = pTimer->Seconds() + m_ExplosiontTimerAmount + Random(0, (int)m_ExplosiontTimerAmount);
}

void Player::TimerPause(void)
{
	for (int shot = 0; shot < 4; shot++)
	{
		pShots[shot]->TimerPause();
	}
}

void Player::TimerUnpause(void)
{
	for (int shot = 0; shot < 4; shot++)
	{
		pShots[shot]->TimerUnpause();
	}
}