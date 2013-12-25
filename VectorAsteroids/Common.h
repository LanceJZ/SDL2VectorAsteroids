#ifndef COMMON_H
#define COMMON_H

#include "Window.h"

class Common
{
protected:
	Vector2i RandomLocation(void);
	int GetRandomX(void);
	int GetRandomY(void);

public:
	Common();
	~Common();
};

#endif