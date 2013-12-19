#ifndef PLAYEREXPLOSIONLINE_H
#define PLAYEREXPLOSIONLINE_H
#include "Entity.h"
#include "Window.h"

struct ShipLine
{
	Vector2i LineStart;
	Vector2i LineEnd;
};

class PlayerExplosionLine :
	public Entity
{
private:
	ShipLine Line;
	ShipLine NewLine;

public:
	PlayerExplosionLine(boost::random::mt19937 &generator);
	~PlayerExplosionLine();

	void Update(double *frame);
	void Draw(void);
	void Initialize(ShipLine line, Vector2i location);
};

#endif