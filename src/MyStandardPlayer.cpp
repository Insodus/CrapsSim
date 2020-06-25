#include "StdAfx.h"
#include "MyStandardPlayer.h"

MyStandardPlayer::MyStandardPlayer(void)
{
	setPlayerName("MyStandardPlayer");
	myComeBetsThisHand = 0;
}

MyStandardPlayer::~MyStandardPlayer(void)
{
}

void MyStandardPlayer::play()
{
	// maintain my come bet counter
	if (justSevenedOut)
	{
		myComeBetsThisHand = 0;
	}
	if (justTookDownComeBet)
	{
		myComeBetsThisHand--;
	}

	// point is off, only basic play
	if (getGameState() == 1)
	{
		if (passBet < 5)
		{
			placePassBet(5);
		}
	}

	// point is on, start betting
	else
	{
		// always take our pass odds, more odds for 6 and 8
		if (passOdds < 5)
		{
			switch (getGamePoint())
			{
			case 4:
			case 5:
			case 9:
			case 10:
				takePassOdds(10);
				break;
			case 6:
			case 8:
				takePassOdds(25);
				break;
			}
		}

		// make 3 come bets
		if (myComeBetsThisHand < 3 && comeBet < 5)
		{
			myComeBetsThisHand++;
			placeComeBet(5);
		}

		// take come odds if it was just moved to a number
		if (justMovedComeBet)
		{
			switch (getGameLastNumber())
			{
			case 4:
			case 5:
			case 9:
			case 10:
				takeComeOddsLastNumber(10);
				break;
			case 6:
			case 8:
				takeComeOddsLastNumber(25);
				break;
			}
		}

	}

}