#ifndef PLAYEREXPLOSIONLINE_H
#define PLAYEREXPLOSIONLINE_H
#include "Entity.h"
#include "Window.h"

class PlayerExplosionLine :
	public Entity
{
private:
	Line ShipLine;
	Line NewLine;
	Color m_Color;

public:
	PlayerExplosionLine(boost::random::mt19937 &generator);
	~PlayerExplosionLine();

	void Update(double *frame);
	void Draw(void);
	void Initialize(Line *Line, Vector2i *Location);
};

#endif