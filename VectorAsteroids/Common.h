#ifndef COMMON_H
#define COMMON_H

#include <vmath.h>
#include <stdexcept>
#include <iostream>
#include <string>
#include <sstream>
#include <random/mersenne_twister.hpp>
#include <random/uniform_int_distribution.hpp>
#include "Window.h"

class Common
{
protected:
	Vector2i RandomLocation(void);
	int GetRandomX(void);
	int GetRandomY(void);
	int Random(int Min, int Max);
	boost::random::mt19937 &m_RandGenerator;

public:
	Common(boost::random::mt19937 &generator);
	~Common();
};

#endif