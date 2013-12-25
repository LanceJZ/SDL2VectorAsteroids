#include "Common.h"

// Add the Random Generator referance to the rest of the classes.
Common::Common()
{
}


Common::~Common()
{
}

//Protected methods ------------------------------------------------------------
Vector2i Common::RandomLocation(void)
{
	return Vector2i(Window::Random(0, Window::GetWindowSize().x), Window::Random(0, Window::GetWindowSize().y));
}

int Common::GetRandomX(void)
{
	return (Window::Random(0, Window::GetWindowSize().x));
}

int Common::GetRandomY(void)
{
	return (Window::Random(0, Window::GetWindowSize().y));
}