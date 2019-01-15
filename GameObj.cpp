#include "StdAfx.h"
#include "GameObj.h"
#include "Logger.h"
#include "Player.h"
#include "RandomGenProvider.h"
#include "WinCryptoRandom.h"

GameObj::GameObj(void)
{
	state = STATE_OFF;
	point = 0;
	lastNumber = 0;
	currPlayers = 0;
	logger = new Logger();
	logger->setPrefix("Dealer");
	randGen = new WinCryptoRandom();
}

GameObj::~GameObj(void)
{
}

void GameObj::addPlayer(Player* player)
{
	players[currPlayers] = player;
	currPlayers++;
	player->setGame(this);
}

int GameObj::getPoint()
{
	return point;
}

void GameObj::setPoint(int point)
{
	this->point = point;
}

int GameObj::getLastNumber()
{
	return lastNumber;
}

int GameObj::getState()
{
	return state;
}

void GameObj::setState(int state)
{
	this->state = state;
}

int GameObj::getRoll()
{
	return randGen->getRandom(1, 6);
}

void GameObj::play()
{
	logger->log("Asking players for bets...");
	for (int i=0;i<currPlayers;i++)
	{
		Player* player = players[i];
		
		// maintain min/max
		if (player->bankRoll < player->playerMin)
		{
			player->playerMin = player->bankRoll;
		}
		if (player->bankRoll > player->playerMax)
		{
			player->playerMax = player->bankRoll;
		}

		// ask them to play
		player->play();

		// reset notifications
		player->justMovedComeBet = false;
		player->justEstablishedPoint = false;
		player->justSevenedOut = false;
	}

	logger->log("Rolling...");
	int roll1 = getRoll();
	int roll2 = getRoll();
	int total = roll1 + roll2;
	bool hard = (roll1 == roll2);
	logger->log("Roll is %d + %d = %d", roll1, roll2, total);
	lastNumber = total;

	// point is off
	if (getState() == STATE_OFF) {
		if (total == 2 || total == 3 || total == 12)
		{
			logger->log("Craps.");
			for (int i=0;i<currPlayers;i++)
			{
				Player* player = players[i];
				if (player->passBet > 0)
				{
					logger->log("Taking down %s's pass bet ($%d)", player->playerName.c_str(), player->passBet);
					player->passBet = 0;
				}

				if (player->dontPass > 0)
				{
					if (total == 2 || total == 3)
					{
						player->bankRoll += player->dontPass;
						logger->log("Paying %s for dont pass bet ($%d) and leaving it up, bankroll is now $%.2f", player->playerName.c_str(), player->dontPass, player->bankRoll);
					}
				}
			}
		}
		else if (total == 7 || total == 11)
		{
			logger->log("Seven winner.");
			for (int i=0;i<currPlayers;i++)
			{
				Player* player = players[i];
				if (player->passBet > 0)
				{
					player->bankRoll += player->passBet;
					logger->log("Paying %s for pass bet ($%d) and leaving it up, bankroll is now $%.2f", player->playerName.c_str(), player->passBet, player->bankRoll);
				}

				if (player->dontPass > 0)
				{
					logger->log("Taking down %s's dont pass bet ($%d)", player->playerName.c_str(), player->dontPass);
					player->dontPass = 0;
				}
			}
		}
		else
		{
			logger->log("Point is now %d.", total);

			for (int i=0;i<currPlayers;i++)
			{
				Player* player = players[i];
				player->justEstablishedPoint = true;

				// pay any old come bets
				switch (total)
				{
				case 4:
					if (player->come4 > 0)
					{
						int payout = (player->come4 * 2);
						player->bankRoll += payout;
						player->come4 = 0;
						logger->log("Paying %s for old come ($%d) on #4 including taking it down, bankroll is now $%.2f", player->playerName.c_str(), payout, player->bankRoll);
						player->justTookDownComeBet = true;

						if (player->come4Odds > 0)
						{
							player->bankRoll += player->come4Odds;
							logger->log("Giving %s unused odds ($%d) for old come on #4, bankroll is now $%.2f", player->playerName.c_str(), player->come4Odds, player->bankRoll);
							player->come4Odds = 0;
						}
					}
					break;
				case 5:
					if (player->come5 > 0)
					{
						int payout = (player->come5 * 2);
						player->bankRoll += payout;
						player->come5 = 0;
						logger->log("Paying %s for old come ($%d) on #5 including taking it down, bankroll is now $%.2f", player->playerName.c_str(), payout, player->bankRoll);
						player->justTookDownComeBet = true;

						if (player->come5Odds > 0)
						{
							player->bankRoll += player->come5Odds;
							logger->log("Giving %s unused odds ($%d) for old come on #5, bankroll is now $%.2f", player->playerName.c_str(), player->come5Odds, player->bankRoll);
							player->come5Odds = 0;
						}
					}
					break;
				case 6:
					if (player->come6 > 0)
					{
						int payout = (player->come6 * 2);
						player->bankRoll += payout;
						player->come6 = 0;
						logger->log("Paying %s for old come ($%d) on #6 including taking it down, bankroll is now $%.2f", player->playerName.c_str(), payout, player->bankRoll);
						player->justTookDownComeBet = true;

						if (player->come6Odds > 0)
						{
							player->bankRoll += player->come6Odds;
							logger->log("Giving %s unused odds ($%d) for old come on #6, bankroll is now $%.2f", player->playerName.c_str(), player->come5Odds, player->bankRoll);
							player->come6Odds = 0;
						}
					}
					break;
				case 8:
					if (player->come8 > 0)
					{
						int payout = (player->come8 * 2);
						player->bankRoll += payout;
						player->come8 = 0;
						logger->log("Paying %s for old come ($%d) on #8 including taking it down, bankroll is now $%.2f", player->playerName.c_str(), payout, player->bankRoll);
						player->justTookDownComeBet = true;

						if (player->come8Odds > 0)
						{
							player->bankRoll += player->come8Odds;
							logger->log("Giving %s unused odds ($%d) for old come on #8, bankroll is now $%.2f", player->playerName.c_str(), player->come5Odds, player->bankRoll);
							player->come8Odds = 0;
						}
					}
					break;
				case 9:
					if (player->come9 > 0)
					{
						int payout = (player->come9 * 2);
						player->bankRoll += payout;
						player->come9 = 0;
						logger->log("Paying %s for old come ($%d) on #9 including taking it down, bankroll is now $%.2f", player->playerName.c_str(), payout, player->bankRoll);
						player->justTookDownComeBet = true;

						if (player->come9Odds > 0)
						{
							player->bankRoll += player->come9Odds;
							logger->log("Giving %s unused odds ($%d) for old come on #9, bankroll is now $%.2f", player->playerName.c_str(), player->come5Odds, player->bankRoll);
							player->come9Odds = 0;
						}
					}
					break;
				case 10:
					if (player->come10 > 0)
					{
						int payout = (player->come10 * 2);
						player->bankRoll += payout;
						player->come10 = 0;
						logger->log("Paying %s for old come ($%d) on #10 including taking it down, bankroll is now $%.2f", player->playerName.c_str(), payout, player->bankRoll);
						player->justTookDownComeBet = true;

						if (player->come10Odds > 0)
						{
							player->bankRoll += player->come10Odds;
							logger->log("Giving %s unused odds ($%d) for old come on #10, bankroll is now $%.2f", player->playerName.c_str(), player->come5Odds, player->bankRoll);
							player->come10Odds = 0;
						}
					}
					break;
				}
			}

			setState(STATE_ON);
			setPoint(total);
		}
	}

	// point is on
	else
	{
		// seven out
		if (total == 7)
		{
			logger->log("Seven out.");

			for (int i=0;i<currPlayers;i++)
			{
				Player* player = players[i];

				// clear bets
				if (player->passBet > 0)
				{
					logger->log("Taking down %s's pass bet ($%d)", player->playerName.c_str(), player->passBet);
					player->passBet = 0;
				}
				if (player->passOdds > 0)
				{
					logger->log("Taking down %s's pass odds ($%d)", player->playerName.c_str(), player->passOdds);
					player->passOdds = 0;
				}
				if (player->fieldBet > 0)
				{
					logger->log("Taking down %s's field bet ($%d)", player->playerName.c_str(), player->fieldBet);
					player->fieldBet = 0;
				}
				if (player->place4 > 0)
				{
					logger->log("Taking down %s's place bet ($%d) on #4", player->playerName.c_str(), player->place4);
					player->place4 = 0;
				}
				if (player->place5 > 0)
				{
					logger->log("Taking down %s's place bet ($%d) on #5", player->playerName.c_str(), player->place5);
					player->place5 = 0;
				}
				if (player->place6 > 0)
				{
					logger->log("Taking down %s's place bet ($%d) on #6", player->playerName.c_str(), player->place6);
					player->place6 = 0;
				}
				if (player->place8 > 0)
				{
					logger->log("Taking down %s's place bet ($%d) on #8", player->playerName.c_str(), player->place8);
					player->place8 = 0;
				}
				if (player->place9 > 0)
				{
					logger->log("Taking down %s's place bet ($%d) on #9", player->playerName.c_str(), player->place9);
					player->place9 = 0;
				}
				if (player->place10 > 0)
				{
					logger->log("Taking down %s's place bet ($%d) on #10", player->playerName.c_str(), player->place10);
					player->place10 = 0;
				}
				if (player->come4 > 0)
				{
					logger->log("Taking down %s's come bet ($%d) on #4", player->playerName.c_str(), player->come4);
					player->come4 = 0;
					
					if (player->come4Odds > 0)
					{
						logger->log("Taking down %s's come odds ($%d) on #4", player->playerName.c_str(), player->come4Odds);
						player->come4Odds = 0;
					}
				}
				if (player->come5 > 0)
				{
					logger->log("Taking down %s's come bet ($%d) on #5", player->playerName.c_str(), player->come5);
					player->come5 = 0;
					
					if (player->come5Odds > 0)
					{
						logger->log("Taking down %s's come odds ($%d) on #5", player->playerName.c_str(), player->come5Odds);
						player->come5Odds = 0;
					}
				}
				if (player->come6 > 0)
				{
					logger->log("Taking down %s's come bet ($%d) on #6", player->playerName.c_str(), player->come6);
					player->come6 = 0;
					
					if (player->come6Odds > 0)
					{
						logger->log("Taking down %s's come odds ($%d) on #6", player->playerName.c_str(), player->come6Odds);
						player->come6Odds = 0;
					}
				}
				if (player->come8 > 0)
				{
					logger->log("Taking down %s's come bet ($%d) on #8", player->playerName.c_str(), player->come8);
					player->come8 = 0;
					
					if (player->come8Odds > 0)
					{
						logger->log("Taking down %s's come odds ($%d) on #8", player->playerName.c_str(), player->come8Odds);
						player->come8Odds = 0;
					}
				}
				if (player->come9 > 0)
				{
					logger->log("Taking down %s's come bet ($%d) on #9", player->playerName.c_str(), player->come9);
					player->come9 = 0;
					
					if (player->come9Odds > 0)
					{
						logger->log("Taking down %s's come odds ($%d) on #9", player->playerName.c_str(), player->come9Odds);
						player->come9Odds = 0;
					}
				}
				if (player->come10 > 0)
				{
					logger->log("Taking down %s's come bet ($%d) on #10", player->playerName.c_str(), player->come10);
					player->come10 = 0;
					
					if (player->come10Odds > 0)
					{
						logger->log("Taking down %s's come odds ($%d) on #10", player->playerName.c_str(), player->come10Odds);
						player->come10Odds = 0;
					}
				}
				if (player->hard4 > 0)
				{
					logger->log("Taking down %s's hard 4 bet ($%d)", player->playerName.c_str(), player->hard4);
					player->hard4 = 0;
				}
				if (player->hard6 > 0)
				{
					logger->log("Taking down %s's hard 6 bet ($%d)", player->playerName.c_str(), player->hard6);
					player->hard6 = 0;
				}
				if (player->hard8 > 0)
				{
					logger->log("Taking down %s's hard 8 bet ($%d)", player->playerName.c_str(), player->hard8);
					player->hard8 = 0;
				}
				if (player->hard10 > 0)
				{
					logger->log("Taking down %s's hard 10 bet ($%d)", player->playerName.c_str(), player->hard10);
					player->hard10 = 0;
				}

				// pay dont pass and come
				if (player->dontPass > 0)
				{
					player->bankRoll += player->dontPass;
					logger->log("Paying %s for dont pass bet ($%d) and leaving it up, bankroll is now $%.2f", player->playerName.c_str(), player->dontPass, player->bankRoll);
				}
				if (player->dontPassOdds > 0)
				{
					double payout = player->dontPassOdds;

					switch (total)
					{
					case 4:
					case 10:
						payout += (player->dontPassOdds * 0.5);
						break;
					case 5:
					case 9:
						payout += (player->dontPassOdds * 0.66);
						break;
					case 6:
					case 8:
						payout += (player->dontPassOdds * 0.83);
						break;
					}

					player->bankRoll += payout;
					player->dontPassOdds = 0;
					logger->log("Paying %s for dont pass odds ($%.2f) including taking it down, bankroll is now $%.2f", player->playerName.c_str(), payout, player->bankRoll);
				}
				if (player->comeBet > 0)
				{
					int payout = (player->comeBet * 2);
					player->bankRoll += payout;
					player->comeBet = 0;
					logger->log("Paying %s for come bet ($%d) including taking it down, bankroll is now $%.2f", player->playerName.c_str(), payout, player->bankRoll);
				}

				player->justSevenedOut = true;
			}

			setState(STATE_OFF);
			setPoint(0);
			logger->log("\n\nNew Round.\n\n");
		}
		else if (total == 2 || total == 3 || total == 12)
		{
			logger->log("Craps. Field pays.");

			for (int i=0;i<currPlayers;i++)
			{
				Player* player = players[i];
				if (player->comeBet > 0)
				{
					logger->log("Taking down %s's come bet ($%d)", player->playerName.c_str(), player->comeBet);
					player->comeBet = 0;
				}

				if (player->fieldBet > 0)
				{
					if (total == 2 || total == 12)
					{
						int payout = (player->fieldBet * 3);
						player->bankRoll += payout;
						logger->log("Paying %s for field bet ($%d) including taking it down, bankroll is now $%.2f", player->playerName.c_str(), payout, player->bankRoll);
						player->fieldBet = 0;
					}
					else
					{
						int payout = (player->fieldBet * 2);
						player->bankRoll += payout;
						logger->log("Paying %s for field bet ($%d) including taking it down, bankroll is now $%.2f", player->playerName.c_str(), payout, player->bankRoll);
						player->fieldBet = 0;
					}
				}
			}
		}
		else if (total == 11)
		{
			logger->log("Yo.");
			for (int i=0;i<currPlayers;i++)
			{
				Player* player = players[i];
				if (player->comeBet > 0)
				{
					player->bankRoll += player->comeBet;
					logger->log("Paying %s for come bet ($%d) and leaving it up, bankroll is now $%.2f", player->playerName.c_str(), player->comeBet, player->bankRoll);
				}
				if (player->fieldBet > 0)
				{
					int payout = (player->fieldBet * 2);
					player->bankRoll += payout;
					logger->log("Paying %s for field bet ($%d) including taking it down, bankroll is now $%.2f", player->playerName.c_str(), payout, player->bankRoll);
					player->fieldBet = 0;
				} 
			}
		}
		else if (total == getPoint())
		{
			logger->log("Winner %d.", getPoint());

			for (int i=0;i<currPlayers;i++)
			{
				Player* player = players[i];

				// pay pass line
				if (player->passBet > 0)
				{
					player->bankRoll += player->passBet;
					logger->log("Paying %s for pass bet ($%d) and leaving it up, bankroll is now $%.2f", player->playerName.c_str(), player->passBet, player->bankRoll);
				}
				if (player->passOdds > 0)
				{
					double payout = player->passOdds;

					switch (total)
					{
					case 4:
					case 10:
						payout += (player->passOdds * 2);
						break;
					case 5:
					case 9:
						payout += (player->passOdds * 1.5);
						break;
					case 6:
					case 8:
						payout += (player->passOdds * 1.2);
						break;
					}

					player->bankRoll += payout;
					player->passOdds = 0;
					logger->log("Paying %s for pass odds ($%.2f) including taking it down, bankroll is now $%.2f", player->playerName.c_str(), payout, player->bankRoll);
				}

				// pay field
				if (player->fieldBet > 0)
				{
					if (total == 4 || total == 9 || total == 10)
					{
						int payout = (player->fieldBet * 2);
						player->bankRoll += payout;
						logger->log("Paying %s for field bet ($%d) including taking it down, bankroll is now $%.2f", player->playerName.c_str(), payout, player->bankRoll);
						player->fieldBet = 0;
					}
					else
					{
						logger->log("Taking down %s's field bet ($%d)", player->playerName.c_str(), player->fieldBet);
						player->fieldBet = 0;
					}
				}

				// move come bets
				if (player->comeBet > 0)
				{
					switch (total)
					{
					case 4:
						logger->log("Moving %s's come bet (%d) to #4", player->playerName.c_str(), player->comeBet);
						player->come4 = player->comeBet;
						break;
					case 5:
						logger->log("Moving %s's come bet (%d) to #5", player->playerName.c_str(), player->comeBet);
						player->come5 = player->comeBet;
						break;
					case 6:
						logger->log("Moving %s's come bet (%d) to #6", player->playerName.c_str(), player->comeBet);
						player->come6 = player->comeBet;
						break;
					case 8:
						logger->log("Moving %s's come bet (%d) to #8", player->playerName.c_str(), player->comeBet);
						player->come8 = player->comeBet;
						break;
					case 9:
						logger->log("Moving %s's come bet (%d) to #9", player->playerName.c_str(), player->comeBet);
						player->come9 = player->comeBet;
						break;
					case 10:
						logger->log("Moving %s's come bet (%d) to #10", player->playerName.c_str(), player->comeBet);
						player->come10 = player->comeBet;
						break;
					}

					player->comeBet = 0;
					player->justMovedComeBet = true;
				}

				// clear dont pass
				if (player->dontPass > 0)
				{
					logger->log("Taking down %s's dont pass bet ($%d)", player->playerName.c_str(), player->dontPass);
					player->dontPass = 0;
				}
				if (player->dontPassOdds > 0)
				{
					logger->log("Taking down %s's dont pass odds ($%d)", player->playerName.c_str(), player->dontPassOdds);
					player->dontPassOdds = 0;
				}
			}

			setState(STATE_OFF);
			setPoint(0);
			logger->log("\n\nNew Round.\n\n");
		}
		else 
		{
			logger->log("Pay on %d ", total);

			for (int i=0;i<currPlayers;i++)
			{
				Player* player = players[i];

				// pay the number
				switch (total)
				{
				case 4:
					if (player->place4 > 0)
					{
						double payout = (player->place4 * 1.8);
						player->bankRoll += payout;
						logger->log("Paying %s for place bet ($%.2f) on #4 and leaving it up, bankroll is now $%.2f", player->playerName.c_str(), payout, player->bankRoll);
					}
					if (player->come4 > 0) {
						int payout = (player->come4 * 2);
						player->bankRoll += payout;
						logger->log("Paying %s for come bet ($%d) on #4 including taking it down, bankroll is now $%.2f", player->playerName.c_str(), payout, player->bankRoll);
						player->come4 = 0;

						if (player->come4Odds > 0)
						{
							double payout = player->come4Odds + (player->come4Odds * 2);
							player->bankRoll += payout;
							logger->log("Paying %s for come bet odds ($%.2f) on #4 including taking it down, bankroll is now $%.2f", player->playerName.c_str(), payout, player->bankRoll);
							player->come4Odds = 0;
						}
					}
					break;
				case 5:
					if (player->place5 > 0)
					{
						double payout = (player->place5 * 1.4);
						player->bankRoll += payout;
						logger->log("Paying %s for place bet ($%.2f) on #5 and leaving it up, bankroll is now $%.2f", player->playerName.c_str(), payout, player->bankRoll);
					}
					if (player->come5 > 0) {
						int payout = (player->come5 * 2);
						player->bankRoll += payout;
						logger->log("Paying %s for come bet ($%d) on #5 including taking it down, bankroll is now $%.2f", player->playerName.c_str(), payout, player->bankRoll);
						player->come5 = 0;

						if (player->come5Odds > 0)
						{
							double payout = player->come5Odds + (player->come5Odds * 1.5);
							player->bankRoll += payout;
							logger->log("Paying %s for come bet odds ($%.2f) on #5 including taking it down, bankroll is now $%.2f", player->playerName.c_str(), payout, player->bankRoll);
							player->come5Odds = 0;
						}
					}
					break;
				case 6:
					if (player->place6 > 0)
					{
						double payout = (player->place6 * 1.16);
						player->bankRoll += payout;
						logger->log("Paying %s for place bet ($%.2f) on #6 and leaving it up, bankroll is now $%.2f", player->playerName.c_str(), payout, player->bankRoll);
					}
					if (player->come6 > 0) {
						int payout = (player->come6 * 2);
						player->bankRoll += payout;
						logger->log("Paying %s for come bet ($%d) on #6 including taking it down, bankroll is now $%.2f", player->playerName.c_str(), payout, player->bankRoll);
						player->come6 = 0;

						if (player->come6Odds > 0)
						{
							double payout = player->come6Odds + (player->come6Odds * 1.2);
							player->bankRoll += payout;
							logger->log("Paying %s for come bet odds ($%.2f) on #6 including taking it down, bankroll is now $%.2f", player->playerName.c_str(), payout, player->bankRoll);
							player->come6Odds = 0;
						}
					}
					break;
				case 8:
					if (player->place8 > 0)
					{
						double payout = (player->place8 * 1.16);
						player->bankRoll += payout;
						logger->log("Paying %s for place bet ($%.2f) on #8 and leaving it up, bankroll is now $%.2f", player->playerName.c_str(), payout, player->bankRoll);
					}
					if (player->come8 > 0) {
						int payout = (player->come8 * 2);
						player->bankRoll += payout;
						logger->log("Paying %s for come bet ($%d) on #8 including taking it down, bankroll is now $%.2f", player->playerName.c_str(), payout, player->bankRoll);
						player->come8 = 0;

						if (player->come8Odds > 0)
						{
							double payout = player->come8Odds + (player->come8Odds * 1.2);
							player->bankRoll += payout;
							logger->log("Paying %s for come bet odds ($%.2f) on #8 including taking it down, bankroll is now $%.2f", player->playerName.c_str(), payout, player->bankRoll);
							player->come8Odds = 0;
						}
					}
					break;
				case 9:
					if (player->place9 > 0)
					{
						double payout = (player->place9 * 1.4);
						player->bankRoll += payout;
						logger->log("Paying %s for place bet ($%.2f) on #9 and leaving it up, bankroll is now $%.2f", player->playerName.c_str(), payout, player->bankRoll);
					}
					if (player->come9 > 0) {
						int payout = (player->come9 * 2);
						player->bankRoll += payout;
						logger->log("Paying %s for come bet ($%d) on #9 including taking it down, bankroll is now $%.2f", player->playerName.c_str(), payout, player->bankRoll);
						player->come9 = 0;

						if (player->come9Odds > 0)
						{
							double payout = player->come9Odds + (player->come9Odds * 1.5);
							player->bankRoll += payout;
							logger->log("Paying %s for come bet odds ($%.2f) on #9 including taking it down, bankroll is now $%.2f", player->playerName.c_str(), payout, player->bankRoll);
							player->come9Odds = 0;
						}
					}
					break;
				case 10:
					if (player->place10 > 0)
					{
						double payout = (player->place10 * 1.8);
						player->bankRoll += payout;
						logger->log("Paying %s for place bet ($%.2f) on #10 and leaving it up, bankroll is now $%.2f", player->playerName.c_str(), payout, player->bankRoll);
					}
					if (player->come10 > 0) {
						int payout = (player->come10 * 2);
						player->bankRoll += payout;
						logger->log("Paying %s for come bet ($%d) on #10 including taking it down, bankroll is now $%.2f", player->playerName.c_str(), payout, player->bankRoll);
						player->come10 = 0;

						if (player->come10Odds > 0)
						{
							double payout = player->come10Odds + (player->come10Odds * 2);
							player->bankRoll += payout;
							logger->log("Paying %s for come bet odds ($%.2f) on #10 including taking it down, bankroll is now $%.2f", player->playerName.c_str(), payout, player->bankRoll);
							player->come10Odds = 0;
						}
					}
					break;
				}

				// pay field
				if (player->fieldBet > 0)
				{
					if (total == 4 || total == 9 || total == 10)
					{
						int payout = (player->fieldBet * 2);
						player->bankRoll += payout;
						logger->log("Paying %s for field bet ($%d) including taking it down, bankroll is now $%.2f", player->playerName.c_str(), payout, player->bankRoll);
						player->fieldBet = 0;
					}
					else
					{
						logger->log("Taking down %s's field bet ($%d)", player->playerName.c_str(), player->fieldBet);
						player->fieldBet = 0;
					}
				}

				// move come bets
				if (player->comeBet > 0)
				{
					switch (total)
					{
					case 4:
						logger->log("Moving %s's come bet (%d) to #4", player->playerName.c_str(), player->comeBet);
						player->come4 = player->comeBet;
						break;
					case 5:
						logger->log("Moving %s's come bet (%d) to #5", player->playerName.c_str(), player->comeBet);
						player->come5 = player->comeBet;
						break;
					case 6:
						logger->log("Moving %s's come bet (%d) to #6", player->playerName.c_str(), player->comeBet);
						player->come6 = player->comeBet;
						break;
					case 8:
						logger->log("Moving %s's come bet (%d) to #8", player->playerName.c_str(), player->comeBet);
						player->come8 = player->comeBet;
						break;
					case 9:
						logger->log("Moving %s's come bet (%d) to #9", player->playerName.c_str(), player->comeBet);
						player->come9 = player->comeBet;
						break;
					case 10:
						logger->log("Moving %s's come bet (%d) to #10", player->playerName.c_str(), player->comeBet);
						player->come10 = player->comeBet;
						break;
					}

					player->comeBet = 0;
					player->justMovedComeBet = true;
				}
			}
		}

		// now do hard ways
		if (total == 4 || total == 6 || total == 8 || total == 10)
		{
			if (roll1 == roll2)
			{
				logger->log("Pay hard way on %d ", total);

				for (int i=0;i<currPlayers;i++)
				{
					Player* player = players[i];

					if (total == 4 && player->hard4 > 0)
					{
						int payout = (player->hard4 * 7);
						player->bankRoll += payout;
						logger->log("Paying %s for hard 4 bet ($%d) and leaving it up, bankroll is now $%.2f", player->playerName.c_str(), payout, player->bankRoll);
					}
					if (total == 6 && player->hard6 > 0)
					{
						int payout = (player->hard6 * 9);
						player->bankRoll += payout;
						logger->log("Paying %s for hard 6 bet ($%d) and leaving it up, bankroll is now $%.2f", player->playerName.c_str(), payout, player->bankRoll);
					}
					if (total == 8 && player->hard8 > 0)
					{
						int payout = (player->hard8 * 9);
						player->bankRoll += payout;
						logger->log("Paying %s for hard 8 bet ($%d) and leaving it up, bankroll is now $%.2f", player->playerName.c_str(), payout, player->bankRoll);
					}
					if (total == 10 && player->hard10 > 0)
					{
						int payout = (player->hard10 * 7);
						player->bankRoll += payout;
						logger->log("Paying %s for hard 10 bet ($%d) and leaving it up, bankroll is now $%.2f", player->playerName.c_str(), payout, player->bankRoll);
					}
				}
			}
			else
			{
				logger->log("Hard ways down on %d ", total);

				for (int i=0;i<currPlayers;i++)
				{
					Player* player = players[i];

					if (total == 4 && player->hard4 > 0)
					{
						logger->log("Taking down %s's hard 4 bet ($%d)", player->playerName.c_str(), player->hard4);
						player->hard4 = 0;
					}
					if (total == 6 && player->hard6 > 0)
					{
						logger->log("Taking down %s's hard 6 bet ($%d)", player->playerName.c_str(), player->hard6);
						player->hard4 = 0;
					}
					if (total == 8 && player->hard8 > 0)
					{
						logger->log("Taking down %s's hard 8 bet ($%d)", player->playerName.c_str(), player->hard8);
						player->hard4 = 0;
					}
					if (total == 10 && player->hard10 > 0)
					{
						logger->log("Taking down %s's hard 10 bet ($%d)", player->playerName.c_str(), player->hard10);
						player->hard4 = 0;
					}
				}
			}
		}
	}

	// point is on or off here, handle any 7, craps, yo

}


