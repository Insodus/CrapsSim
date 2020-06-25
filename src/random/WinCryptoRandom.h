#pragma once
#include "RandomGenProvider.h"

class WinCryptoRandom
	: public RandomGenProvider
{
public:
	WinCryptoRandom(void);
	~WinCryptoRandom(void);

	int getRandom(int min, int max);
};

