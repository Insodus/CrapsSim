#include "StdAfx.h"
#include "BoostRandomGen.h"

#include <boost/random/uniform_int_distribution.hpp>

BoostRandomGen::BoostRandomGen(void)
{
	currGen = 1;
}

BoostRandomGen::~BoostRandomGen(void)
{
}

int BoostRandomGen::getRandom(int min, int max)
{
	boost::random::uniform_int_distribution<> dist(min, max);
	return dist(gen);
}

int BoostRandomGen::getRandom2(int min, int max)
{
	boost::random::uniform_int_distribution<> dist(min, max);
	int ret = 0;
	switch (currGen)
	{
	case 1:
		ret = dist(gen1);
		break;
	case 2:
		ret = dist(gen2);
		break;
	case 3:
		ret = dist(gen3);
		break;
	case 4:
		ret = dist(gen4);
		break;
	}

	currGen++;
	if (currGen == 5)
	{
		currGen = 1;
	}

    return ret;
}
