#pragma once

#include "GameObj.h"

class Player
{
public:
	Player(void);
	~Player(void);

	// some high level objects
	std::string playerName;
	std::string scriptName;
	Logger* logger;
	GameObj* game;
	
	double playerMin;
	double playerMax;

	// some high level methods
	void setGame(GameObj* game);
	void setPlayerName(std::string playerName);
	void give(int amt);
	void report();
	int getTotalOnTable();

	// what players should implement
	virtual void play();
	virtual void setScriptName(std::string scriptName);

	// all below methods are used by sub-class players to execute their logic

	// information about current status
	double bankRoll;
	bool justMovedComeBet;
	bool justEstablishedPoint;
	bool justSevenedOut;
	bool justTookDownComeBet;

	// current bets
	int passBet;
	int passOdds;
	int dontPass;
	int dontPassOdds;
	int comeBet;
	int fieldBet;
	int place4;
	int place5;
	int place6;
	int place8;
	int place9;
	int place10;
	int come4;
	int come5;
	int come6;
	int come8;
	int come9;
	int come10;
	int come4Odds;
	int come5Odds;
	int come6Odds;
	int come8Odds;
	int come9Odds;
	int come10Odds;
	int hard4;
	int hard6;
	int hard8;
	int hard10;
	int any7;
	int anyCraps;
	int yoBet;
	int splitCandE;

	// moves a player can make
	void placePassBet(int amt);
	void takePassOdds(int amt);
	void placeDontPassBet(int amt);
	void takeDontPassOdds(int amt);
	void placeComeBet(int amt);
	void takeComeOddsLastNumber(int amt);
	void placeFieldBet(int amt);
	void makePlaceBet(int num, int amt);
	void makeHardwayBet(int num, int amt);
	void placeAnySevenBet(int amt);
	void placeAnyCrapsBet(int amt);
	void placeYoBet(int amt);
	void placeSplitCandE(int amt);

	// get some global game info
	int getGameState();
	int getGamePoint();
	int getGameLastNumber();

};

