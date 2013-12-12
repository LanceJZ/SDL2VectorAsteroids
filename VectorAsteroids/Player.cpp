#include "Player.h"

//Public Methods
void Player::Draw(void)
{
	if (m_Active && !m_Hit)
	{
		//Set each point as an array then you can use a for/next to draw the three lines.
		Window::DrawLine(m_Front, m_RSide, 90, 0, 255, 255);
		Window::DrawLine(m_Front, m_LSide, 90, 0, 255, 255);
		Window::DrawLine(m_LInSide, m_RInSide, 90, 0, 255, 255);

		if (m_Thrust)
			DrawThrust();
	}
	else if (m_ExplosionOn)
		DrawExplosion();

	DrawShots();
}

void Player::Update(double *Frame)
{
	if (m_Active && !m_Hit)
	{
		CheckForEdge();
		UpdateShip(Frame);
		UpdateFrame(Frame);
	}
	else if (m_ExplosionOn)
		UpdateExplosion(Frame);

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

	m_MaxThrust = 325;
	m_ThrustMagnitude = 1.55;
	m_TurnRate = 0.06;

	m_NumberOfShots = 4;
	m_ThrustDrawTimerAmount = 0.100;
	m_ExplosiontTimerAmount = 2.500;

	pTimer = new Timer();

	p_Shotsound = Mix_LoadWAV("PlayerShot.wav");
	Mix_VolumeChunk(p_Shotsound, MIX_MAX_VOLUME / 3);
	p_Thrustsound = Mix_LoadWAV("PlayerThrust.wav");
	Mix_VolumeChunk(p_Thrustsound, MIX_MAX_VOLUME / 3);
	p_Explosionsound = Mix_LoadWAV("PlayerExplosion.wav");
	Mix_VolumeChunk(p_Explosionsound, MIX_MAX_VOLUME / 4);

	InitializeShot(generator);
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

void Player::InitializeShot(boost::random::mt19937 &generator)
{
	for (int shot = 0; shot < m_NumberOfShots; shot++)
	{
		pShots[shot] = new PlayerShot(generator);
	}
}

void Player::UpdateShip(double *Frame)
{
	//Place points for lines of ship.
	m_Front.x = m_Location.x + (m_ShipHeight / 2);
	m_Front.y = m_Location.y;

	m_RSide.x = m_Location.x - (m_ShipHeight / 2);
	m_RSide.y = m_Location.y - (m_ShipWidth / 2);

	m_LSide.x = m_Location.x - (m_ShipHeight / 2);
	m_LSide.y = m_Location.y + (m_ShipWidth / 2);

	m_RInSide.x = m_RSide.x + 3;
	m_RInSide.y = m_RSide.y + 2;

	m_LInSide.x = m_LSide.x + 3;
	m_LInSide.y = m_LSide.y - 2;

	m_LFlame.x = m_LInSide.x;
	m_LFlame.y = m_LInSide.y - 2;
	m_RFlame.x = m_RInSide.x;
	m_RFlame.y = m_RInSide.y + 2;
	m_FlameTip.x = m_LSide.x - 3;
	m_FlameTip.y = m_Front.y;


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

	DoRotationThrust();
}

void Player::DoRotationThrust(void)
{
	float sinRot = sin(m_Rotation);
	float cosRot = cos(m_Rotation);

	m_Front -= m_Location;
	m_RSide -= m_Location;
	m_LSide -= m_Location;
	m_LInSide -= m_Location;
	m_RInSide -= m_Location;
	m_LFlame -= m_Location;
	m_RFlame -= m_Location;
	m_FlameTip -= m_Location;

	float xnewtop = m_Front.x * cosRot - m_Front.y * sinRot;
	float ynewtop = m_Front.x * sinRot + m_Front.y * cosRot;

	float xnewrb = m_RSide.x * cosRot - m_RSide.y * sinRot;
	float ynewrb = m_RSide.x * sinRot + m_RSide.y * cosRot;

	float xnewlb = m_LSide.x * cosRot - m_LSide.y * sinRot;
	float ynewlb = m_LSide.x * sinRot + m_LSide.y * cosRot;

	float xnewinsider = m_RInSide.x * cosRot - m_RInSide.y * sinRot;
	float ynewinsider = m_RInSide.x * sinRot + m_RInSide.y * cosRot;

	float xnewinsidel = m_LInSide.x * cosRot - m_LInSide.y * sinRot;
	float ynewinsidel = m_LInSide.x * sinRot + m_LInSide.y * cosRot;

	float xnewLFlame = m_LFlame.x * cosRot - m_LFlame.y * sinRot;
	float ynewLFlame = m_LFlame.x * sinRot + m_LFlame.y * cosRot;

	float xnewRFlame = m_RFlame.x * cosRot - m_RFlame.y * sinRot;
	float ynewRFlame = m_RFlame.x * sinRot + m_RFlame.y * cosRot;

	float xnewFlameTip = m_FlameTip.x * cosRot - m_FlameTip.y * sinRot;
	float ynewFlameTip = m_FlameTip.x * sinRot + m_FlameTip.y * cosRot;

	m_Front.x = xnewtop + m_Location.x;
	m_Front.y = ynewtop + m_Location.y;

	m_RSide.x = xnewrb + m_Location.x;
	m_RSide.y = ynewrb + m_Location.y;

	m_LSide.x = xnewlb + m_Location.x;
	m_LSide.y = ynewlb + m_Location.y;

	m_RInSide.x = xnewinsider + m_Location.x;
	m_RInSide.y = ynewinsider + m_Location.y;

	m_LInSide.x = xnewinsidel + m_Location.x;
	m_LInSide.y = ynewinsidel + m_Location.y;

	m_LFlame.x = xnewLFlame + m_Location.x;
	m_LFlame.y = ynewLFlame + m_Location.y;

	m_RFlame.x = xnewRFlame + m_Location.x;
	m_RFlame.y = ynewRFlame + m_Location.y;

	m_FlameTip.x = xnewFlameTip + m_Location.x;
	m_FlameTip.y = ynewFlameTip + m_Location.y;

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

		m_Acceleration.x = cosRot * m_ThrustMagnitude;
		m_Acceleration.y = sinRot * m_ThrustMagnitude;		
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

void Player::DrawThrust(void)
{
	if (pTimer->Seconds() > m_ThrustDrawTimer)
	{
		m_ThrustDrawTimer = pTimer->Seconds() + m_ThrustDrawTimerAmount;
	}
	else
	{
		Window::DrawLine(m_LFlame, m_FlameTip, 150, 0, 230, 55);
		Window::DrawLine(m_RFlame, m_FlameTip, 150, 0, 230, 55);
	}
}

void Player::SetExplosion(void)
{
	m_ExLFront = m_Front;
	m_ExRFront = m_Front;
	m_ExRSide = m_RSide;
	m_ExLSide = m_LSide;
	m_ExLInside = m_LInSide;
	m_ExRInside = m_RInSide;

	int max = 200;

	m_ExMoveA = Vector2f((Random(-max, max) * 0.1) - (Random(-max, max) * 0.1), (Random(-max, max) * 0.1) - (Random(-max, max) * 0.1));
	m_ExMoveB = Vector2f((Random(-max, max) * 0.1) - (Random(-max, max) * 0.1), (Random(-max, max) * 0.1) - (Random(-max, max) * 0.1));
	m_ExMoveC = Vector2f((Random(-max, max) * 0.1) - (Random(-max, max) * 0.1), (Random(-max, max) * 0.1) - (Random(-max, max) * 0.1));

	m_ExplosionTimer = pTimer->Seconds() + m_ExplosiontTimerAmount + Random(0, (int)m_ExplosiontTimerAmount);
}

void Player::UpdateExplosion(double *Frame) //Make explosion a seperate class.
{
	double frame;
	frame = *Frame;
	//LineA
	m_ExRFront += m_ExMoveA * frame;
	m_ExRSide += m_ExMoveA * frame;
	//LineB
	m_ExLFront += m_ExMoveB * frame;
	m_ExLSide += m_ExMoveB * frame;
	//LineC
	m_ExLInside += m_ExMoveC * frame;
	m_ExRInside += m_ExMoveC * frame;
	
	if (m_ExplosionTimer < pTimer->Seconds())
		m_ExplosionOn = false;
}

void Player::DrawExplosion(void)
{
	Window::DrawLine(m_ExRFront, m_ExRSide, 150, 0, 230, 255);
	Window::DrawLine(m_ExLFront, m_ExLSide, 150, 0, 230, 255);
	Window::DrawLine(m_ExLInside, m_ExRInside, 150, 0, 230, 255);
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