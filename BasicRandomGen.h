#pragma once
#include "RandomGenProvider.h"
class BasicRandomGen :
	public RandomGenProvider
{
public:
	BasicRandomGen(void);
	~BasicRandomGen(void);

	int getRandom(int min, int max);
	double uniformDivide(int seed);
};

