#pragma once
#include "randomgenprovider.h"
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/shuffle_order.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/lagged_fibonacci.hpp>

class BoostRandomGen :
	public RandomGenProvider
{
	boost::random::lagged_fibonacci44497 gen;

	boost::random::mt19937 gen1;
	boost::random::mt11213b gen2;
	boost::random::rand48 gen3;
	boost::random::kreutzer1986 gen4;

	int currGen;

public:
	BoostRandomGen(void);
	~BoostRandomGen(void);
	
	int getRandom(int min, int max);
	int getRandom2(int min, int max);
};

