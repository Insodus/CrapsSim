// CrapsSim.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Logger.h"
#include "GameObj.h"
#include "MyStandardPlayer.h"
#include "MyLuaPlayer.h"

#if _DEBUG
void pause()
{
	system("pause");
}
#endif

int main(int argc, char* argv[])
{
	#if _DEBUG
	atexit(pause);
	#endif

	// setup our variables for the run
	CRAPS_LOGGING_ENABLED = true;
	Logger logger;
	logger.setPrefix("GameManager");
	int games = 5;
	int roundsPerGame = 100;
	double playerTotal = 0;
	double playerMin = 10000;
	double playerMax = 0;
	int mode = 1;
	std::string luaScript;
	bool isScripted = false;

	// check for report/lua param
	for (int i=1;i<argc;i++) {
		std::string arg(argv[i]);
		if (arg == "-report")
		{
			mode = 2;
		}
		else
		{
			luaScript = arg;
			isScripted = true;
		}
	}

	// change our run parameters if we are in report mode
	if (mode == 2)
	{
		logger.log("Running in report mode... please wait while I play...");
		games = 100;
		CRAPS_LOGGING_ENABLED = false;
	}

	// run the games
	for (int i=0;i<games;i++) {

		logger.log("\n\n--- New Game Starting ---\n\n");

		GameObj game;
		Player* player;

		if (isScripted)
		{
			player = new MyLuaPlayer();
			player->setScriptName(luaScript);
		}
		else
		{
			player = new MyStandardPlayer();
		}

		player->give(1000);
		game.addPlayer(player);

		for (int j=0;j<roundsPerGame;j++)
		{
			game.play();
		}

		logger.log("\n\n -- End Of Game Report --\n\n");
		logger.log("At the end of this game, %s has $%.2f", player->playerName.c_str(), player->bankRoll);
		logger.log("The minimum bankroll for the player during this game was $%.2f", player->playerMin);
		logger.log("The maximum bankroll for the player during this game was $%.2f", player->playerMax);

		// add to rolling average
		playerTotal += player->bankRoll;

		// maintain min/max
		if (player->bankRoll < playerMin)
		{
			playerMin = player->bankRoll;
		}
		if (player->bankRoll > playerMax)
		{
			playerMax = player->bankRoll;
		}
	}

	CRAPS_LOGGING_ENABLED = true;
	double playerAverage = playerTotal / games;
	logger.log("\n\n -- End Of Run Report --\n\n");
	logger.log("After %d games the average ending bankroll was $%.2f", games, playerAverage);
	logger.log("After %d games the minimum ending bankroll was $%.2f", games, playerMin);
	logger.log("After %d games the maximum ending bankroll was $%.2f", games, playerMax);

	return 0;
}
