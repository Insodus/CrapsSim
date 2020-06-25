#include "StdAfx.h"
#include "Player.h"
#include "GameObj.h"

Player::Player(void)
{
	this->logger = new Logger();
	bankRoll = 0;

	justMovedComeBet = false;
	justEstablishedPoint = false;
	justSevenedOut = false;
	justTookDownComeBet = false;

	playerMin = 100000;
	playerMax = 0;

	passBet = 0;
	passOdds = 0;
	dontPass = 0;
	dontPassOdds = 0;
	comeBet = 0;
	fieldBet = 0;
	place4 = 0;
	place5 = 0;
	place6 = 0;
	place8 = 0;
	place9 = 0;
	place10 = 0;
	come4 = 0;
	come5 = 0;
	come6 = 0;
	come8 = 0;
	come9 = 0;
	come10 = 0;
	come4Odds = 0;
	come5Odds = 0;
	come6Odds = 0;
	come8Odds = 0;
	come9Odds = 0;
	come10Odds = 0;
}

Player::~Player(void)
{
}

void Player::setGame(GameObj* game)
{
	this->game = game;
}

void Player::setPlayerName(std::string playerName)
{
	this->playerName = playerName;
	logger->setPrefix(playerName);
}

void Player::setScriptName(std::string scriptName)
{
	this->scriptName = scriptName;
}

void Player::give(int amt)
{
	bankRoll += amt;
	logger->log("Recieved %d, new total is $%.2f", amt, bankRoll);
}

void Player::report()
{
	logger->log("I currently have a bankroll of $%.2f", bankRoll);
	logger->log("I currently have $%d on the table", getTotalOnTable());
}

int Player::getTotalOnTable()
{
	int totalDown = passBet + passOdds  + dontPass + 
				dontPassOdds + comeBet + fieldBet + place4 + 
				place5 + place6 + place8 + place9 + 
				place10 + come4 + come5 + come6 + 
				come8 + come9 + come10 + come4Odds + 
				come5Odds + come6Odds + come8Odds + 
				come9Odds + come10Odds;
	return totalDown;
}

int Player::getGameState()
{
	return game->getState();
}

int Player::getGamePoint()
{
	return game->getPoint();
}

int Player::getGameLastNumber()
{
	return game->getLastNumber();
}

void Player::placePassBet(int amt)
{
	if (amt > bankRoll)
	{
		logger->log("That is more money than you have at this time.");
		return;
	}

	if (game->getState() == game->STATE_ON)
	{
		logger->log("Can not make a pass bet at this time.");
		return;
	}

	passBet += amt;
	bankRoll -= amt;
	logger->log("Making pass bet of %d, my bankroll is now $%.2f", amt, bankRoll);
}

void Player::takePassOdds(int amt)
{
	if (amt > bankRoll)
	{
		logger->log("That is more money than you have at this time.");
		return;
	}

	if (game->getState() == game->STATE_OFF)
	{
		logger->log("Can not take pass odds at this time.");
		return;
	}

	if (passBet == 0)
	{
		logger->log("You can not take pass odds without a pass bet.");
		return;
	}

	passOdds += amt;
	bankRoll -= amt;
	logger->log("Taking pass odds of %d, my bankroll is now $%.2f", amt, bankRoll);
}

void Player::placeDontPassBet(int amt)
{
	if (amt > bankRoll)
	{
		logger->log("That is more money than you have at this time.");
		return;
	}

	if (game->getState() == game->STATE_ON)
	{
		logger->log("Can not make a dont pass bet at this time.");
		return;
	}

	dontPass =+ amt;
	bankRoll -= amt;
	logger->log("Making dont pass bet of %d, my bankroll is now $%.2f", amt, bankRoll);
}

void Player::takeDontPassOdds(int amt)
{
	if (amt > bankRoll)
	{
		logger->log("That is more money than you have at this time.");
		return;
	}

	if (game->getState() == game->STATE_OFF)
	{
		logger->log("Can not take dont pass odds at this time.");
		return;
	}

	if (dontPass == 0)
	{
		logger->log("You can not take dont pass odds without a dont pass bet.");
		return;
	}

	dontPassOdds += amt;
	bankRoll -= amt;
	logger->log("Taking dont pass odds of %d, my bankroll is now $%.2f", amt, bankRoll);
}

void Player::placeComeBet(int amt)
{
	if (amt > bankRoll)
	{
		logger->log("That is more money than you have at this time.");
		return;
	}

	if (game->getState() == game->STATE_OFF)
	{
		logger->log("Can not make a come bet at this time.");
		return;
	}

	comeBet += amt;
	bankRoll -= amt;
	logger->log("Making come bet of %d, my bankroll is now $%.2f", amt, bankRoll);
}

