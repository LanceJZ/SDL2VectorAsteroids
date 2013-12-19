#ifndef PLAYERSHIP_H
#define PLAYERSHIP_H
#include "Window.h"
#include "PlayerExplosionLine.h"

struct Color
{
	int Red;
	int Green;
	int Blue;
	int Alpha;
};

class PlayerShip
{
private:
	PlayerExplosionLine *pExplosionLines[3];

	ShipLine PlayerLines[3];
	ShipLine NewPlayerLines[3];

	ShipLine ThrustLines[2];
	ShipLine NewThrustLines[2];

	void Initialize(void);
	void MoveRotateLines(float *rotation, Vector2f *location, float *scale);	

public:
	PlayerShip();
	~PlayerShip();

	void Update(float *rotation, Vector2f *location, float *scale);
	void Draw(Color *color);
	void DrawThrust(void);
	void UpdateExplosion(double *frame);
	void DrawExplosion(void);
	void SetExplosion(Vector2i location);
	void InitializeLines(boost::random::mt19937 &generator);
};

#endif