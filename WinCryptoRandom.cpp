#include "StdAfx.h"
#include "WinCryptoRandom.h"

WinCryptoRandom::WinCryptoRandom(void)
{
}

WinCryptoRandom::~WinCryptoRandom(void)
{
}

int WinCryptoRandom::getRandom(int min, int max)
{
	unsigned int num = 0;
	rand_s(&num);
	//return min + (num % (max - min + 1));
	return min + (num / ( UINT_MAX / ( max - min + 1 )));
}
