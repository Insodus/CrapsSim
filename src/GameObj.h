#pragma once

#include "Logger.h"
#include "random/RandomGenProvider.h"

/* for ciclical dependency */
class Player;

class GameObj
{
	int state;
	int point;
	int lastNumber;

	Logger* logger;
	RandomGenProvider* randGen;

	Player* players[5];
	int currPlayers;

	int getRoll();

public:

	const static int STATE_OFF = 1;
	const static int STATE_ON = 2;

	GameObj(void);
	~GameObj(void);

	void addPlayer(Player* player);

	void setState(int state);
	int getState();

	void setPoint(int point);
	int getPoint();

	void play();
	int getLastNumber();

};