void Player::takeComeOddsLastNumber(int amt)
{
	if (amt > bankRoll)
	{
		logger->log("That is more money than you have at this time.");
		return;
	}

	if (!justMovedComeBet)
	{
		logger->log("You didnt have a come bet last hand.");
		return;
	}

	switch (game->getLastNumber())
	{
	case 4:
		come4Odds += amt;
		break;
	case 5:
		come5Odds += amt;
		break;
	case 6:
		come6Odds += amt;
		break;
	case 8:
		come8Odds += amt;
		break;
	case 9:
		come9Odds += amt;
		break;
	case 10:
		come10Odds += amt;
		break;
	}

	bankRoll -= amt;
	logger->log("Taking come odds of %d, my bankroll is now $%.2f", amt, bankRoll);
}

void Player::placeFieldBet(int amt)
{
	if (amt > bankRoll)
	{
		logger->log("That is more money than you have at this time.");
		return;
	}

	if (game->getState() == game->STATE_OFF)
	{
		logger->log("Can not make a field bet at this time.");
		return;
	}

	fieldBet += amt;
	bankRoll -= amt;
	logger->log("Making field bet of %d, my bankroll is now $%.2f", amt, bankRoll);
}

void Player::makePlaceBet(int num, int amt)
{
	if (num != 4 && num != 5 && num != 6 && num != 8 && num != 9 && num != 10)
	{
		logger->log("You cant place bet on number %d", num);
		return;
	}

	if (amt > bankRoll)
	{
		logger->log("That is more money than you have at this time.");
		return;
	}

	if (game->getState() == game->STATE_OFF)
	{
		logger->log("Can not make a field bet at this time.");
		return;
	}

	switch (num)
	{
	case 4:
		place4 += amt;
		break;
	case 5:
		place5 += amt;
		break;
	case 6:
		place6 += amt;
		break;
	case 8:
		place8 += amt;
		break;
	case 9:
		place9 += amt;
		break;
	case 10:
		place10 += amt;
		break;
	}

	bankRoll -= amt;
	logger->log("Making place bet on %d of %d, my bankroll is now $%.2f", num, amt, bankRoll);
}

void Player::makeHardwayBet(int num, int amt)
{
	if (num != 4 && num != 6 && num != 8 && num != 10)
	{
		logger->log("You cant make a hardway bet on number %d", num);
		return;
	}

	if (amt > bankRoll)
	{
		logger->log("That is more money than you have at this time.");
		return;
	}

	if (game->getState() == game->STATE_OFF)
	{
		logger->log("Can not make a hardway bet at this time.");
		return;
	}

	switch (num)
	{
	case 4:
		hard4 += amt;
		break;
	case 6:
		hard6 += amt;
		break;
	case 8:
		hard8 += amt;
		break;
	case 10:
		hard10 += amt;
		break;
	}

	bankRoll -= amt;
	logger->log("Making hardway bet on %d of %d, my bankroll is now $%.2f", num, amt, bankRoll);
}

void Player::placeAnySevenBet(int amt)
{
	if (amt > bankRoll)
	{
		logger->log("That is more money than you have at this time.");
		return;
	}

	any7 += amt;
	bankRoll -= amt;
	logger->log("Making any 7 bet of %d, my bankroll is now $%.2f", amt, bankRoll);
}

void Player::placeAnyCrapsBet(int amt)
{
	if (amt > bankRoll)
	{
		logger->log("That is more money than you have at this time.");
		return;
	}

	anyCraps += amt;
	bankRoll -= amt;
	logger->log("Making any craps bet of %d, my bankroll is now $%.2f", amt, bankRoll);
}

void Player::placeYoBet(int amt)
{
	if (amt > bankRoll)
	{
		logger->log("That is more money than you have at this time.");
		return;
	}

	yoBet += amt;
	bankRoll -= amt;
	logger->log("Making yo bet of %d, my bankroll is now $%.2f", amt, bankRoll);
}

void Player::placeSplitCandE(int amt)
{
	if (amt > bankRoll)
	{
		logger->log("That is more money than you have at this time.");
		return;
	}

	splitCandE += amt;
	bankRoll -= amt;
	logger->log("Making split C and E bet of %d, my bankroll is now $%.2f", amt, bankRoll);
}

void Player::play()
{
	logger->log("I'm a stupid default player.");
}