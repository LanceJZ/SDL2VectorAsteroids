#ifndef EXPLOSION_H
#define EXPLOSION_H
#include "Dot.h"

class Explosion
{
private:
	Dot *pDots[12];
	bool m_Active;

public:
	Explosion(void);
	~Explosion(void);

	void Draw();
	void Update(double *Frame);
	void Activate(Vector2i location, int Size);
	void PauseTimer(void);
	void UnpauseTimer(void);
};

#endif