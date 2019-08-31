#ifndef UTIL_HPP
#define UTIL_HPP
#include <iostream>
#include <limits>

void clearscreen();

constexpr void pause()
{
	std::cin.ignore(std::umeric_limits<streamsize>::max(), '\n');
}

#endif
