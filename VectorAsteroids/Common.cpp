#include "Common.h"

// Add the Random Generator referance to the rest of the classes.
Common::Common(boost::random::mt19937 &generator) : m_RandGenerator(generator)
{
}


Common::~Common()
{
}

//Protected methods ------------------------------------------------------------
Vector2i Common::RandomLocation(void)
{
	return Vector2i(Random(0, Window::GetWindowSize().x), Random(0, Window::GetWindowSize().y));
}

int Common::GetRandomX(void)
{
	return (Random(0, Window::GetWindowSize().x));
}

int Common::GetRandomY(void)
{
	return (Random(0, Window::GetWindowSize().y));
}

int Common::Random(int Min, int Max)
{
	boost::random::uniform_int_distribution<> roll(Min, Max);
	return roll(m_RandGenerator);
}

