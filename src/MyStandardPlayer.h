#pragma once
#include "Player.h"

class MyStandardPlayer :
	public Player
{
public:
	MyStandardPlayer(void);
	~MyStandardPlayer(void);

	void play();

	int myComeBetsThisHand;
};

