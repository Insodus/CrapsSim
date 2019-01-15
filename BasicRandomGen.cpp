#include "StdAfx.h"
#include "BasicRandomGen.h"

BasicRandomGen::BasicRandomGen(void)
{
	srand((unsigned) time(NULL));
	for (int i=0;i<100;i++) rand();
}

BasicRandomGen::~BasicRandomGen(void)
{
}

int BasicRandomGen::getRandom(int min, int max)
{
	return min + (int)uniformDivide(rand()) * ( max - min + 1 );
}

double BasicRandomGen::uniformDivide(int seed)
{
	return seed * ( 1.0 / ( RAND_MAX + 1.0 ) );
}