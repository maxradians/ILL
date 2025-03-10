#pragma once
#include <random>

int ra(int min, int max)
{
	std::random_device random; //get random number from hardware stuff
	std::mt19937 gen(random());
	std::uniform_int_distribution<> distrib(min, max);

	int randomNumber = distrib(gen);

	return randomNumber;
}
